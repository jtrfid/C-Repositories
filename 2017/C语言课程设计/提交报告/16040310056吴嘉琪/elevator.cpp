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
	floor = IdleWhatFloorToGoTo(&up);//Event
	if (floor > 0 && up)
	{
		floor = IdleWhatFloorToGoTo(&up);
		SetMotorPower(1);//Transition
			*state = MovingUp;
			printf("transition:from idel to movingup.\n");

       }
	if (floor>0 && !up)//Event
	{
		floor = IdleWhatFloorToGoTo(&up);
		SetMotorPower(-1);//Transition
		*state = MovingDown;
		printf("transition:from idel to movingdow.\n");
     }
	if (GetOpenDoorLight() || GetCallLight(GetNearestFloor(), true) || GetCallLight(GetNearestFloor(), false))//Event
	{
		SetDoor(floor, true);
		SetOpenDoorLight(false);
		SetCallLight(GetNearestFloor(), true, false);
		SetCallLight(GetNearestFloor(), false, false);//Transition
		*state = DoorOpen;
		printf("transition:idel to  DoorOpen.\n");

	      }
	if (GetNearestFloor())
	{
		SetCloseDoorLight(false);
		return;
	}
}

void StateMovingUp(int *state)
{
	if (fabs(GetFloor() - GoingUpToFloor()) < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;
		printf("Transition: from MovingUp to DoorOpen.\n");
		SetCallLight(GetNearestFloor(), true, false);
		SetCallLight(Lib_FloorNum, false, false);
		SetPanelFloorLight(GetNearestFloor(), false);

	}
	if (GetOpenDoorLight() || GetCloseDoorLight())
		SetOpenDoorLight(false);
	SetCloseDoorLight(false);
}

void StateMovingDown(int *state)
{
	if (fabs(GetFloor() - GoingDownToFloor()) < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;
		printf("Transition: from MovingDown to DoorOpen.\n");
		SetCallLight(GetNearestFloor(), false, false);
		SetCallLight(1, true, false);
		SetPanelFloorLight(GetNearestFloor(), false);
	}
	if (GetOpenDoorLight() || GetCloseDoorLight())
		SetOpenDoorLight(false);
	SetCloseDoorLight(false);

}

void StateDoorOpen(int *state)
{
	if (GetCloseDoorLight())
	{
		SetDoor(GetNearestFloor(),true);
		SetCloseDoorLight(false); 
		*state = DoorClosing;
		printf("Transition: from DoorOpen to DoorCloseing.\n");
	}
	if (IsDoorOpen(true))
	{SetDoor(GetNearestFloor(), false);
	*state = DoorClosing;
	printf("Transition: from DoorOpen to DoorCloseing.\n");
	}
	if (GetOpenDoorLight())
	{
		SetOpenDoorLight(false);
	}


}

void StateDoorClosing(int *state)
{
	if (GetOpenDoorLight())
	{
		SetDoor(GetNearestFloor(),true);
		SetOpenDoorLight(false);
		*state = DoorOpen;
		printf("Transition: from Closing to DoorOpen.\n");

	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
	}
	if (IsBeamBroken())
	{
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;
		printf("Transition: from DoorOpen to DoorCloseing.\n");
	}
	if (IsDoorClosed(true))
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
