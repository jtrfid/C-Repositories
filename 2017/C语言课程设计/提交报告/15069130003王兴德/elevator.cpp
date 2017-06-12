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
	int floor; bool up;
	floor = IdleWhatFloorToGoTo(&up);
	if (floor > 0 && up){
		SetMotorPower(1);
		*state = MovingUp;
		printf("上升\n");
	}
	if (floor > 0 && !up){
		SetMotorPower(-1);
		*state = MovingDown;
		printf("下降\n");
	}
	if (floor = -1)
	{
		if (GetOpenDoorLight())
		{
			SetDoor(GetNearestFloor(), true);
			SetOpenDoorLight(false);
			*state = DoorOpen;
		}
		if (GetCallLight(GetNearestFloor(), up))
		{
			SetCallLight(GetNearestFloor(),up,false);
			*state = DoorOpen;
		}
		if (GetCallLight(GetNearestFloor(), !up))
		{
			SetCallLight(GetNearestFloor(), !up,false);
			*state = DoorOpen;
		}
		if (GetCloseDoorLight())
		{
			SetCloseDoorLight(false);
			*state = DoorClosing;
			return;
		}
	}
}

void StateMovingUp(int *state)
{
	int floor = GoingUpToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(GetNearestFloor(), true);
		SetPanelFloorLight(GetNearestFloor(), false);
		*state = DoorOpen;
		if (GetCallLight(GetNearestFloor(), true))
			SetCallLight(GetNearestFloor(), true, false);
	}
	if (GetCloseDoorLight())
		SetCloseDoorLight(false);
	if (GetOpenDoorLight())
		SetOpenDoorLight(false);
}

void StateMovingDown(int *state)
{
	int floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(GetNearestFloor(), true);
		SetPanelFloorLight(GetNearestFloor(), false);
		*state = DoorOpen;
		if (GetCallLight(GetNearestFloor(), false))
			SetCallLight(GetNearestFloor(), false, false);
	}
	if (GetCloseDoorLight())
		SetCloseDoorLight(false);
	if (GetOpenDoorLight())
		SetOpenDoorLight(false);
}

void StateDoorOpen(int *state)
{
	bool Open;
	if (GetCloseDoorLight())
	{
		SetDoor(GetNearestFloor(), false);
		SetCloseDoorLight(false);
		*state = DoorClosing;
	}
	if (IsDoorOpen(GetNearestFloor()))
	{
		SetDoor(GetNearestFloor(), false);
		*state = DoorClosing;
	}
	if (GetOpenDoorLight())
	{
		SetOpenDoorLight(true);
		*state = DoorOpen;
	}
}

void StateDoorClosing(int *state)
{
	bool Open;
	if (GetOpenDoorLight())
	{
		SetDoor(GetNearestFloor(), true);
		SetOpenDoorLight(false);
		*state = DoorOpen;
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
	}
	if (IsBeamBroken())
	{
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;
	}
	if (IsDoorClosed(GetNearestFloor()))
	{
		*state = Idle;
	}
}
/***********************************************
* 主控循环： 状态机，每隔一定时间(如，100ms)被调用一次，采集系统的运行状态
***********************************************/
void main_control(int *state)
{
	if (IsElevatorRunning())  // 仿真正在运行
	{
		switch (*state)
		{
		case Idle: 
			if (GetNearestFloor() != 1) 
			{
				AutoTo1Floor();// Idle状态，一定时间无动作，自动到一楼
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