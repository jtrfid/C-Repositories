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
	floor = IdleWhatFloorToGoTo(&up);//自行检测目标楼层数及下一步运行方向
	if (GetFloor()>1)//10s无动作自动到一楼
		AutoTo1Floor();
	if (floor>-1 && up == 1)//非当前楼层门外呼叫
	{
		SetCallLight(GetNearestFloor(), 1, 0);
		*state = MovingUp;
	}
	if (floor>-1 && up == 0)
	{
		SetCallLight(GetNearestFloor(), 0, 0);
		*state = MovingDown;
	}
	if (GetOpenDoorLight())//空闲状态下开门
	{
		SetDoor(GetFloor(), 1);
		SetOpenDoorLight(0);
		*state = DoorOpen;
	}

	if (up&&GetCallLight(GetNearestFloor(), 1))//当前楼层呼叫
	{
		SetDoor(GetNearestFloor(), 1);
		SetCallLight(GetNearestFloor(), 1, 0);
		*state = DoorOpen;
	}
	if (!up&&GetCallLight(GetNearestFloor(), 1))
	{
		SetDoor(GetNearestFloor(), 1);
		SetCallLight(GetNearestFloor(), 0, 0);
		*state = DoorOpen;
	}

	if (GetCloseDoorLight())//关门，无反应
	{
		SetCloseDoorLight(0);
		*state = Idle;
	}

}

void StateMovingUp(int *state)
{
	int floor = GoingUpToFloor();//动态监测去的楼层
	CancelTo1Floor();
	SetMotorPower(1);
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)//到目的地，开门
	{
		SetMotorPower(0);
		SetDoor(floor, 1);
		SetCallLight(floor, 1 || 0, 0);
		SetPanelFloorLight(floor, 0);
		*state = DoorOpen;
	}
	if (GetOpenDoorLight() || GetCloseDoorLight())//运行状态下开门关门都没用
	{
		SetOpenDoorLight(0);
		SetCloseDoorLight(0);
	}
	
}

void StateMovingDown(int *state)
{
	int floor = GoingDownToFloor();//动态检测要去楼层
	CancelTo1Floor();
	SetMotorPower(-1);
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)//到达目的地，开门
	{
		SetMotorPower(0);
		SetDoor(floor, 1);
		SetCallLight(floor, 1 || 0, 0);
		SetPanelFloorLight(floor, 0);
		*state = DoorOpen;
	}
	if (GetOpenDoorLight() || GetCloseDoorLight())//运行状态下开门关门都没用
	{
		SetOpenDoorLight(0);
		SetCloseDoorLight(0);
	}

}

void StateDoorOpen(int *state)
{
	int floor = GetFloor();
	CancelTo1Floor();
	if (GetCloseDoorLight())//手动关门
	{
		SetDoor(floor, 0);
		SetCloseDoorLight(0);
		*state = DoorClosing;
	}
	if (IsDoorOpen(floor))//开门过后自动关门
	{
		SetDoor(floor, 0);
		*state = DoorClosing;
	}
	if (GetOpenDoorLight())//开门状态下按开门无动作
	{
		SetOpenDoorLight(0);
	}

}

void StateDoorClosing(int *state)
{
	int floor = GetFloor();
	CancelTo1Floor();
	if (GetOpenDoorLight())//手动开门
	{
		SetDoor(floor, 1);
		SetOpenDoorLight(0);
		*state = DoorOpen;
	}
	if (GetCloseDoorLight())//关门状态下按关门无反应
	{
		SetCloseDoorLight(0);
	}
	if (IsDoorClosed(floor))//关门结束，进入空闲状态
		*state = Idle;
}

void main_control(int *state)//主控循环
{
	if (IsElevatorRunning())
	{
		switch (*state)
		{
		case Idle:
			StateIdle(state); break;
		case MovingUp:
			StateMovingUp(state); break;
		case MovingDown:
			StateMovingDown(state); break;
		case DoorOpen:
			StateDoorOpen(state); break;
		case DoorClosing:
			StateDoorClosing(state); break;
		default:
			printf("没有这种状态!!!\n");
		}
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
