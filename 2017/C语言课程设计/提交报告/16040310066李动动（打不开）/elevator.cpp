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
	if (GetOpenDoorLight())
	{
		SetDoor(GetNearestFloor(), true);
		SetOpenDoorLight(false);
		*state = DoorOpen;
	}/*检测门内开关按钮，开门，消费门内开门按钮*/
	if (GetCloseDoorLight())
	{
		SetDoor(GetNearestFloor(), true);
		SetCloseDoorLight(false);
		*state = DoorClose;
		return;
	}/*检测门内开关按钮，关门，消费门内关门按钮*/
	if (floor > 0 && up)
	{
		SetMotorPower(1);
		*state = MovingUp;
	}/*检测将要上升，进入上升状态*/

	if (floor > 0 && !up)
	{
		SetMotorPower(-1);
		*state = MovingDown;
	}/*检测将要下降，进入下降状态*/

	if (GetCallLight(GetNearestFloor(), true) && up)
	{
		SetDoor(GetNearestFloor(), true);
		SetCallLight(GetNearestFloor(),true,false);
		*state = DoorOpen;
	}/*检测呼叫按钮，开门，消费开门按钮，进入开门状态*/

}

void StateMovingUp(int *state)
{
	int floor;
	floor = GoingUpToFloor();
		if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
		{
			SetMotorPower(0);
			SetDoor(floor, true);
			*state = DoorOpen;
			SetCallLight(floor, true, false);
			SetPanelFloorLight(floor,false);
		}/*到达目标楼层，停止，开门，消费呼叫按钮和门内当前楼层按钮*/
		if (GetOpenDoorLight() || GetCloseDoorLight())
		{
			SetOpenDoorLight(false);
			SetCloseDoorLight(false);
		}/*运动状态，开关门失效*/
		if (GetNearestFloor()==3)
		{
			SetCallLight(floor, down, false);
		}/*到了最高层，消费门外down失效*/
}

void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(floor, true);
		*state = DoorOpen;
		SetCallLight(floor,false,false);
		SetPanelFloorLight(floor, false);
	}/*到达目标楼层，停止，开门，消费门外呼叫按钮和门内当前楼层按钮*/
	if (GetOpenDoorLight() || GetCloseDoorLight())
	{
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);
	}/*运动状态，开关门失效*/
	if (GetNearestFloor() == 1)
	{
		SetCallLight(floor, up, false);
	}/*到了最底层，消费门外up按钮*/
}

void StateDoorOpen(int *state)
{
	int floor;
	floor = GetNearestFloor();
	if (GetCloseDoorLight())
	{
		SetDoor(floor,false);
		SetCloseDoorLight(false);
		*state = DoorClosing;
	}/*检测关门灯，正在开门，按了关门灯，转而关门，消费关门按钮，进入关门状态*/
	if (GetOpenDoorLight())
	{
		SetDoor(floor, false);
		SetOpenDoorLight(false);
		*state = DoorOpen;
	}/*检测门外开门按钮，正在开门，按了开门灯，无动作,消费开门按钮,进入开门状态*/
	if (IsDoorOpen(floor))
	{
		SetDoor(floor,false);
		*state = DoorClosing;
	}/*开门结束后，自动进入关门状态*/
}

void StateDoorClosing(int *state)
{
	int floor;
	floor = GetNearestFloor();
	if (GetOpenDoorLight())
	{
		SetDoor(floor, false);
		SetOpenDoorLight(false);
		*state = DoorOpen;
	}/*检测门外开门按钮，正在关门，按了开门灯，转而开门，消费关门按钮*/

	if (GetCloseDoorLight())
	{
		SetDoor(floor, false);
		SetCloseDoorLight(false);
		*state = DoorClose;
	}/*检测门外关门按钮，按了关门灯，关门，消费关门按钮*/

	if (IsBeamBroken())
	{
		SetDoor(floor,true);
		*state = DoorOpen;
	}/*红外探测到遮挡 ，转而开门*/
	if (IsDoorClosed(floor))
	{
		*state = Idle;
	}/*关门结束后，进入Idle状态*/
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
			if(GetNearestFloor() !=1 )
			{
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
