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
	bool up;
	int floor;
	floor=IdleWhatFloorToGoTo(&up);//获取目的楼层
	if (GetCallLight(1, true))
	{
		SetDoor(1, true);
		SetCallLight(1, true,false);
		*state = DoorOpen;
	}
	if (floor > 0 && up)
	{
		SetMotorPower(1);
		*state = MovingUp;
		printf("from idle to moving up/n");
		return;
	}
	 else if (floor > 0 && !up)
	{
		SetMotorPower(-1);
		*state = MovingDown;
		printf("from idle to moving down");
		return;
	}
	else if (GetOpenDoorLight())
	{
		SetDoor(GetNearestFloor(), true);
		SetOpenDoorLight(false);
		*state = DoorOpen;
		printf("from idle to DoorOpen");
		return;       
	}
	else if (GetCallLight(floor, up))
	{
		SetDoor(floor, true);
		SetCallLight(GetNearestFloor(), up, false);
		*state = DoorOpen;
		printf("from idle to DoorOpen");
		return;
	}
	else if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
		printf("from idle to idle");
		return;
	}
} 
void StateMovingUp(int *state)
{
	int floor;
	bool up;
	floor=GoingUpToFloor();
	up = GetCallLight(floor, true);
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(floor, true);
		if (up==true)
		SetCallLight(floor,true,false);
		else SetCallLight(floor, false, false);
		SetPanelFloorLight(floor, false);
		*state = DoorOpen;
		return;
	}
     if (GetOpenDoorLight||GetCloseDoorLight)
	{
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);
	}
}

void StateMovingDown(int *state)
{
	int floor;
	bool up;
	floor = GoingDownToFloor();
	up = GetCallLight(floor, true);
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(floor, true);
		*state = DoorOpen;
		if (up==1)
		SetCallLight(floor, true, false);
		else
			SetCallLight(floor, false, false);
		SetPanelFloorLight(floor, false);
		return;
	}
     if (GetOpenDoorLight || GetCloseDoorLight)
	{
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);
	}
		

}

void StateDoorOpen(int *state)
{    
	if (GetCloseDoorLight())
	{
		SetDoor(GetNearestFloor(), false);
		SetCloseDoorLight(false);
		*state = DoorClosing;
	}
	else if (IsDoorOpen(GetNearestFloor()))
	{
		SetDoor(GetNearestFloor(), false);
		*state = DoorClosing;
	}
	else if (GetOpenDoorLight())
		SetOpenDoorLight(false);
}

void StateDoorClosing(int *state)
{
	if (GetOpenDoorLight())
	{
		SetDoor(GetNearestFloor(), false);
		SetOpenDoorLight(false);
		*state = DoorOpen;
	}
	else if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
	}
	else if (IsBeamBroken())
	{
		SetDoor(GetNearestFloor(), true);
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
