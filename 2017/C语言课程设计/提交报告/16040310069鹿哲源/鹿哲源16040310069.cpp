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
	if (floor > 0 && up) {
		SetMotorPower(1);
		*state = MovingUp;
	}
	if (floor > 0 && !up){
		SetMotorPower(-1);
		*state = MovingDown;
	}
	if (GetCallLight(GetNearestFloor(), 1))
	{
		SetCallLight(GetNearestFloor(), 1, false);
		SetDoor(GetNearestFloor(),true);
		*state = DoorOpen;
	}

	if (GetCallLight(GetNearestFloor(), 0))
	{
		SetCallLight(GetNearestFloor(), 0, false);
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;
	}
	if (GetOpenDoorLight()){
		SetOpenDoorLight(false);
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;
	}
	if (GetCloseDoorLight()){
		SetCloseDoorLight(false);
		*state = DoorClosing;
	}

}
void StateMovingUp(int *state)
{
	    int floor; bool up;
		floor = GoingUpToFloor();
		if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
		{
			SetMotorPower(0);
			SetDoor(GetNearestFloor(), true);
			SetMotorPower(0);
			SetCallLight(GetNearestFloor(), false, false);
			SetCallLight(GetNearestFloor(), true, false);
			SetPanelFloorLight(GetNearestFloor(), false);
			*state = DoorOpen;
		}
		else {
			if (GetOpenDoorLight() == true)  { SetOpenDoorLight(false); }
			if (GetCloseDoorLight() == true)	{ SetCloseDoorLight(false); }
		}
}
void StateMovingDown(int *state)
{
	int floor; bool up;
		floor = GoingDownToFloor();
		if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
		{
			SetMotorPower(0);
			SetCallLight(GetNearestFloor(), true, false);
			SetCallLight(GetNearestFloor(), false, false);
			SetDoor(GetNearestFloor(), true);
			SetPanelFloorLight(GetNearestFloor(), false);
			*state = DoorOpen;
		}
		else {
			if (GetOpenDoorLight() == true)  { SetOpenDoorLight(false); }
			if (GetCloseDoorLight() == true)	{ SetCloseDoorLight(false); }
		}

}

void StateDoorOpen(int *state)
{

	if (GetCloseDoorLight()){
		SetDoor(GetNearestFloor(),false);
		SetCloseDoorLight(false);
		*state = DoorClosing;
	}
	if (IsDoorOpen(GetNearestFloor())) {
		SetDoor(GetNearestFloor(), false);
		*state = DoorClosing;
	}
	if (GetOpenDoorLight()) {
		SetOpenDoorLight(false);
	}
}

void StateDoorClosing(int *state)
{

	if (GetOpenDoorLight()){
		SetOpenDoorLight(false);
		*state = DoorOpen;
	}
	if (GetCloseDoorLight()){
		SetCloseDoorLight(false);
	}
    if (IsBeamBroken()){
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;
	}
	if (IsDoorClosed(GetNearestFloor())){
		SetOpenDoorLight(false);
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
