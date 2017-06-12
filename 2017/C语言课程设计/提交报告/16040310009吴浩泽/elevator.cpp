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
	int floor, nowfloor;
	bool up;
	nowfloor = GoingDownToFloor();
	if (nowfloor == 1)
	{
		if (GetCallLight(1, 1))
		{
			*state = DoorOpen;
			SetCallLight(1, 1, false);
			printf("Transition: from Idlestate to DoorOpen. \n");
			return;
		}
	}
	floor = IdleWhatFloorToGoTo(&up);
	if (floor > -1)
	{
		if (up&&GetPanelFloorLight(floor))
		{
			*state = MovingUp;
			printf("Transition: from Idlestate to MovingUp.\n");
			return;
		}
		{
			*state = MovingDown;
			printf("Transition: from Idlestate to MovingDown.\n");
			return; }
		if (GetCallLight(floor, up) | GetCallLight(floor, !up))
		{
			if (up == true)
			{
				*state = MovingUp;
				printf("Transition: from Idlestate to MovingUp.\n");
				return;
			}
			if (up == false)
			{
				*state = MovingDown;
				printf("Transition: from Idlestate to MovingDown.\n");
				return;
			}
		}
		{
			if (GetCloseDoorLight())
			{
				SetCloseDoorLight(false);
				*state = DoorClosing;
				printf("Transition: from Idlestate to DoorClosing.\n");
				return;
			}
			if (GetOpenDoorLight())
			{
				SetOpenDoorLight(false);
				*state = DoorOpen;
				printf("Transition: from Idlestate to DoorOpen.\n");
				return;
			}
		}
		if (nowfloor > 1)
		{
			AutoTo1Floor();
		}
	}
	void StateMovingUp(int *state)
	{
		int foorl;
		bool up = 1;
		CancelTo1Floor();
		floor = GoingUpToFloor();
		if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
		{
			if (GetCallLight(floor, up) | GetCallLight(floor, !up))
			{
				SetCallLight(floor, up, false); SetCallLight(floor, !up, false);
			}
			if (GetPanelFloorLight(floor))
				etPanelFloorLight(floor, false);
			SetMotorPower(0);
			SetDoor(floor, 1);
			*state = DoorOpen;
			printf("Transition: from Movingup to DowmOpen.\n");
			return;
		}
		else
			SetMotorPower(1);

		void StateMovingDown(int *state)
		{
			int floor;
			CancelTo1Floor();
			floot = GoingDownToFloor();
			if (GetPanelFloorLight(floor))
				SetPanelFloorLight(floor, false);
			if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
			{
				SetCallLight(floor, up, false);
				SetCallLight(floor, !up, false);
			}
			if (GetPanelFloorLight(floor))
				SetPanelFloorLight(floor, false);
			SetMotorPower(0);
			SetDoor(floor, 1);
			*state = DoorOpen;
			printf("Transition: from movingdown to DoorOpen.\n");
			return;
		}
	else
		SetMotorPower(-1);
}

	void StateDoorOpen(int *state)
	{
		int floor;
		floor = GetNearestFloor();
		CancelTo1Floor();
		if (GetCloseDoorLight())
		{
		SetDoor(floor, false);
		SetCloseDoorLight(false);
		*state = DoorClosing;
		return;
	}
	else if (IsDoorOpen(floor))
	{
		SetDoor(floor, false);
		*state = DoorClosing;
		return;
	}
	if (GetOpenDoorLight())
	{
		SetDoor(floor, true);
		SetOpenDoorLight(false);
	}
}

void StateDoorClosing(int *state)
{
	int floor;
	floor = GetNearestFloor();
	CancelTo1Floor();
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
		SetDoor(floor, false);
	}
	else if (GetOpenDoorLight())
	{
		SetDoor(floor, true);
		SetOpenDoorLight(false);
		*state = DoorOpen;
		return;
	}
	else if (IsDoorClosed(floor))
	{
		*state = Idle;
		return;
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
