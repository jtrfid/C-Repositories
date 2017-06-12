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
	floor = IdleWhatFloorToGoTo(&up);//静态检测
	if (floor > 0 && up)//s1
	{
		SetMotorPower(1);
		*state = MovingUp;
		printf("Transition: from Idle to MovingUp.\n");
	}
	if (floor > 0 && !up)//事件s2
	{
		SetMotorPower(-1);
		*state = MovingDown;
		printf("Transition: from Idle to MovingDown.\n");
	}
	if (GetOpenDoorLight())//事件s3中的E1
	{
		SetDoor(GetNearestFloor(), 1);
		SetOpenDoorLight(0);
		*state = DoorOpen;
		printf("Transition: from Idle to DoorOpen.\n");
	}
	if (up&&GetCallLight(GetNearestFloor(), 1))//事件s3中的E4
	{
		SetDoor(GetNearestFloor(), 1);
		SetCallLight(GetNearestFloor(), 1, 0);
		*state = DoorOpen;
		printf("Transition: from Idle to DoorOpen.\n");
	}
	if (!up&&GetCallLight(GetNearestFloor(), 1))//事件s3中的E5
	{
		SetDoor(GetNearestFloor(), 1);
		*state = DoorOpen;
		printf("Transition: from Idle to DoorOpen.\n");
	}
	if (GetCloseDoorLight())//事件s4
	{
		SetCloseDoorLight(0);
		return;
	}

}

void StateMovingUp(int *state)
{
	int floor;
	bool up;
	floor = GoingUpToFloor();//动态检测
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)//s5  
	{
		SetMotorPower(0);//
		SetDoor(floor, 1);
		SetPanelFloorLight(GetNearestFloor(), 0);
		SetCallLight(floor, 1, 0);// 消费
		SetCallLight(floor, 0, 0);
		*state = DoorOpen;
		printf("Transition: from MovingUp to DoorOpen.\n");
	}
	if (GetNearestFloor() == 3)
		SetCallLight(floor, 1, 0);
	if (GetOpenDoorLight()) //事件D 1
		SetOpenDoorLight(0);
	if (GetCloseDoorLight())//事件D 2
		SetOpenDoorLight(0);
}
void StateMovingDown(int *state)
{
	int floor;
	bool up;
	floor = GoingDownToFloor();//动态检测
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)//s6  
	{
		SetMotorPower(0);
		SetDoor(floor, 1);
		SetPanelFloorLight(GetNearestFloor(), 0);//消费门内楼层按钮
		SetCallLight(GetNearestFloor(),1|| 0, 0);//消费门外up/down按钮
		*state = DoorOpen;
		printf("Transition: from MovingDown to DoorOpen.\n");
	}
	if (GetNearestFloor() == 1)
		SetCallLight(floor, 0, 0);
	if (GetOpenDoorLight())//事件D
		SetCloseDoorLight(0);
	if (GetCloseDoorLight())//事件D
		SetOpenDoorLight(0);
}

void StateDoorOpen(int *state)
{
	int floor;
	floor = GetNearestFloor();
	if (GetCloseDoorLight())//事件s7   1
	{
		SetDoor(GetNearestFloor(), 0);
		SetCloseDoorLight(0);//消费
		*state = DoorClosing;
		printf("Transition: from  DoorOpen to DoorClosing.\n");
	}
	if (IsDoorOpen(floor))//事件s7  2
	{
		SetDoor(floor, 0);
		*state = DoorClosing;
		printf("Transition: from  DoorOpen to DoorClosing.\n");
	}
	if (GetOpenDoorLight())//事件s7   3
	{
		SetOpenDoorLight(0);//消费
	}
}
void StateDoorClosing(int *state)
{
	int floor;
	floor = GetNearestFloor();
	if (GetOpenDoorLight())//事件s8     1
	{
		SetDoor(floor, 1);
		SetOpenDoorLight(0);
		*state = DoorOpen;
		printf("Transition: from DoorClosing to DoorOpen.\n");
	}
	if (GetCloseDoorLight())//事件s8    2
	{
		SetCloseDoorLight(0);
		return;
	}
	if (IsBeamBroken())//事件s8   3
	{
		SetDoor(floor, 1);
		*state = DoorOpen;
		printf("Transition: from DoorClosing to DoorOpen.\n");
	}
	if (IsDoorClosed(floor))
	{
		*state = Idle;
		printf("Transition: from DoorClosing to DoorOpen.\n");
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
