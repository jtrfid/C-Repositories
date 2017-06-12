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
	floor=IdleWhatFloorToGoTo(&up);//确定目标楼层
	if (floor>0&&up)//如果要向上运行
	{
		
		SetMotorPower(1);
		*state = MovingUp;
	}
	if (floor>0&&!up)//如果要向下运行
	{ 
		SetMotorPower(-1);
		*state = MovingDown;
	}
	if (GetOpenDoorLight())//门内按了开门灯
	{
		SetOpenDoorLight(false);//消费开门灯
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;
	}
	if (GetCallLight(GetNearestFloor(), true))//如果门外按了向上
	{
		SetDoor(GetNearestFloor(), true);
		SetCallLight(GetNearestFloor(), true, false);
		*state = DoorOpen;
	}
	if (GetCallLight(GetNearestFloor(), false))//如果门外按了向下
	{
		SetDoor(GetNearestFloor(), true);
		SetCallLight(GetNearestFloor(), false, false);
		*state = DoorOpen;
	}
	if (GetCloseDoorLight())//在门关闭时消费门内关门键
	{ 
		SetCloseDoorLight(false); 
	return;
	}
}

void StateMovingUp(int *state)
{
	int floor;
	floor = GoingUpToFloor();
	SetOpenDoorLight(false);
	SetCloseDoorLight(false);
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)//检测是否到目标楼层
	{
		SetMotorPower(0);
		SetDoor(GetNearestFloor(), true);
		SetCallLight(GetNearestFloor(), true, false);
		SetCallLight(GetNearestFloor(), false, false);
		SetPanelFloorLight(GetNearestFloor(), false);
		if (GetNearestFloor() == Lib_FloorNum)//到了最高层，消费门外down按钮
		    SetCallLight(GetNearestFloor(), false, false);
		*state = DoorOpen;
	}

}

void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();
	SetOpenDoorLight(false);
	SetCloseDoorLight(false);
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(GetNearestFloor(), true);
		SetCallLight(GetNearestFloor(), false, false);
		SetCallLight(GetNearestFloor(), true, false);
		SetPanelFloorLight(GetNearestFloor(), false);
		*state = DoorOpen;
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
	if (IsDoorOpen(GetNearestFloor()))//如果当前楼层门已经打开
	{
		SetDoor(GetNearestFloor(), false);//自动进行关门
		*state = DoorClosing;
	}
	if (GetOpenDoorLight())//如果正在开门时还按了开门键
		SetOpenDoorLight(false);//消费开门键
}

void StateDoorClosing(int *state)
{
	if (GetOpenDoorLight() )//正在关门时按了开门键
	{
		SetDoor(GetNearestFloor(), true);
		SetOpenDoorLight(false);
		*state = DoorOpen;
	}
	if (GetCloseDoorLight() )//正在关门时按了关门灯
		SetCloseDoorLight(false);
	if (IsBeamBroken())//如果遇到障碍物
	{
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;
	}
	if (IsDoorClosed(GetNearestFloor()) )//如果门关好了
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
