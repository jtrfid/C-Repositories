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
	floor = IdleWhatFloorToGoTo(&up);//Event
	if (floor>0 && up)
	{
		SetMotorPower(1);//Transition
		*state = MovingUp;
		printf("注意，电梯上升");
	}//按下有效楼层，电梯上升
	else if (floor>0 && !up)
	{
		SetMotorPower(-1);//Transition
		*state = MovingDown;
		printf("注意，电梯下降");
	}//按下有效楼层，电梯下降
	if (GetOpenDoorLight())//Event
	{
		SetDoor(GetNearestFloor(), true);//Transition
		SetOpenDoorLight(false);//消费开门按键
		printf("开门了");
		*state = DoorOpen;//按下看门按键，电梯打开
	}
	if (GetCallLight(GetNearestFloor(), true))//Event
	{
		SetDoor(GetNearestFloor(), true);//Transition
		SetOpenDoorLight(false);//消费开门按键
		*state = DoorOpen;
		printf("开门了");
	}

	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);//消除无效按键
		return;
	}
}

void StateMovingUp(int *state)
{
	int floor;
	floor = GoingUpToFloor();
	if (fabs(GetFloor() - floor)<Lib_FloorTolerance)//Event
	{
		SetMotorPower(0);
		SetDoor(GetNearestFloor(), true);
		if (GetNearestFloor()<Lib_FloorNum)
			SetCallLight(GetNearestFloor(), true, false);
		else SetCallLight(GetNearestFloor(), false, false);
		SetPanelFloorLight(GetNearestFloor(), false);//消费按键
		*state = DoorOpen;
		printf("开门了");//电梯移动至指定位置开门
	}
	if (GetOpenDoorLight())
		SetOpenDoorLight(false);
	if (GetCloseDoorLight())
		SetCloseDoorLight(false);//电梯运动时消费开门，关门的无效按键
}

void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor)<Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;
		printf("开门了");
	}
	if (GetNearestFloor() == 1)
		SetCallLight(1, true, false);//电梯在一楼时，消费门外的上升按键
	else SetCallLight(GetNearestFloor(), false, false);
	SetPanelFloorLight(GetNearestFloor(), false);//消费门内为当前楼层的按键
	if (GetOpenDoorLight())
		SetOpenDoorLight(false);
	if (GetCloseDoorLight())
		SetCloseDoorLight(false);//电梯运动时消费开门，关门的无效按键
}

void StateDoorOpen(int *state)
{
	if (GetCloseDoorLight())
	{
		SetDoor(GetNearestFloor(), false);
		SetCloseDoorLight(false);
		*state = DoorClosing;
		printf("注意，关门了");//检测到关门命令完成关门，并消费关门按键
	}
	if (IsDoorOpen(GetNearestFloor()))
	{
		SetDoor(GetNearestFloor(), false);
		*state = DoorClosing;
		printf("注意，关门了");
	}
	if (GetOpenDoorLight())
		SetOpenDoorLight(false);//消费无效的开门按键
}

void StateDoorClosing(int *state)
{
	if (GetOpenDoorLight())
	{
		SetDoor(GetNearestFloor(), true);
		SetOpenDoorLight(false);
		*state = DoorOpen;
		printf("注意，关门了");

	}

	if (GetCloseDoorLight())
		SetCloseDoorLight(false);//消费无效的关门按键
	if (IsBeamBroken())
	{
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;
		printf("注意安全");//关门时检测到有遮挡物，马上开门
	}
	if (IsDoorClosed(GetNearestFloor()))
	{
		*state = Idle;//关门完成，恢复原始状态
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
