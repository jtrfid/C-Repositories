#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ElevatorLib.h"

/**********************************************
 * Idle状态，电梯停止在某楼层，门是关闭的，处于静止状态，等待相关事件的发生，从而转换到下一个状态。
 **********************************************/
/* Idle状态函数 */
void StateIdle(int *state)
{
	int floor;
	bool up;
	floor = IdleWhatFloorToGoTo(&up);
	if (floor > 0 && up)
	{
		SetMotorPower(1);// Transition
		*state = MovingUp;
		return;
	}
	if (floor > 0 && !up)
	{
		SetMotorPower(-1);// Transition
		*state = MovingDown;
		return;
	}
	if (GetOpenDoorLight())// Event
	{
		SetDoor(GetNearestFloor(), true);// Transition
		*state = DoorOpen;
		SetOpenDoorLight(false);
		return;
	}
	if (GetCallLight(GetNearestFloor(), true))// Event
	{
		SetDoor(GetNearestFloor(), true);// Transition
		*state = DoorOpen;
		SetCallLight(GetNearestFloor(), true, false);
		return;
	}
	if (GetCallLight(GetNearestFloor(), false))// Event
	{
		SetDoor(GetNearestFloor(), true);// Transition
		*state = DoorOpen;
		SetCallLight(GetNearestFloor(), false, false);
		return;
	}
	if (GetCloseDoorLight())// Event
	{
		SetCloseDoorLight(false);
		return;
	}
}

/* MovingUp状态函数 */
void StateMovingUp(int *state)
{
	int floor;
	floor = GoingUpToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)// Event 是否到达指定楼层
	{
		SetMotorPower(0);
		SetDoor(floor, true);// Transition
		*state = DoorOpen;
		SetPanelFloorLight(floor, false);
		if (GetCallLight(GetNearestFloor(), true))// Event
		{
			SetCallLight(floor, true, false);
			return;
		}
		if (GetCallLight(GetNearestFloor(), false))// Event
		{
			SetCallLight(floor, false, false);
			return;
		}
	}
	if (GetOpenDoorLight())// Event
	{
		SetOpenDoorLight(false);
		return;
	}
	if (GetCloseDoorLight())// Event
	{
		SetCloseDoorLight(false);
		return;
	}
}

/* MovingDown状态函数 */
void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)// Event
	{
		SetMotorPower(0);
		SetDoor(floor, true);// Transition
		*state = DoorOpen;
		SetPanelFloorLight(floor, false);
		if (GetCallLight(GetNearestFloor(), false))// Event
		{
			SetCallLight(floor, false, false);
			return;
		}
		if (GetCallLight(GetNearestFloor(), false))// Event
		{
			SetCallLight(floor, true, false);
			return;
		}
	}
	if (GetOpenDoorLight())// Event
	{
		SetOpenDoorLight(false);
		return;
	}
	if (GetCloseDoorLight())// Event
	{
		SetCloseDoorLight(false);
		return;
	}
}

/* DoorOpen状态函数 */
void StateDoorOpen(int *state)
{
	int floor;
	floor = GetNearestFloor();
	if (GetCloseDoorLight())// Event
	{
		SetDoor(floor, false);// Transition
		*state = DoorClosing;
		SetCloseDoorLight(false);
		return;
	}
	if (IsDoorOpen(floor))// Event
	{
		SetDoor(floor, false);// Transition
		*state = DoorClosing;
		return;
	}
	if (GetOpenDoorLight())// Event
	{
		SetOpenDoorLight(false);
		return;
	}
}

/* DoorClosing状态函数 */
void StateDoorClosing(int *state)
{
	int floor;
	floor = GetNearestFloor();
	if (GetOpenDoorLight())// Event
	{
		SetDoor(floor, true);// Transition
		*state = DoorOpen;
		SetOpenDoorLight(false);
		return;
	}
	if (GetCloseDoorLight())// Event
	{
		SetCloseDoorLight(false);
		return;
	}
	if (IsBeamBroken())// Event 检测红外是否探测到遮挡物
	{
		SetDoor(floor, true);// Transition
		*state = DoorOpen;
		return;
	}
	if (IsDoorClosed(floor))// Event
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
