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
	if (GetCallLight(floor,1)||GetPanelFloorLight(floor))
	{
		floor = IdleWhatFloorToGoTo(&up);
		if (floor > 0 && up)
			{
				SetMotorPower(1);
				*state = MovingUp;
				printf("transition:from Idle to MovingUp");
			}
		if (floor > 0 && !up)
			{
				SetMotorPower(-1);
				*state = MovingDown;
				printf("transition:from Idle to MovingDown");
			}
	}
	if (GetOpenDoorLight() || GetCallLight(floor,1))
	{
		SetDoor(floor,1);
		SetOpenDoorLight(false);
		SetCallLight(floor, 1, false);
		*state = DoorOpen;
		printf("transition:from Idle to DoorOpen");
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
		return; 
	}
}

void StateMovingUp(int*state)
{
	if (GoingUpToFloor())
	{
		int floor;
		floor = GoingUpToFloor();
		if (fabs(GetFloor()- floor) < Lib_FloorTolerance)
		{
			SetMotorPower(0);
			SetDoor(floor,1);
			*state = DoorOpen;
			SetCallLight(floor,1,false);
			if (floor=Lib_FloorNum)
				SetCallLight(floor,1,false);
			SetPanelFloorLight(floor,false);
			GetNearestFloor();
		}
	}
	if (GetOpenDoorLight() || GetCloseDoorLight())
	{
		SetOpenDoorLight(false);
		GetCloseDoorLight();
	}
	
}

void StateMovingDown(int *state)
{
	if (GoingDownToFloor())
	{
		int floor;
		floor = GoingDownToFloor();
		if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
		{
			SetMotorPower(0);
			SetDoor(floor,1);
			*state = DoorOpen;
			SetCallLight(floor,1,false);
		}
		if (floor = Lib_FloorTolerance)
		{
			SetCallLight(floor, 1, false);
			SetPanelFloorLight(floor, false);
			GetNearestFloor();
		}
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
	GetNearestFloor();
	if (GetCloseDoorLight()||IsDoorOpen(floor))
	{
		SetDoor(floor,false);
		SetCloseDoorLight(false);
		*state = DoorClosing;
	}
	if (GetOpenDoorLight())
	{
		SetOpenDoorLight(false);
	}
}

void StateDoorClosing(int *state)
{
	int floor;
	if (GetOpenDoorLight()||IsBeamBroken())
	{
		SetDoor(floor,1);
		SetOpenDoorLight(false);
		*state = DoorOpen;
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
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
