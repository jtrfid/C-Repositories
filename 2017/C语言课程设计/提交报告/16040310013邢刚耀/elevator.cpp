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
	if (floor>0&&up)
	{
		SetMotorPower(1);
		*state = MovingUp;
		printf("transition:from Idle to Movingup.\n");
	}
	else if (floor>0&&!up)
	{
		SetMotorPower(-1);
		*state = MovingDown;
		printf("transition:from Idle to Movingdown.\n");
	}
	else if (GetOpenDoorLight())
	{
		SetDoor(floor,true);
		SetOpenDoorLight(false);
		*state = DoorOpen;
		printf("transition:from Idle to DoorOpen.\n");
	}
	else if (GetCallLight(floor, true))
	{
		SetDoor(floor, true);
		SetCallLight(floor, true, false);
		*state = DoorOpen;
		printf("transition:from Idle to DoorOpen.\n");
	}
	else if (GetCallLight(floor, false))
	{
		SetDoor(floor, true);
		SetCallLight(floor, false, false);
		*state = DoorOpen;
		printf("transition:from Idle to DoorOpen.\n");
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
		return;
	}
}

void StateMovingUp(int *state)
{
	int floor; bool up;
	{
		floor = GoingUpToFloor();
		if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
		{
			SetMotorPower(0);
			SetDoor(floor, true);
			*state = DoorOpen;
			if (GetCallLight(floor, true))
			{
                SetCallLight(floor, true, false);
			}
			else if (GetCallLight(floor, false))
			{
                SetCallLight(floor, false, false);
			}
			if (floor == Lib_FloorNum)
			{
				SetCallLight(floor, false, false);
			}
			printf("transition:from movingup to DoorOpen.\n");
		}
		if (floor == Lib_FloorNum)
			SetCallLight(floor, false, false);
		if (GetOpenDoorLight() ||GetCloseDoorLight())
		{
			SetOpenDoorLight(false);
			SetCloseDoorLight(false);
		}
	}
}

void StateMovingDown(int *state)
{
	int floor; bool up;
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(floor, true);
		*state = DoorOpen;
		if (GetCallLight(floor, true))
		{
            SetCallLight(floor, true, false);
		}
		else if (GetCallLight(floor, false))
		{
            SetCallLight(floor, false, false);
		}
		if (floor == 1)
		{
			SetCallLight(1, true, false);
			SetPanelFloorLight(1, false);
		}
	}
	if (GetOpenDoorLight() ||GetCloseDoorLight())
	{
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);
	}
}

void StateDoorOpen(int *state)
{
	int floor; bool up;
	floor = GetNearestFloor();
	if (GetCloseDoorLight())
	{
		SetDoor(floor, false);
		SetCloseDoorLight(false);
		*state = DoorClosing;
		return;
		printf("transition:from dooropen to doorclosing.\n");
	}
	else if (IsDoorOpen(floor))
	{
		SetDoor(floor, false);
		*state = DoorClosing;
		return;
		printf("transition:from dooropen to Doorclosing.\n");
	}
	else if (GetOpenDoorLight())
	{
		SetOpenDoorLight(false);
		*state = DoorOpen;
	}
}

void StateDoorClosing(int *state)
{
	int floor; bool up;
	floor = GetNearestFloor();
	if (GetOpenDoorLight())
	{
		SetDoor(floor, true);
		SetOpenDoorLight(false);
		*state = DoorOpen;
		printf("transition:from doorclosing to DoorOpen.\n");
	}
	else if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
		*state = DoorOpen;
		printf("transition:from doorclosing to DoorOpen.\n");
	}
	else if (IsBeamBroken())
	{
		SetDoor(floor, true);
		*state = DoorOpen;
		printf("transition:from doorclosing to DoorOpen.\n");
	}
	else if (IsDoorClosed(floor))
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
