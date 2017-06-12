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
	if (floor > 0 && up)
	{
		SetMotorPower(1);
		*state = MovingUp;
		printf("Transition: StateIdle from  to MovingUp.\n");
	}
	if (floor > 0 && !up)
	{
		SetMotorPower(-1);
		*state = MovingDown;
		printf("Transition: StateIdle from  to MovingUp.\n");
	}
	if (GetCallLight(GetNearestFloor(),true) || GetOpenDoorLight())
	{
		SetOpenDoorLight(false);
		SetCallLight(GetNearestFloor(), true, false);
		SetCallLight(GetNearestFloor(), false, false);
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;
		printf("Transition: StateIdle from  to DoorOpen.\n");
		return;
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
		return;
	}
	if (GetCallLight(GetNearestFloor(), true) || GetCallLight(GetNearestFloor(), false))
	{
		SetDoor(GetNearestFloor(), true);
		SetCallLight(GetNearestFloor(), true, false);
		SetCallLight(GetNearestFloor(), false, false);
		*state = DoorOpen;
	}
}

void StateMovingUp(int *state)
{
	int floor;
	floor = GoingUpToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(GetNearestFloor(),true);
		*state = DoorOpen;
		printf("Transition:MovingUp from  to DoorOpen.\n");
		SetCallLight(GetNearestFloor(),true,false);
		SetCallLight(GetNearestFloor(), false, false);
		SetPanelFloorLight(GetNearestFloor(), false);
	}
	if (GetOpenDoorLight() || GetCloseDoorLight())
	{
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);
	}
}

void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;
		printf("Transition:MovingDown from  to DoorOpen.\n");
		SetCallLight(GetNearestFloor(), true, false);
		SetCallLight(GetNearestFloor(), false, false);
		SetPanelFloorLight(GetNearestFloor(), false);
	}
	if (GetOpenDoorLight() || GetCloseDoorLight())
	{
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);
	}
}

void StateDoorOpen(int *state)
{
	int floor;
	if (GetCloseDoorLight())
	{
		SetDoor(GetNearestFloor(), false);
		SetCloseDoorLight(false);
		*state = DoorClosing;
		printf("Transition: DoorOpen from  to DoorClosing.\n");
	}
	if (IsDoorOpen(GetNearestFloor()))
	{
		SetDoor(GetNearestFloor(), false);
		*state = DoorClosing;
		printf("Transition: DoorOpen from  to DoorClosing.\n");
	}
	if (GetOpenDoorLight())
	{
		SetOpenDoorLight(false);
	}
}

void StateDoorClosing(int *state)
{
	if (GetOpenDoorLight())
	{
		SetDoor(GetNearestFloor(),true);
		SetOpenDoorLight(false);
		*state = DoorOpen;
		printf("Transition: DoorClosing from  to DoorOpen.\n");
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
	}
	if (IsBeamBroken())
	{
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;
		printf("Transition: DoorClosing from  to DoorOpen.\n");
	}
	if (IsDoorClosed(GetNearestFloor()))
	{
		*state = Idle;
		printf("Transition: DoorClosing from  to Idle.\n");
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
