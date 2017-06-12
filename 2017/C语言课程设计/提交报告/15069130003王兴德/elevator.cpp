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
	floor = IdleWhatFloorToGoTo(&up);
	if (floor > 0 && up){
		SetMotorPower(1);
		*state = MovingUp;
		printf("����\n");
	}
	if (floor > 0 && !up){
		SetMotorPower(-1);
		*state = MovingDown;
		printf("�½�\n");
	}
	if (floor = -1)
	{
		if (GetOpenDoorLight())
		{
			SetDoor(GetNearestFloor(), true);
			SetOpenDoorLight(false);
			*state = DoorOpen;
		}
		if (GetCallLight(GetNearestFloor(), up))
		{
			SetCallLight(GetNearestFloor(),up,false);
			*state = DoorOpen;
		}
		if (GetCallLight(GetNearestFloor(), !up))
		{
			SetCallLight(GetNearestFloor(), !up,false);
			*state = DoorOpen;
		}
		if (GetCloseDoorLight())
		{
			SetCloseDoorLight(false);
			*state = DoorClosing;
			return;
		}
	}
}

void StateMovingUp(int *state)
{
	int floor = GoingUpToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(GetNearestFloor(), true);
		SetPanelFloorLight(GetNearestFloor(), false);
		*state = DoorOpen;
		if (GetCallLight(GetNearestFloor(), true))
			SetCallLight(GetNearestFloor(), true, false);
	}
	if (GetCloseDoorLight())
		SetCloseDoorLight(false);
	if (GetOpenDoorLight())
		SetOpenDoorLight(false);
}

void StateMovingDown(int *state)
{
	int floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(GetNearestFloor(), true);
		SetPanelFloorLight(GetNearestFloor(), false);
		*state = DoorOpen;
		if (GetCallLight(GetNearestFloor(), false))
			SetCallLight(GetNearestFloor(), false, false);
	}
	if (GetCloseDoorLight())
		SetCloseDoorLight(false);
	if (GetOpenDoorLight())
		SetOpenDoorLight(false);
}

void StateDoorOpen(int *state)
{
	bool Open;
	if (GetCloseDoorLight())
	{
		SetDoor(GetNearestFloor(), false);
		SetCloseDoorLight(false);
		*state = DoorClosing;
	}
	if (IsDoorOpen(GetNearestFloor()))
	{
		SetDoor(GetNearestFloor(), false);
		*state = DoorClosing;
	}
	if (GetOpenDoorLight())
	{
		SetOpenDoorLight(true);
		*state = DoorOpen;
	}
}

void StateDoorClosing(int *state)
{
	bool Open;
	if (GetOpenDoorLight())
	{
		SetDoor(GetNearestFloor(), true);
		SetOpenDoorLight(false);
		*state = DoorOpen;
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
	}
	if (IsBeamBroken())
	{
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;
	}
	if (IsDoorClosed(GetNearestFloor()))
	{
		*state = Idle;
	}
}
/***********************************************
* ����ѭ���� ״̬����ÿ��һ��ʱ��(�磬100ms)������һ�Σ��ɼ�ϵͳ������״̬
***********************************************/
void main_control(int *state)
{
	if (IsElevatorRunning())  // ������������
	{
		switch (*state)
		{
		case Idle: 
			if (GetNearestFloor() != 1) 
			{
				AutoTo1Floor();// Idle״̬��һ��ʱ���޶������Զ���һ¥
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