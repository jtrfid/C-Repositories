m#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ElevatorLib.h"

/**********************************************
 * Idle状态，电梯停止在某楼层，门是关闭的，处于静止状态，等待相关事件的发生，从而转换到下一个状态。
 **********************************************/
void StateIdle(int *state)
{
	int floor; bool up;
	floor = IdleWhatFloorToGoTo(&up);                                       //检测将要到达的楼层。
	if (floor > 0 && up)                                                     //楼层高于当前楼层，上升。
	{
		SetMotorPower(1);                                                  
		*state = MovingUp;                                                 //  状态机由Idle状态变为MovingUp。
	}
	if (floor>0 && !up)
	{
		SetMotorPower(-1);
		*state = MovingDown;                                              //  状态机由Idle状态变为MovingDown。      

	}
	if (GetOpenDoorLight())                                                //等待开门按钮的指令。
	{
		SetOpenDoorLight(false);
		SetDoor;
		*state = DoorOpen;
	}
	if (GetCallLight(floor = GetNearestFloor(), true) || GetCallLight(floor = GetNearestFloor(), false))
	{
		SetCallLight(floor = GetNearestFloor(), true, false);
		SetCallLight(floor = GetNearestFloor(), false, false);
		SetDoor;
		*state = DoorOpen;                                                 //电梯门外按钮作用下进入DoorOpen状态
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
		*state = Idle;                                                    //门内关门按钮指令进入Idle状态
	}
}

void StateMovingUp(int *state)
{
	int  floor;
	GetFloor();
	floor = GoingUpToFloor();
	if (fabs(GetFloor() - floor)<Lib_FloorTolerance)                       //检测电梯是否到达目标楼层。
	{
		SetMotorPower(0); SetDoor(floor, true);
		*state = DoorOpen;
		SetPanelFloorLight(floor, false);

	}
	if (GetOpenDoorLight() || GetCloseDoorLight())
	{
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);                                         //消费按钮。防止下一周期继续该按钮
		*state = DoorOpen;
	}
}

void StateMovingDown(int *state)
{
	int  floor; bool up;
	GetFloor();
	floor = GoingUpToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0); SetDoor();
		*state = DoorOpen;
		SetPanelFloorLight(floor,false);                                 //消费当前门内楼层按钮
	}
	if (GetOpenDoorLight() || GetCloseDoorLight())
	{
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);
		*state = DoorOpen;
	}
}

}

void StateDoorOpen(int *state)
{
	if (GetCloseDoorLight())
	{
		SetDoor(floor,false);
		SetCloseDoorLight(false);
		*state = DoorOpen;                                              //由不受外部干扰的开门状态
	}
	if (IsDoorOpen())
	{
		SetDoor(floor,true);
		*state = DoorOpen;
	}
	if (GetOpenDoorLight())
	{
		SetOpenDoorLight(false);
		*state = DoorOpen;
	}
}

}

void StateDoorClosing(int *state)
{
	int Floor; Floor = GetNearestFloor();
	if (GetOpenDoorLight() || IsBeamBroken())                          //检测电梯门外是否有障碍物
	{
		SetDoor(floor,true);
		SetOpenDoorLight(false);
		*state = DoorOpen;
	}

	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
		*state = DoorOpen;
	}
	if (IsDoorClosed())
	{
		*state = Idle;
	}
}

/***********************************************
 * 状态机，每隔一定时间(如，100ms)被调用一次，采集系统的运行状态
 ***********************************************/
void main_control(int *state)
{  
    if(IsElevatorRunning())  // 仿真正在运行
    {
		switch(*state)
		{
		case Idle:
			// Idle状态，一定时间无动作，自动到一楼
			if(GetNearestFloor() !=1 ) {
				AutoTo1Floor();
			}
			StateIdle(state);
			break;
		case MovingUp:
			CancelTo1Floor(); // 其它状态，取消自动到一楼
			StateMovingUp(state);
			break;
		case MovingDown:
			CancelTo1Floor();
			StateMovingDown(state);
			break;
		case DoorOpen:
			CancelTo1Floor();
			StateDoorOpen(state);
			break;
		case DoorClosing:
			CancelTo1Floor();
			StateDoorClosing(state);
			break;
		default:
			printf("没有这种状态!!!\n");  
		}
    }
}
