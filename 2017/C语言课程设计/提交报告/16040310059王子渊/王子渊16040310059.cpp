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
	if (floor > 0 && up)
	{
		SetMotorPower(1);
		*state = MovingUp;
		printf("from idle to moving up/n");

	}
	else if (floor>0&&!up)
	{
		SetMotorPower(-1);
		*state = MovingDown;
		printf("from idle to moving down");
	}
	else if (GetCallLight(1,1))
	{
		SetDoor(1, 1);
		SetCallLight(1, 1, 0);
		*state = DoorOpen;

	}
	else if (GetOpenDoorLight())
	{
		SetOpenDoorLight(0);
		SetDoor(GetNearestFloor(), 1);
		*state = DoorOpen;
	}
	else if (GetCallLight(floor, up))
	{
		SetCallLight(floor, up, 0);
		SetDoor(floor, 1);
		*state = DoorOpen;
		printf("from idle to DoorOpen");
	}
	else if (GetCloseDoorLight())
	{
		SetCloseDoorLight(0);
		printf("from idle to idle");
		return;
	}
}
void StateMovingUp(int *state)
{

	int floor = GoingUpToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(floor, 1);
		SetCallLight(floor, 1&&0, 0);
		SetPanelFloorLight(floor, 0);
	    *state = DoorOpen;
	  return;
	}
	if (GetOpenDoorLight() || GetCloseDoorLight())
	{
		SetOpenDoorLight(0);
		SetCloseDoorLight(0);
	}

}
void StateMovingDown(int *state)
{
	int floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(floor, 1);
		SetCallLight(floor, 1 && 0, 0);
		SetPanelFloorLight(floor, 0);
		*state = DoorOpen;
		return;
	}

	if (GetOpenDoorLight() || GetCloseDoorLight())
	{
		SetOpenDoorLight(0);
		SetCloseDoorLight(0);
	}
}
void StateDoorOpen(int *state)
{
	if (GetCloseDoorLight())
	{
		SetDoor(GetNearestFloor(), 0);
		SetCloseDoorLight(0);
		*state = DoorClosing;
     }
	    else if (IsDoorOpen(GetNearestFloor()))
	{
		SetDoor(GetNearestFloor(),0);
		*state = DoorClosing;
	}
	    else if (GetOpenDoorLight())
		SetOpenDoorLight(0);

}

void StateDoorClosing(int *state)
{
	if (GetOpenDoorLight())
	{
		SetDoor(GetNearestFloor(), 0);
		SetOpenDoorLight(0);
		*state = DoorOpen;
	}
	    else if (GetCloseDoorLight())
	{
		SetCloseDoorLight(0);
	}
	    else if (IsBeamBroken())
	{
		SetDoor(GetNearestFloor(), 1);
		*state = DoorOpen;
	}
	    else if (IsDoorClosed(GetNearestFloor()))
		*state = Idle;

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
