#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ElevatorLib.h"

/**********************************************
 * Idle״̬������ֹͣ��ĳ¥�㣬���ǹرյģ����ھ�ֹ״̬���ȴ�����¼��ķ������Ӷ�ת������һ��״̬��
 **********************************************/
void StateIdle(int *state)
{
	int floor; bool up;
	if (GetCallLight(floor,1)||GetPanelFloorLight(floor))
	{
		floor = IdleWhatFloorToGoTo(&up);
		if (floor > 0 && up)
			{
				SetMotorPower(1);
				*state = MovingUp;
				printf("transition:from Idle to MovingUp");
			}
		if (floor > 0 && !up)
			{
				SetMotorPower(-1);
				*state = MovingDown;
				printf("transition:from Idle to MovingDown");
			}
	}
	if (GetOpenDoorLight() || GetCallLight(floor,1))
	{
		SetDoor(floor,1);
		SetOpenDoorLight(false);
		SetCallLight(floor, 1, false);
		*state = DoorOpen;
		printf("transition:from Idle to DoorOpen");
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
		return; 
	}
}

void StateMovingUp(int*state)
{
	if (GoingUpToFloor())
	{
		int floor;
		floor = GoingUpToFloor();
		if (fabs(GetFloor()- floor) < Lib_FloorTolerance)
		{
			SetMotorPower(0);
			SetDoor(floor,1);
			*state = DoorOpen;
			SetCallLight(floor,1,false);
			if (floor=Lib_FloorNum)
				SetCallLight(floor,1,false);
			SetPanelFloorLight(floor,false);
			GetNearestFloor();
		}
	}
	if (GetOpenDoorLight() || GetCloseDoorLight())
	{
		SetOpenDoorLight(false);
		GetCloseDoorLight();
	}
	
}

void StateMovingDown(int *state)
{
	if (GoingDownToFloor())
	{
		int floor;
		floor = GoingDownToFloor();
		if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
		{
			SetMotorPower(0);
			SetDoor(floor,1);
			*state = DoorOpen;
			SetCallLight(floor,1,false);
		}
		if (floor = Lib_FloorTolerance)
		{
			SetCallLight(floor, 1, false);
			SetPanelFloorLight(floor, false);
			GetNearestFloor();
		}
	}
	if (GetOpenDoorLight() || GetCloseDoorLight())
	{
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);
	}
}

void StateDoorOpen(int *state)
{
	int floor;
	GetNearestFloor();
	if (GetCloseDoorLight()||IsDoorOpen(floor))
	{
		SetDoor(floor,false);
		SetCloseDoorLight(false);
		*state = DoorClosing;
	}
	if (GetOpenDoorLight())
	{
		SetOpenDoorLight(false);
	}
}

void StateDoorClosing(int *state)
{
	int floor;
	if (GetOpenDoorLight()||IsBeamBroken())
	{
		SetDoor(floor,1);
		SetOpenDoorLight(false);
		*state = DoorOpen;
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
	}
}

/***********************************************
 * ״̬����ÿ��һ��ʱ��(�磬100ms)������һ�Σ��ɼ�ϵͳ������״̬
 ***********************************************/
void main_control(int *state)
{  
    if(IsElevatorRunning())  // ������������
    {
		switch(*state)
		{
		case Idle:
			// Idle״̬��һ��ʱ���޶������Զ���һ¥
			if(GetNearestFloor() !=1 ) {
				AutoTo1Floor();
			}
			StateIdle(state);
			break;
		case MovingUp:
			CancelTo1Floor(); // ����״̬��ȡ���Զ���һ¥
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
			printf("û������״̬!!!\n");  
		}
    }
}
