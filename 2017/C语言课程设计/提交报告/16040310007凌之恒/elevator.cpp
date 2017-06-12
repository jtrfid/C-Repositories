#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ElevatorLib.h"

/**********************************************
* Idle状态，电梯停止在某楼层，门是关闭的，处于静止状态，等待相关事件的发生，从而转换到下一个状态。
**********************************************/
void StateIdle(int *state)//空闲状态函数
{
	int floor;
	bool up;
	floor = IdleWhatFloorToGoTo(&up); // Event检测将要到的目标楼层
	if (floor > 0 && up)//电梯从静止状态上升
	{
		SetMotorPower(1);
		*state = MovingUp;
		printf("from Idle to MovingUp.\n");
	}
	else if (floor>0 && !up)//无操作，下降
	{
		SetMotorPower(-1);
		*state = MovingDown;
		printf("from Idle to MovingDown.\n");
	}
	else if (GetOpenDoorLight())//消费开门按钮
	{
		SetDoor(GetNearestFloor(), true);
		SetOpenDoorLight(false);
		*state = DoorOpen;
	}
	else if (GetCallLight(GetNearestFloor(), true))//消费门外up/down按钮
	{
		SetDoor(GetNearestFloor(), true);
		SetCallLight(GetNearestFloor(), true, false);
		*state = DoorOpen;
	}
	else if (GetCallLight(GetNearestFloor(), false))//关闭按钮灯，防止下一周期重复处理此按钮行为。
	{
		SetDoor(GetNearestFloor(), true);
		SetCallLight(GetNearestFloor(), false, false);
		*state = DoorOpen;
	}
	else if (GetCloseDoorLight())//因此仅读取关门灯，并关闭关门灯
	{
		SetCloseDoorLight(false);
		return;
	}
}


void StateMovingUp(int *state)//上升状态函数
{
	int floor = GoingUpToFloor();
	bool up;
	up = GetCallLight(floor, true);
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)// 到达目标楼层,开门
	{
		SetMotorPower(0);
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;
		SetCallLight(GetNearestFloor(), true, false);
		if (floor = Lib_FloorNum)//判断是否为最高层
		{
			SetCallLight(GetNearestFloor(), false, false);
		}
		SetPanelFloorLight(GetNearestFloor(), false);

	}

	if (GetOpenDoorLight() || GetCloseDoorLight())//运动状态，开关门失效
	{
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);

	}


}

void StateMovingDown(int *state)//下降状态函数
{
	int floor = GoingDownToFloor();
	bool up;
	up = GetCallLight(floor, true);
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)//判断是否到达目标楼层
	{
		SetMotorPower(0);
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;
		SetCallLight(GetNearestFloor(), false, false);
		if (floor = 1)//消费门外up按钮
		{
			SetCallLight(GetNearestFloor(), true, false);
		}
		SetPanelFloorLight(GetNearestFloor(), false);
	}
	if (GetOpenDoorLight() || GetCloseDoorLight())//运动状态，开关门失效
	{
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);

	}
}

void StateDoorOpen(int *state)//开门状态函数
{
	int floor = GetNearestFloor();
	bool up;
	if (GetCloseDoorLight())//开门中按关门键，进行关门操作
	{
		SetDoor(GetNearestFloor(), false);
		SetCloseDoorLight(false);
		*state = DoorClosing;
	}
	if (IsDoorOpen(floor))//开门结束后，自动关门
	{
		SetDoor(GetNearestFloor(), false);
		*state = DoorClosing;
	}
	if (GetOpenDoorLight())//开门中，按开门后无动作
	{
		SetOpenDoorLight(false);
	}
}

void StateDoorClosing(int *state)//关门状态函数
{
	int floor = GetNearestFloor();
	bool up;
	if (GetOpenDoorLight())//关门中按开门键，进行开门操作
	{
		SetDoor(GetNearestFloor(), true);
		SetOpenDoorLight(false);
		*state = DoorOpen;
	}
	if (GetCloseDoorLight())//关门，按了关门灯
	{
		SetCloseDoorLight(false);
	}
	if (IsBeamBroken())//判断是否红外线阻挡
	{
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;
	}
	if (IsDoorClosed(floor))//关门结束后，电机静止，等待下一步指令
	{
		*state = Idle;
	}

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
