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
	bool up;
	int floor;
	floor = IdleWhatFloorToGoTo(&up);//静态检测将要到达的目标楼层
	if (floor>0 && up)
	{
		SetMotorPower(1);//检验门内楼层按钮，进入上升状态
		*state = MovingUp;
		printf("Transition:from Idle to Moving up\n");
	}
	if (floor>0 && !up)
	{
		SetMotorPower(-1);//检验门内楼层按钮，进入下降状态
		*state = MovingDown;
		printf("Transition:from Idle to Moving Down\n");
	}
	if (GetOpenDoorLight())
	{
		SetDoor(GetNearestFloor(), 1);
		SetOpenDoorLight(0);//检验门内开门按钮，进入开门状态
		*state = DoorOpen;
		printf("Transition:from Idle to Door Open\n");
	}
	if (GetCallLight(GetNearestFloor(), 1))
	{
		SetDoor(GetNearestFloor(), 1);
		SetCallLight(GetNearestFloor(), 1, 0); //检验门外向上呼叫按钮，进入开门状态
		*state = DoorOpen;
		printf("Transition:from Idle to Door Open\n");
	}
	if (GetCallLight(GetNearestFloor(), 0))
	{
		SetDoor(GetNearestFloor(), 1);
		SetCallLight(GetNearestFloor(), 0, 0);//检验门外呼叫按钮，进入开门状态
		*state = DoorOpen;
		printf("Transition:from Idle to Door Open\n");
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(0);//检验门内关门按钮，无响应
		return;
		printf("Transition:from Idle to Idle\n");
	}
}

void StateMovingUp(int *state)
{
	int floor;
	floor = GoingUpToFloor();//动态检测，目标楼层
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)//到达目标楼层
	{
		SetMotorPower(0);//关闭电机
		SetDoor(floor, 1);//开门
		SetPanelFloorLight(floor, 0);//消费门内楼层按钮为当前楼层的按钮
		SetCallLight(floor, 1, 0);//消费当前楼层门外向上呼叫按钮
		if (floor == Lib_FloorNum)
		{
			SetCallLight(floor, 0, 0);//到了最高层，消费最高层门外向下呼叫按钮
		}
		*state = DoorOpen;//进入开门状态
		printf("Transition:from Moving up to Door Open\n");
	}
	if (GetOpenDoorLight() || GetCloseDoorLight())
	{
		SetOpenDoorLight(0);//运动过程中，门内开关门按钮失效
		SetCloseDoorLight(0);
		printf("Transition:from Moving Up to Moving Up\n");
	}
}

void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();//动态检测目标楼层
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)//到达目标楼层
	{
		SetMotorPower(0);//关闭电机
		SetDoor(floor, 1);//开门
		SetCallLight(floor, 0, 0);//消费当前楼层门外向下呼叫按钮
		if (floor == 1)
			SetCallLight(floor, 1, 0);//到达一层，消费一层门外向上呼叫按钮
		SetPanelFloorLight(floor, 0);//消费门内楼层按钮为当前楼层的按钮
		*state = DoorOpen;//进入开门状态
		printf("Transition:from Moving Down to Door Open\n");
	}
	if (GetOpenDoorLight() || GetCloseDoorLight())
	{
		SetOpenDoorLight(0);
		SetCloseDoorLight(0);//运动状态，门内开关门按钮失效
		printf("Transition:from Moving Down to Moving Down\n");
	}
}

void StateDoorOpen(int *state)
{
	int floor;
	floor = GetNearestFloor();//静态检测当前楼层
	if (GetCloseDoorLight())//检测关门按钮
	{
		SetDoor(floor, 0);//关闭电梯门
		SetCloseDoorLight(0);//消费关门按钮
		*state = DoorClosing;//进入关门状态
		printf("Transition:from Door Open to Door Closing\n");
	}
	if (IsDoorOpen(floor))//开门结束后
	{
		SetDoor(floor, 0);//自动进行关门
		*state = DoorClosing;//进入关门状态
		printf("Transition:from Door Open to Door Closing\n");
	}
	if (GetOpenDoorLight())//检验开门按钮
	{
		SetOpenDoorLight(0);//正在开门，开门按钮失效
		printf("Transition:from Door Open to Door Open\n");
	}
}

void StateDoorClosing(int *state)
{
	int floor;
	floor = GetNearestFloor();//静态检测当前楼层
	if (GetOpenDoorLight())//检验开门按钮
	{
		SetDoor(floor, 1);//开门
		SetOpenDoorLight(0);//消费开门按钮
		*state = DoorOpen;//进入开门状态
		printf("Transition:from Door Closing to Door Open\n");
	}
	if (GetCloseDoorLight())//检验关门按钮
	{
		SetCloseDoorLight(0);//正在关门，关门按钮失效
		printf("Transition:from Door Closing to Door Closing\n");
	}
	if (IsBeamBroken())//检测是否有障碍物
	{
		SetDoor(floor, 1);//开门
		*state = DoorOpen;//进入开门状态
		printf("Transition:from Door Closing to Door Open\n");
	}
	if (IsDoorClosed(floor))//关门结束
	{
		*state = Idle;//进入ldle状态
		printf("Transition:from Door Closing to  Idle\n");
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


