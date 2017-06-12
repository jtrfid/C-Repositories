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
	floor = IdleWhatFloorToGoTo(&up);//获取将要到达楼层
	if (floor > 0 && up)
	{
		SetMotorPower(1);
		*state = MovingUp;
		printf("Transition: from Idle to MovingUp.\n");
	}
	if (floor > 0 && !up)
	{
		SetMotorPower(-1);
		*state = MovingDown;
		printf("Transition: from Idle to MovingDown.\n");
	}
	if (GetOpenDoorLight())
	{
		SetOpenDoorLight(false);
		SetDoor(floor, true);
		*state = DoorOpen;
		printf("Transition: from Idle to DoorOpen.\n");
	}
	floor = GetNearestFloor();//使参数floor对应该楼层
	if (GetCallLight(floor, true))//如果该层楼外向上灯亮，开门，消费向上灯
	{
		SetCallLight(floor, true, false);
		SetDoor(floor, true);
		*state = DoorOpen;
		printf("Transition: from Idle to DoorOpen.\n");
	}
	if (GetCallLight(floor, false))//如果该层楼外向下灯亮，开门，消费向上下灯
	{
		SetCallLight(floor, false, false);
		SetDoor(floor, true);
		*state = DoorOpen;
		printf("Transition: from Idle to DoorOpen.\n");
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
		return;
	}
}

void StateMovingUp(int *state)
{
	int floor;
	floor = GoingUpToFloor();//获取上升时即将到达的楼层
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		if (GetCallLight(floor, true))//第一个if控制消费目标楼层向上灯按钮及门内按钮
		{
			SetCallLight(floor, true, false);
			SetPanelFloorLight(floor, false);
			SetMotorPower(0);
			SetDoor(floor, true);
			*state = DoorOpen;
			printf("Transition: from MovingUp to DoorOpen.\n");
			return;
		}
		if (GetCallLight(floor, false))//第二个if控制消费目标楼层向下灯按钮及门内按钮
			SetCallLight(floor, false, false);
		SetPanelFloorLight(floor, false);
		SetMotorPower(0);
		SetDoor(floor, true);
		*state = DoorOpen;
		printf("Transition: from MovingUp to DoorOpen.\n");
	}
	if (GetOpenDoorLight())
	{
		SetOpenDoorLight(false);
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
		return;
	}
}

void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();//获取下降时即将到达的楼层
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		if (GetCallLight(floor, true))//第一个if控制消费目标楼层向上灯按钮及门内按钮
		{
			SetCallLight(floor, true, false);
			SetPanelFloorLight(floor, false);
			SetMotorPower(0);
			SetDoor(floor, true);
			*state = DoorOpen;
			printf("Transition: from MovingDown to DoorOpen.\n");
			return;
		}
		if (GetCallLight(floor, false))//第二个if控制消费目标楼层向下灯按钮及门内按钮
			SetCallLight(floor, false, false);
		SetPanelFloorLight(floor, false);
		SetMotorPower(0);
		SetDoor(floor, true);
		*state = DoorOpen;
		printf("Transition: from MovingDown to DoorOpen.\n");
	}
	if (GetOpenDoorLight())
	{
		SetOpenDoorLight(false);
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
		return;
	}
}

void StateDoorOpen(int *state)
{
	int floor;
	floor = GetNearestFloor();
	if (GetCloseDoorLight())
	{
		SetDoor(floor, false);
		SetCloseDoorLight(false);
		*state = DoorClosing;
		printf("Transition: from DoorOpen to DoorClosing.\n");
	}
	if (IsDoorOpen(floor))//检测门是否完全打开
	{
		SetDoor(floor, false);
		*state = DoorClosing;
		printf("Transition: from DoorOpen to DoorClosing.\n");
	}
	if (GetOpenDoorLight())
	{
		SetOpenDoorLight(false);
		return;
	}
}

void StateDoorClosing(int *state)
{
	int floor;
	floor = GetNearestFloor();
	if (GetOpenDoorLight())
	{
		SetDoor(floor, true);
		SetOpenDoorLight(false);
		*state = DoorOpen;
		printf("Transition: from DoorClosing to DoorOpen.\n");
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
		return;
	}
	if (IsBeamBroken())//检测是否有障碍物
	{
		SetDoor(floor, true);
		SetOpenDoorLight(false);
		*state = DoorOpen;
		printf("Transition: from DoorClosing to DoorOpen.\n");
	}
	if (IsDoorClosed(floor))//检测门是否完关闭
	{
		*state = Idle;
		printf("Transition: from DoorClosing to Idle.\n");
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
