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
	floor = IdleWhatFloorToGoTo(&up);//静态检测将要到达的目标楼层
	if (floor>0 && up)//Event
	{
		SetMotorPower(1);//Transition
		*state = MovingUp;//进入上升状态
		printf("Transition:from Idle to MovingUp.\n");
	}
	else if (floor>0 && !up)//Event
	{
		SetMotorPower(-1);//Transition
		*state = MovingDown;//进入下降状态
		printf("Transition:from Idle to MovingDown.\n");
	}
	else if (GetOpenDoorLight())//门内开门按钮被按
	{
		SetDoor(GetNearestFloor(), true);//在当前层开门
		SetOpenDoorLight(false);//消费掉开门按钮
		*state = DoorOpen;//进入开门状态
		printf("Transition:from Idle to DoorOpen.\n");
	}
	else if (GetCallLight(floor, up))//某层门外呼叫被按下
	{
		SetDoor(floor, true);//到达目标楼层，并开门
		SetCallLight(GetNearestFloor(), up, false);//在当前楼层消费掉门外呼叫按钮
		*state = DoorOpen;//进入开门状态
		printf("Transition: from Idle to DoorOpen.\n");
	}
	else if (GetCloseDoorLight())//门内关门按钮被按下
	{
		SetCloseDoorLight(false);//消费掉关门按钮，防止下一周期重复处理
		return;
	}
}
/*MovingUp状态函数*/
void StateMovingUp(int *state)
{
	int floor;
	floor = GoingUpToFloor();//动态检测将要到达的楼层
	if (fabs(GetFloor() - floor)<Lib_FloorTolerance)//判断是否到达目标楼层
	{
		SetMotorPower(0);//电梯停止工作
		SetDoor(floor, true);//在所到楼层开门
		SetPanelFloorLight(floor, false);//消费门内楼层按钮为当前层的按钮
		*state = DoorOpen;// 进入开门状态
		if (floor == 3)//如果到达最高楼层，就消费掉门外的down按钮
			SetCallLight(floor, false, false);
		else
			SetCallLight(floor, true, false);//消费门外up按钮
		printf("Transition: from MovingUp to DoorOpen.\n");
	}
	else if (GetOpenDoorLight() || GetCloseDoorLight())//门内开或关门按钮被按下
	{
		SetOpenDoorLight(false);//无动作，消费开关门按钮
		SetCloseDoorLight(false);
	}
}
/*MovingDown状态函数*/
void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();//动态检测将要到达的楼层
	if (fabs(GetFloor() - floor)<Lib_FloorTolerance)//判断是否到达目标楼层
	{
		SetMotorPower(0);//电梯停止工作
		SetDoor(floor, true);//在所到楼层开门
		SetPanelFloorLight(floor, false);//消费门内楼层按钮为当前层的按钮
		*state = DoorOpen;//进入开门状态
		if (floor == 1)//如果到达了一层
			SetCallLight(floor, true, false);//消费门外up按钮
		else
			SetCallLight(floor, false, false);//消费门外down按钮
		printf("Transition: from MovingDown to DoorOpen.\n");

	}
	else if (GetOpenDoorLight() || GetCloseDoorLight())//门内开或关门按钮被按下
	{
		SetOpenDoorLight(false);//无动作，消费开关门按钮
		SetCloseDoorLight(false);
	}
}
/*DoorOpen状态函数*/
void StateDoorOpen(int *state)
{
	if (GetCloseDoorLight())//门内关门按钮被按
	{
		SetDoor(GetNearestFloor(), false);//在当前楼层关门
		SetCloseDoorLight(false);//消费门内关门按钮
		*state = DoorClosing;//进入关门状态
		printf("Transition:from DoorOpen to DoorClosing.\n");
	}
	else if (IsDoorOpen(GetNearestFloor()))//开门结束后
	{
		SetDoor(GetNearestFloor(), false);//自动进行关门
		*state = DoorClosing;//进入关门状态
		printf("Transition:from DoorOpen to DoorClosing.\n");
	}
	else if (GetOpenDoorLight())//门内开门按钮被按
	{
		SetOpenDoorLight(false);//消费开门按钮
	}
}
/*DoorClosing状态函数*/
void StateDoorClosing(int *state)
{
	if (GetOpenDoorLight())//门内开门按钮被按
	{
		SetDoor(GetNearestFloor(), true);//在当前楼层开门
		SetOpenDoorLight(false);//消费门内开门按钮
		*state = DoorOpen;//进入开门状态
		printf("Transition:from DoorClosing to DoorOpen.\n");
	}
	else if (GetCloseDoorLight())//门内关门按钮被按
	{
		SetCloseDoorLight(false);//消费关门按钮
	}
	else if (IsBeamBroken())//红外检测到有障碍物
	{
		SetDoor(GetNearestFloor(), true);//在当前楼层开门
		*state = DoorOpen;//进入开门状态
		printf("Transition:from DoorClosing to DoorOpen.\n");
	}
	else if (IsDoorClosed(GetNearestFloor()))//关门结束后
	{
		*state = Idle;//进入待机状态
		printf("Transition:from DoorClosing to Idle.\n");
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