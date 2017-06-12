#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ElevatorLib.h"

/**********************************************
 * Idle状态，电梯停止在某楼层，门是关闭的，处于静止状态，等待相关事件的发生，从而转换到下一个状态。
 **********************************************/
bool up;
void StateIdle(int *state)
{
	int floor; bool up;
	floor = IdleWhatFloorToGoTo(&up);
	if (floor > 0 && up)                   //检测楼层准备上升
	{
		SetMotorPower(1);
		*state = MovingUp;
	}
	if (floor > 0 && !up)
	{                                      //检测楼层准备下降
		SetMotorPower(-1);
		*state = MovingDown;
	}
	if((GetOpenDoorLight())
	{
		SetOpenDoorLight(false);              //消费键，检测门内开门按钮，开门
		SetDoor(GetNearestFloor(),true);
		*state = DoorOpen;
	}
	if (GetCallLight())
	{
		SetCallLight(false);                 //消费键，检测门外呼叫按钮，开门
		SetDoor();
		*state = DoorOpen;
	}
	if (GetCloseDoorLight())                  //消费键，检测门内关门按钮，不变化
	{
		SetCloseDoorLight(false);
		return;

	}
}

void StateMovingUp(int *state)
{
	int floor; bool up;
	floor = GoingUpToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)    //到达指定楼层，开门
	{
		SetMotorPower(0);
		SetDoor;
		*state = DoorOpen;
	}
	if (GetCallLight)                                        //检测门外呼叫按钮，到达后开门，消费键
	{
		SetDoor;
		SetCallLight(GetNearestFloor(),false,false);
	}
	if (GetOpenDoorLight() || GetCloseDoorLight())              //运行过程，门内开关按钮无效
	{
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);
	}
	
}

void StateMovingDown(int *state)
{
	int floor; bool up;
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)            //到达指定楼层开门
	{
		SetMotorPower(0);
		SetDoor;
		*state = DoorOpen;
	}
	if (GetCallLight)                                              //检测门外呼叫按钮，到达后关闭
	{
		SetCallLight(GetNearestFloor(),false,false);
	}
	if (floor = 1)                                                  //检测门外上升按钮，关闭
	{
		SetCallLight(1,true,false);
	}
	if (GetPanelFloorLight);                               //检测门内楼层按钮，消费键
	{
		SetPanelFloorLight(GetNearestFloor(),false); }
	if (GetOpenDoorLight() || GetCloseDoorLight)               //检测门内开关门按钮，无效
	{
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);
	}
	


}

void StateDoorOpen(int *state)
{
	if (GetCloseDoorLight)                //检测门内关门按钮，关门，消费键
	{
		SetDoor(GetNearestFloor(),false);
		SetCloseDoorLight(false);
		*state = DoorClosing;
	}
	if (GetOpenDoorLight)                     //检测门内开门按钮，开门，消费键
	{
		SetOpenDoorLight(false);
	}
	if (IsDoorOpen)                      //检测门开启，自动关闭
	{
		SetDoor(GetNearestFloor(),false);
		*state = DoorClosing;
	}
}

void StateDoorClosing(int *state)
{
	if (GetOpenDoorLight)                //检测门内开门按钮消费键，开门
	{
		SetDoor;
		SetOpenDoorLight(false);
		*state = DoorOpen;
	}
	if (GetCloseDoorLight)            //检测门内关门按钮，消费键，无效
	{
		SetCloseDoorLight(false);
	}
	if (IsBeamBroken)              //检测有无障碍物，有障碍物保持开门状态
	{
		SetDoor;
		*state = DoorOpen;
	}
	if (IsDoorClosed)          //检测门状态关闭，进入静止状态
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
