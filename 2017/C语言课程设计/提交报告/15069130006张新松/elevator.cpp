#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ElevatorLib.h"

/**********************************************
* Idle状态，电梯停止在某楼层，门是关闭的，处于静止状态，等待相关事件的发生，从而转换到下一个状态。
**********************************************/
void StateIdle(int *state)
{

	int floor;
	bool up;
	floor = IdleWhatFloorToGoTo(&up);
	if (floor > 0 && up)                                            /* 转换状态，movingup*/
	{
		SetMotorPower(1);
		*state = MovingUp;
	}
    if (floor > 0 && !up)                                        /*转换状态，movingdown*/
	{
		SetMotorPower(-1);
		*state = MovingDown;
	}
	if (GetOpenDoorLight())                                       /*由门内开门按钮转换成dooropen状态*/
	{
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;
		SetOpenDoorLight(false);
	}
	if (GetCallLight(GetNearestFloor(), true))                                     /*门外呼叫按钮转换成dooropen状态*/
	{
		SetDoor(GetNearestFloor(), true);
		SetCallLight(GetNearestFloor(), true, false);
		*state = DoorOpen;
	}
	if (GetCallLight(GetNearestFloor(), false))
	{
		SetDoor(GetNearestFloor(), true);
		SetCallLight(GetNearestFloor(), false, false);
		*state = DoorOpen;
		
	}
	
	if (GetCloseDoorLight())                                  /*门内关闭按钮转换成doorclose状态*/
	{
		SetCloseDoorLight(false);
	}
}

void StateMovingUp(int *state)
{
	int floor;
	floor = GoingUpToFloor();

	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)                           /*检测是否到达目标楼层*/
	{
		if (GetNearestFloor() == Lib_FloorNum)                                            /*检测是否到达顶层*/
		{
			SetMotorPower(0);
			SetDoor(floor, true);
			SetCallLight(floor, false, false);
			SetPanelFloorLight(floor, false);
			*state = DoorOpen;

		}
		else
		{
			SetMotorPower(0);
			SetDoor(GetNearestFloor(), true);
			SetPanelFloorLight(GetNearestFloor(), false);
			SetCallLight(GetNearestFloor(), true, false);
			SetCallLight(GetNearestFloor(), false, false);
			*state = DoorOpen;
			GetNearestFloor();  /*获得所在层数*/
		}
	}
	

	if (GetOpenDoorLight())                                               /*消费开关门按钮*/
	{
		SetOpenDoorLight(false);
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
	}
}

void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();

	if (fabs(GetFloor() - floor)< Lib_FloorTolerance)                            /*检测是否快到目的楼层*/
	{
		if (floor == 1)                                                            /*检测是否到达1层 */
		{
			SetMotorPower(0);
			SetDoor(floor, true);
			SetCallLight(floor, true, false);
			SetPanelFloorLight(floor, false);
			*state = DoorOpen;
		}
		else
		{
			SetMotorPower(0);
			SetDoor(GetNearestFloor(), true);
			SetPanelFloorLight(GetNearestFloor(), false);
			SetCallLight(GetNearestFloor(), false, false);
			SetCallLight(GetNearestFloor(), true, false);
			*state = DoorOpen;
			GetNearestFloor();
		}
	}
	
	if (GetOpenDoorLight())                                               /*消费开关门按钮*/
	{
		SetOpenDoorLight(false);
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
	}

}
void StateDoorOpen(int *state)
{
	int floor;
	floor = GetNearestFloor();
	if (GetCloseDoorLight())                                         /*正在开门，按下关门灯，转而关门*/
	{
		SetDoor(floor, false);
		SetCloseDoorLight(false);
		*state = DoorClosing;
	}
	if (GetOpenDoorLight())                                           /*消费开门按钮*/
	{
		SetOpenDoorLight(false);
	}
	
	if (IsDoorOpen(floor))                                            /*开门结束后自动进行关门*/
	{
		SetDoor(floor, false);
		*state = DoorClosing;
	}

}
void StateDoorClosing(int *state)
{
	int floor;
	floor = GetNearestFloor();
	if (GetOpenDoorLight())                                     /*正在关门，按下开门按钮，转为开门状态*/
	{
		SetDoor(GetNearestFloor(), true);
		SetOpenDoorLight(false);
		*state = DoorOpen;
	}
	if (GetCloseDoorLight())                                      /*消费关门按钮*/
	{
		SetCloseDoorLight(false);
	}
	if (IsBeamBroken())                                         /*检查是否有障碍物，进而转换为开门状态*/
	{
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;
	}
	if (IsDoorClosed(floor))                                    /*关门结束后切换为闲置状态*/
	{
		*state = Idle;
	}
}


	

/***********************************************
* 状态机，每隔一定时间(如，100ms)被调用一次，采集系统的运行状态
***********************************************/
void main_control(int *state)
{
	if (IsElevatorRunning())  // 仿真正在运行
	{
		switch (*state)
		{
		case Idle:
			// Idle状态，一定时间无动作，自动到一楼
			if (GetNearestFloor() != 1) {
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
