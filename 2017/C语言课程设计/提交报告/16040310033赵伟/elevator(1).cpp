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
	int floor;
	bool up;
	bool s;
	floor = IdleWhatFloorToGoTo(&up); 
	if (floor > 0 && up)  
	{
		SetMotorPower(1);  //检查门内楼层按钮，上升
		*state = MovingUp;
		printf("Transition: from Idle to Moving up.\n");
	}
	else if (floor > 0 && !up)  
	{
		SetMotorPower(-1); //同上，下降
		*state = MovingDown;
		printf("Transition: from Idle to Moving down.\n");
	}
	else if (GetOpenDoorLight())  
	{
		SetDoor(floor, true);
		SetOpenDoorLight(false);//检查门内开门按钮  
		*state = DoorOpen;
		printf("Transition: from Idle to Door open.\n");
	}
	else if (GetCallLight(1, true)) 
	{
		SetDoor(1, true);
		SetCallLight(1, true, false); //检查门外呼叫按钮
		*state = DoorOpen;
		printf("Transition: from Idle to Door open.\n");
	}
	else if (GetCallLight(GetNearestFloor(), true)) 
	{
		SetDoor(floor, true);
		SetCallLight(floor, true, false); 
		*state = DoorOpen;
		printf("Transition: from Idle to Door open.\n");
	}
	else if (GetCallLight(GetNearestFloor(), false)) 
	{
		SetDoor(floor, true);
		SetCallLight(floor, false, false);
		*state = DoorOpen;
		printf("Transition: from Idle to Door open.\n");
	}
	else if (GetCloseDoorLight()) 
	{
		SetCloseDoorLight(false); 
		return;
	}
}

void StateMovingUp(int *state)
{
#define Lib_FloorNum  3
#define Lib_FloorTolerance 0.01
	int floor;
	floor = GoingUpToFloor();//动态监测
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetCallLight(floor, false, false);//消费按钮
		SetPanelFloorLight(floor, false);//消费按钮
		SetDoor(floor, true);
		if (GetNearestFloor() == Lib_FloorNum) 
		{
			SetCallLight(floor, false, false);//改为消费向下按钮
		}
		*state = DoorOpen;
		printf("Transition: from MovingUp to Door open.\n");

	}
	if (GetOpenDoorLight() || GetCloseDoorLight())
	{
		SetOpenDoorLight(false);
		SetCloseDoorLight(false)
		return;
	}
}

void StateMovingDown(int *state)
{
#define Lib_FloorTolerance 0.01
	int floor;
	floor = GoingDownToFloor();
	int GetNearestFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)  
		SetMotorPower(0);
		SetDoor(floor, true); 
		*state = DoorOpen;
		printf("Transition: from MovingDown to Door open.\n");
		if (floor > 1)
			SetCallLight(floor, false, false);
		else   SetCallLight(floor, true, false);//改为消费向上按钮
		SetPanelFloorLight(floor, false);
	}
	else if (GetOpenDoorLight() || GetCloseDoorLight()) 
	{
		SetOpenDoorLight(false);  
		SetCloseDoorLight(false);
		return;
	}
}

void StateDoorOpen(int *state)
{
	int floor;
	if (GetCloseDoorLight())  
	{
		SetDoor(GetNearestFloor(), false);
		SetCloseDoorLight(false); 
		*state = DoorClosing;
		printf("Transition: from Door open to Door closing.\n");
	}
	else if (IsDoorOpen(GetNearestFloor()))  
	{
		SetDoor(GetNearestFloor(), false);  
		*state = DoorClosing;
		printf("Transition: from Door open to Door closing.\n");
	}
	else if (GetOpenDoorLight())  
	{
		SetOpenDoorLight(false);  
		*state = DoorClosing;
		printf("Transition: from Door open to Door closing.\n");
	}
}

void StateDoorClosing(int *state)
{
	int floor;
	if (GetOpenDoorLight())  
	{
		SetDoor(floor, true);
		SetOpenDoorLight(false);  
		*state = DoorOpen;
		printf("Transition: from Door closing to Door open.\n");
	}
	else if (GetCloseDoorLight())  
	{
		SetCloseDoorLight(false);  
		return;
	}
	else if (IsBeamBroken())  
	{
		SetDoor(floor, true); 
		*state = DoorOpen;
		printf("Transition: from Door closing to Door open.\n");
	}
	else if (IsDoorClosed(GetNearestFloor()))
		*state = Idle;
	printf("Transition: from Door closing to Idle.\n");
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
