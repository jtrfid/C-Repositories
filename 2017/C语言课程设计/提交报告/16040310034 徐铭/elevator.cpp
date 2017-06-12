#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ElevatorLib.h"

/**********************************************
* Idle状态，电梯停止在某楼层，门是关闭的，处于静止状态，等待相关事件的发生，从而转换到下一个状态。
**********************************************/
bool up;
void StateIdle(int *state)
{
	int floor;
	floor = IdleWhatFloorToGoTo(&up);//要到达的目标楼层
	if (floor > 0 && up) 
		SetMotorPower(1); //电梯上行
	else if (floor > 0 && !up) 
		SetMotorPower(-1);//电梯下行
	else if (GetOpenDoorLight())//开门灯状态
	{
		SetOpenDoorLight(false);
		SetDoor(floor, true);//开门
	}
	else if (GetCallLight(floor, false))
	{
		SetCallLight(floor, false, false);
		SetDoor(floor, true);//开门
	}
	else if (GetCallLight(floor, true))
	{
		SetCallLight(floor, up, false);
		SetDoor(floor, true);//开门
	}
	else if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
		return;
	}
}

void StateMovingUp(int *state)
{
	int floor;
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor)<Lib_FloorTolerance)//电梯到达
	{
		SetMotorPower(0);//停止
		SetDoor(floor, true);//开门
	    if (floor == Lib_FloorNum)//楼层为最高层
		SetCallLight(floor, false, false);
		else if (floor != Lib_FloorNum)
			SetCallLight(floor, true, false);
	}
	if (fabs(GetFloor() - floor)>Lib_FloorTolerance)
	{
		SetCloseDoorLight(false);
		SetOpenDoorLight(false);
	}
}

void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor)<Lib_FloorTolerance)//电梯处于上行状态
	{
		SetMotorPower(0);//停止
		SetDoor(floor, true);//开门
	    if (floor == 1)
		SetCallLight(floor, true, false);
		if (floor > 1) 
			SetCallLight(floor, false, false);
		SetPanelFloorLight(floor, false);
	}
	if (fabs(GetFloor() - floor)>Lib_FloorTolerance)//电梯处于下行状态
	{
		SetCloseDoorLight(false);
		SetOpenDoorLight(false);
	}
}

void StateDoorOpen(int *state)
{
	int floor;
	if (GetOpenDoorLight())//开门按钮亮起
	{
		SetDoor(floor, true);//开门
		SetOpenDoorLight(false);
	}
	else if (IsDoorOpen(floor))
		SetDoor(floor, false);//关门
	else if (GetCloseDoorLight())//关门按钮亮起
	{
		SetDoor(floor, false);//关门
		SetCloseDoorLight(false);
	}
}

void StateDoorClosing(int *state)
{
	int floor;
	if (GetOpenDoorLight())//开门按钮状态
	{
		SetOpenDoorLight(false);
		SetDoor(floor, true);//开门
	}
	else if (GetCloseDoorLight())//关门按钮状态
	{
		SetCloseDoorLight(false);
		SetDoor(floor, false);//关门
	}
	else if (IsBeamBroken())//检查是否有物体在门间
		SetDoor(floor, true);//开门
}

/***********************************************
* 状态机，每隔一定时间(如，100ms)被调用一次，采集系统的运行状态
***********************************************/
void main_control(int *state)
{
	if (IsElevatorRunning())  // 仿真正在运行
	{
		switch (*state)
		{
		case Idle:
			// Idle状态，一定时间无动作，自动到一楼
			if (GetNearestFloor() != 1) {
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

