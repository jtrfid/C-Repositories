#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ElevatorLib.h"

/**********************************************
 三层电梯仿真程序状态函数
 学号：16040310091
 姓名：张又文
 日期：2017.5.18
 **********************************************/
/*空闲状态函数*/
void StateIdle(int *state)
{
	int floor;
	bool up;
	floor = IdleWhatFloorToGoTo(&up);/*静态检测，bool up;*/
    if (GetNearestFloor() != 1)
	{
		AutoTo1Floor();
	}/*1层以上，一定时间无动作，自动下降到1楼。*/
	if (GetPanelFloorLight(floor) && floor > GetNearestFloor())
	{
		SetMotorPower(1);
		*state = MovingUp;
	}
	else if (GetCallLight(floor, true) && floor > GetNearestFloor())
	{
		SetMotorPower(1);
		*state = MovingUp;
	}
	else if (GetCallLight(floor, false) && floor > GetNearestFloor())
	{
		SetMotorPower(1);
		*state = MovingUp;
	}
	else if (GetOpenDoorLight() || (GetCallLight(GetNearestFloor(), true) || GetCallLight(GetNearestFloor(), false)))
	{
		SetDoor(GetNearestFloor(), true);
		SetOpenDoorLight(false);
		*state = DoorOpen;
	}
	else if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);/*消费*/
	}
	else if (GetCallLight(floor, true) && floor < GetNearestFloor())
	{
		SetMotorPower(-1);
		*state = MovingDown;
	}
	else if (GetCallLight(floor, false) && floor < GetNearestFloor())
	{
		SetMotorPower(-1);
		*state = MovingDown;
	}
	else if (GetPanelFloorLight(floor)&&floor < GetNearestFloor())
	{
		SetMotorPower(-1);
		*state = MovingDown;
	}
}/*Idle状态，电梯停止在某楼层，门是关闭的，处于静止状态，等待相关事件的发生，从而转换到下一个状态。*/
void StateMovingUp(int *state)
{
	int floor;
	floor = GoingUpToFloor();/*动态检测*/
	if (GetOpenDoorLight())
	{
		SetOpenDoorLight(false);/*消费*/
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);/*消费*/
	}
	if (floor - GetFloor() < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(floor, true);
		SetCallLight(floor,true,false);
		SetCallLight(floor, false, false);
		SetPanelFloorLight(floor,false);
		*state = DoorOpen;
	}/*到达目标楼层，停止，开门     消费门外up按钮; 到了最高层Lib_FloorNum, 消费门外down按钮。消费门内楼层按钮。*/
}/*Moving up状态*/
void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();/*动态检测*/
	if (GetOpenDoorLight())
	{
		SetOpenDoorLight(false);/*消费*/
	}
	if (GetCloseDoorLight())
	{
			SetCloseDoorLight(false);/*消费*/
	}
	if (GetFloor() - floor < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(floor,true);
		SetPanelFloorLight(floor,false);
		SetCallLight(floor, true, false);
		SetCallLight(floor, false, false);
		*state = DoorOpen;
	}/*与moving up状态类似*/
}/*Moving down状态*/
void StateDoorOpen(int *state)
{
	int floor;
	floor = GetNearestFloor();
	if (GetCloseDoorLight())/*转而关门*/
	{
		SetDoor(floor,false);
		SetCloseDoorLight(false);
		*state = DoorClosing;
	}
	if (IsDoorOpen(floor))/*开门结束后，自动进入关门状态*/
	{
		SetDoor(floor, false);
		*state = DoorClosing;
	}
}/*电梯门打开*/
void StateDoorClosing(int *state)
{
	int floor;
	floor = GetNearestFloor();
	if (GetOpenDoorLight())/*转而开门*/
	{
		SetDoor(floor,true);
		*state = DoorOpen;
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
	}
	if (IsDoorClosed(floor))/*关门结束后，进入Idle状态。*/
	{
		*state = Idle;
	}
}/*正在关门*/
/***********************************************
 * 主控循环： 状态机，每隔一定时间(如，100ms)被调用一次，采集系统的运行状态
 ***********************************************/
void main_control(int *state)
{  
	if (IsElevatorRunning())
	{
		switch (*state)
		{
		case Idle:
			StateIdle(state);
			break;
		case MovingUp:
			StateMovingUp(state);
			break;
		case MovingDown:
			StateMovingDown(state);
			break;
		case DoorOpen:
			StateDoorOpen(state);
			break;
		case DoorClosing:
			StateDoorClosing(state);
			break;
		default:
			printf("没有这种状态!!!\n");
		}
	}
}