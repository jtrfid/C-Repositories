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
	}//电梯准备上升
	else if (floor > 0 && !up)
	{
		SetMotorPower(-1);
		*state = MovingDown;
	}//电梯准备下降
	if (GetOpenDoorLight())
	{
		SetDoor(floor, 1);
		SetOpenDoorLight(0);
		*state = DoorOpen;
	}// 电梯内开门
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(0);
		return;
	}//电梯关闭
}


void StateMovingUp(int *state)
{
	int floor; bool up;
	floor = GoingUpToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(floor, 1);
		*state = DoorOpen;
		if (fabs(GetFloor() - 3) < Lib_FloorTolerance)
			SetCallLight(floor, 0, 0);
		SetCallLight(floor, 1, 0);
		SetPanelFloorLight(floor, 0);
	}//开门
	else {
		GetOpenDoorLight(); SetOpenDoorLight(0);
		GetCloseDoorLight(); SetCloseDoorLight(0);

	}//安全设置，无动作
}

void StateMovingDown(int *state)
{
	int floor; bool up;
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(floor,1);
		*state = DoorOpen;
		if (fabs(GetFloor() - 1) < Lib_FloorTolerance)
			SetCallLight(floor, 1, 0);
		SetCallLight(floor, 0, 0);
		SetPanelFloorLight(floor, 0);
		GetNearestFloor();
	}//下降后开门
	else {
		GetOpenDoorLight(); SetOpenDoorLight(0);
		GetCloseDoorLight(); SetCloseDoorLight(0);
	}//关灯


}

void StateDoorOpen(int *state)
{
	int floor; bool up;
	floor=GetNearestFloor();
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(0);
		SetDoor(floor, 0);
		*state = DoorClosing;
	}//正在开门，按关门，关门
	if (IsDoorOpen(floor))
	{
		SetDoor(floor, 0);
		*state = DoorClosing;
	}//开门完，直接关门
	if (GetOpenDoorLight())
	{
		SetOpenDoorLight(0);
	}//开门时，按开门，无动作

}

void StateDoorClosing(int *state)
{
	int floor; bool up;
	floor=GetNearestFloor();
	if (GetOpenDoorLight())
	{
		SetOpenDoorLight(0);
		SetDoor(floor, 1);
		*state = DoorOpen;
	}//正在关门，按开门，开门
	else if (GetCloseDoorLight())
	{
		SetCloseDoorLight(0);
	}//正在关门，按关门，无动作
	else if (IsBeamBroken())
	{
		SetDoor(floor,1);
		*state = DoorOpen;
	}//门口有东西挡住，自动上升
	else if (IsDoorClosed(floor))
	{
		*state = Idle;
	}//关门后待机


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
