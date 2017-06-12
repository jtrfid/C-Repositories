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
		SetMotorPower(1);//变迁
		*state = MovingUp;
		printf("Idle到MovingUp.\n");
	}
	else if (floor > 0 && !up)//事件
	{
		SetMotorPower(-1);//变迁
		*state = MovingDown;
		printf("Idle到MovingDown.\n");
	}
	else if (GetOpenDoorLight())//事件
	{
		SetOpenDoorLight(false);//消费
		SetDoor(GetNearestFloor(), true);//变迁
		*state = DoorOpen;
		printf("Idle到DoorOpen.\n");
	}
	else if (GetCallLight(GetNearestFloor() ,false))//事件
	{
		SetCallLight(GetNearestFloor(), true, false);//消费
		SetDoor(GetNearestFloor(), true);//变迁
		*state = DoorOpen;
		printf("Idle到DoorOpen.\n");
	}
	 if (GetCloseDoorLight())//事件
	{
		SetCloseDoorLight(false); //消费（不变）
		*state = Idle;
		return;
	}


	
}

void StateMovingUp(int *state)
{
	int floor;
	floor = GoingDownToFloor();//事件
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)//事件
	{
		SetMotorPower(0);//变迁
		SetDoor(GetNearestFloor(), true);//变迁
		*state = DoorOpen;
		SetCallLight(GetNearestFloor(), true, false);//消费
		SetCallLight(Lib_FloorNum, false, false);//消费
		SetPanelFloorLight(GetNearestFloor(), false);//消费
		*state = DoorOpen;
	}
	else  GetOpenDoorLight();//检测
		GetCloseDoorLight();//检测
		SetOpenDoorLight(false);//消费
		SetCloseDoorLight(false);//消费



}

void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)//事件
	{
		SetMotorPower(0);//变迁
		SetDoor(GetNearestFloor(), true);//变迁
		*state = DoorOpen;
		SetCallLight(GetNearestFloor(), false, false);//消费
		SetCallLight(Lib_FloorNum, true, false);//消费
		SetPanelFloorLight(GetNearestFloor(), false);//消费
		*state = DoorOpen;
	}
	else  GetOpenDoorLight();//检测
	GetCloseDoorLight();//检测
	SetOpenDoorLight(false);//消费
	SetCloseDoorLight(false);//消费
}

void StateDoorOpen(int *state)
{

	GetNearestFloor();
	if (GetCloseDoorLight())//事件
	{
		SetDoor(GetNearestFloor(), false);//变迁
		SetCloseDoorLight(false);//消费
		*state = DoorClosing;

	}
	if (IsDoorOpen(GetNearestFloor()))//事件
	{
		SetDoor(GetNearestFloor(), false);//变迁
		*state = DoorClosing;

	}
	if (GetOpenDoorLight())//事件
	{ 
		SetOpenDoorLight(false);//消费
	}

}


void StateDoorClosing(int *state)
{
	if (GetOpenDoorLight())//事件
	{
		SetDoor(GetNearestFloor(), true);//变迁
		SetOpenDoorLight(false);//消费
		*state = DoorOpen;
	}
	if (GetCloseDoorLight())//事件
	{ 
		SetCloseDoorLight(false); //变迁
	}
	if (IsBeamBroken())//事件
	{
		SetDoor(GetNearestFloor(), true);//变迁
		*state = DoorOpen;
	}
	if (IsDoorClosed(GetNearestFloor()))
	{
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
