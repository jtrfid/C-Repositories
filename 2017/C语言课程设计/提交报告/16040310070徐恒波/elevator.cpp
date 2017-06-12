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
	floor = IdleWhatFloorToGoTo(&up);//event
	if (floor > 0 && up)
	{
		SetMotorPower(1);
		*state = MovingUp;
		printf("Transition: from Idle to MovingUp.\n");
	} //Transition
	if (floor > 0 && !up)
	{
		SetMotorPower(-1);
		*state = MovingDown;
		printf("Transition: from Idle to MovingDown.\n");
	}  //Transition
	floor = GetNearestFloor();
	if (GetOpenDoorLight())//event
	{
		SetDoor(floor, true);
		SetOpenDoorLight(false);
		return;
	}
	if (GetCallLight(floor, true))//event
	{
		SetDoor(floor, true);
		SetCallLight(floor, up, false);
		return;
	}
   if (GetCloseDoorLight())//event
	{
		SetCloseDoorLight(false);
		return;
	}

}
void StateMovingUp(int *state)
{
	int floor;
	bool up;
	floor = GoingUpToFloor()
		if (fabs(GetFloor() - floor) <= Lib_FloorTolerance);//event
	{
		if (GetCallLight(floor,true, true))//event
		{
			SetMotorPower(0);
			SetCallLight(floor,true, false);
			GetNearestFloor();;//获取当前楼层
			SetDoor(floor, true);
			*state = DoorOpen;
			printf("Transition: from Idle to DoorOpen.\n");//Transition
			return;
		}
		if (GetCallLight(floor, false, true))//event
		{
			SetMotorPower(0);
			SetCallLight(floor, false, false);
			GetNearestFloor();;//获取当前楼层
			SetDoor(floor, true);
			*state = DoorOpen;
			printf("Transition: from Idle to DoorOpen.\n");//Transition
			return;
		}
		if (GetCloseDoorLight())//event
		{
			SetCloseDoorLight(false);
			return;
		}
		if (GetOpenDoorLight())//event
		{
			SetOpenDoorLight(true);
			return;
		}
	}


	void StateMovingDown(int *state)
	{
		int floor;
		floor = GetNearestFloor();
		floor = GoingUpToFloor();
		if (fabs(GetFloor() - floor) <= Lib_FloorTolerance)//event
		{

			if (GetCallLight(floor, false))//event
			{
				SetCallLight(floor,false, false);
				SetMotorPower(0);
				SetDoor(floor, true);
				SetPanelFloorLight(floor, false);
				*state = DoorOpen;
				printf("Transition: from MovingDown to DoorOpen.\n");//Transition
				return;
			}
			if (GetCallLight(floor, true))//event
			{
				SetCallLight(floor, false, false);
				SetMotorPower(0);
				SetDoor(floor, true);
				*state = DoorOpen;
				printf("Transition: from MovingDown to DoorOpen.\n");//Transition
				return;
			}

			if (GetCloseDoorLight())//event
			{
				SetCloseDoorLight(false);
				return;
			}
			if (GetOpenDoorLight())//event
			{
				SetOpenDoorLight(false);
				return;
			}
		}
	}
	void StateDoorOpen(int *state)
	{
		int floor;
		bool up;
		floor = GetNearestFloor();
		if (GetCloseDoorLight())//event
		{
			SetDoor(floor, false);
			SetCloseDoorLight(false);
			*state = DoorClosing;
			printf("Transition: from DoorOpen to DoorClosing.\n");//Transition
			return;
		}
		if (IsDoorOpen(floor))//event
		{
			SetDoor(floor, false);
			*state = DoorClosing;
			printf("Transition: from DoorOpen to DoorClosing.\n");//Transition
			return;
		}
		if (GetOpenDoorLight())//event
		{
			SetOpenDoorLight(false);
			return;
		}

	}
}

void StateDoorClosing(int *state)
{
	int floor;
	floor = GetNearestFloor();
	if (GetOpenDoorLight())//event
	{
		SetDoor(floor, true);
		SetOpenDoorLight(false);

		*state = DoorOpen;
		printf("Transition: from DoorClosing to DoorOpen.\n");//Transition
		return;
	}
	if (GetCloseDoorLight())//event
	{
		SetCloseDoorLight(false);
		return;
	}
	if (IsBeamBroken())//event
	{
		SetOpenDoorLight(false);
		SetDoor(floor, true);
		*state = DoorOpen;
		printf("Transition: from DoorClosing to DoorOpen.\n");//Transition
		return;
	}
	if (IsDoorClosed(floor))//event
	{
		*state = Idle;
		printf("Transition: from DoorClosing to Idle.\n");//Transition
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
