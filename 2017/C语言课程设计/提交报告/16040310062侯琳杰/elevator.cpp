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
	if (floor > 0 && up){ SetMotorPower(1);
	*state = MovingUp;
	}
	else if (floor > 0 && !up){ SetMotorPower(-1);
	*state = MovingDown;
	}
	if (GetOpenDoorLight()){ SetDoor(floor,true);
	SetOpenDoorLight(false);
	*state = DoorOpen;
	}
	if (GetCloseDoorLight()){ SetCloseDoorLight(false);
	return;
	}
}

void StateMovingUp(int *state)
{
	int floor; bool up, s;
	floor = GoingUpToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance){ SetMotorPower(0);
	SetDoor(floor,true);
	*state = DoorOpen;
	if (fabs(GetFloor() - 3) < Lib_FloorTolerance) 
	SetCallLight(floor, false, false);
    SetCallLight(floor,true,false);
	SetPanelFloorLight(floor,false);
	}
	else {
		GetOpenDoorLight(); SetOpenDoorLight(false);
		GetCloseDoorLight(); SetCloseDoorLight(false);
	}
}

void StateMovingDown(int *state)
{
	int floor; bool up;
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance){
		SetMotorPower(0);
		SetDoor(floor, true);
		*state = DoorOpen;
		if (fabs(GetFloor() - 1) < Lib_FloorTolerance)
		SetCallLight(floor, true, false);
		SetCallLight(floor, false, false);
		SetPanelFloorLight(floor, false);
		GetNearestFloor();
	}
	else {
		GetOpenDoorLight(); SetOpenDoorLight(false);
		GetCloseDoorLight(); SetCloseDoorLight(false);
	}
}

void StateDoorOpen(int *state)
{
	int floor;
    floor=GetNearestFloor();
	if (GetCloseDoorLight()){
		SetCloseDoorLight(false);
		*state = DoorClosing;
	}
	else if (IsDoorOpen(floor)){
		SetDoor(floor, false);
		*state = DoorClosing;
	}
	else if (GetOpenDoorLight())
		SetOpenDoorLight(false);
}

void StateDoorClosing(int *state)
{
	int floor;
	floor = GetNearestFloor();
	if (GetCloseDoorLight()){
		SetDoor(floor, true);
		SetOpenDoorLight(false);
		*state = DoorOpen;
	}
	else if (GetCloseDoorLight()) SetCloseDoorLight(false);
	else if (IsBeamBroken()){
		SetDoor(floor, true);
		*state = DoorOpen;
	}
	else if (IsDoorClosed(floor)) *state = Idle;
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

