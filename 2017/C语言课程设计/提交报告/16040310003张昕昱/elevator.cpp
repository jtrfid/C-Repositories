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
	if (floor > 0 && !up)
	{
		SetMotorPower(-1);//Transition，下降
		printf("from Idle to MovingDown.\n");
		*state = MovingDown;
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);//Transition，关灯
		return;
	}
	if (floor > 0 && up)
	{
		SetMotorPower(1);//Transition，上升
		printf("from Idle to MovingUp.\n");
		*state = MovingUp;

	}
	if (GetOpenDoorLight())//Event
	{
		SetDoor(GetNearestFloor(), true);//当前楼或最近楼开门
		SetOpenDoorLight(false);//关灯
		printf("from Idle to DoorOpen .\n");
		*state = DoorOpen;
	}
	if (GetCallLight(GetNearestFloor(), true))//Event
	{
		SetDoor(GetNearestFloor(), true);//当前楼或最近楼开门
		SetCallLight(GetNearestFloor(), up, false);//当前楼或最近楼上升按钮灯灭
		printf("from Idle to DoorOpen .\n");
		*state = DoorOpen;
	}
	
}

void StateMovingUp(int *state)
{
	int floor;
	floor = GoingUpToFloor();//Event，检测去的楼层
	if (fabs(GetFloor() - floor)<Lib_FloorTolerance)// 动态监测，电梯正在上升时，检测将要到达的目标楼层
	{
		SetMotorPower(0);//Transition，停止
		SetDoor(GetNearestFloor(), true);//当前楼或最近楼开门
		*state = DoorOpen;
		SetCallLight(GetNearestFloor(), true, false);//当前楼或最近楼上升按钮灯灭
		SetPanelFloorLight(GetNearestFloor(), false);//Transition，当前楼或最近楼关灯
		SetCallLight(GetNearestFloor(), false, false); //当前楼或最近楼下降按钮灯灭
		if (floor == 3) { SetCallLight(GetNearestFloor(), true, false); };//Event
		SetPanelFloorLight(GetNearestFloor(), false);//Transition，当前楼或最近楼关灯

	}
	if (GetOpenDoorLight()) { SetOpenDoorLight(false); };
	if (GetCloseDoorLight()) { SetCloseDoorLight(false); };//无动作，消费按键行为
}

void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)//动态监测，电梯正在下降时，检测将要到达的目标楼层
	{
		SetMotorPower(0);//Transition，停止
		SetDoor(GetNearestFloor(), true);//当前楼或最近楼开门
		*state = DoorOpen;
		SetCallLight(GetNearestFloor(), false, false);//当前楼或最近楼下降按钮灯灭
		SetCallLight(GetNearestFloor(), true, false);//当前楼或最近楼上升按钮灯灭
		if (floor == 1) { SetCallLight(GetNearestFloor(), false, false); };//Event
		SetPanelFloorLight(GetNearestFloor(), false);//Transition，当前楼或最近楼关灯

		if (GetOpenDoorLight()) { SetOpenDoorLight(false); };
		if (GetCloseDoorLight()) { SetCloseDoorLight(false); };//无动作，消费按键行为
	}
}

void StateDoorOpen(int *state)
{
	GetNearestFloor();//获取当前层
	if (IsDoorOpen(GetNearestFloor()))//Event
	{
		SetDoor(GetNearestFloor(), false);//当前楼或最近楼关门
		*state = DoorClosing;

	}
	if (GetCloseDoorLight())//Event
	{
		SetDoor(GetNearestFloor(), false);//当前楼或最近楼关门
		SetCloseDoorLight(false);//关灯
		*state = DoorClosing;

	}
	
	if (GetOpenDoorLight()) { SetOpenDoorLight(false); };
}

void StateDoorClosing(int *state)
{

	if (GetOpenDoorLight())//Event
	{
		SetDoor(GetNearestFloor(), true);//当前楼或最近楼开门
		SetOpenDoorLight(false);//关灯
		*state = DoorOpen;
	}
	if (GetCloseDoorLight()) { SetCloseDoorLight(false); };
	if (IsBeamBroken())//，红外探测到遮挡物体
	{
		SetDoor(GetNearestFloor(), true);//当前楼或最近楼开门
		*state = DoorOpen;
	}
	if (IsDoorClosed(GetNearestFloor())) { *state = Idle; };
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
