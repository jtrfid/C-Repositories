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
	bool up; bool down;
	up = true;
	down = false;
	if ((GetCallLight || GetPanelFloorLight) == true)
	{
		int floor;
		floor = IdleWhatFloorToGoTo(&up);//Event
		if (floor>0 && up)
		{
			SetMotorPower(1);
			*state = MovingUp;
		}
		else if (floor > 0 && !up)
		{
			SetMotorPower(-1);
			*state = MovingDown;
		}
	}
	else if (GetOpenDoorLight() == true)
	{
		SetDoor(GetFloor(), true);
		SetOpenDoorLight(false);
		*state = DoorOpen;
	}
	if (GetCallLight(GetFloor(), up) || GetCallLight(GetFloor(), down))
	{
		SetDoor(GetFloor(), true);
		SetCallLight(GetFloor(), up, false);
		SetCallLight(GetFloor(), down, false);
		*state = DoorOpen;
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
		return;
	}
}

void StateMovingUp(int *state)
{
	bool up; bool down;
	up = true;
	down = false;
	int floor;
	floor = GoingUpToFloor();
	if ((GetCallLight || GetPanelFloorLight) == true)
	{
		if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
		{
			SetMotorPower(0);
			SetDoor(floor, true);
			*state = DoorOpen;
			if (floor== Lib_FloorNum)
			{
				SetCallLight(floor, down, false);
			}
			else
			{
				SetCallLight(floor, up, false);
			}
			SetPanelFloorLight(GetNearestFloor(), false);//��������¥�㰴ťΪ��ǰ¥�㰴ť
		}
	}
	if ((GetOpenDoorLight || GetCloseDoorLight) == true)
	{
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);
	}
}

void StateMovingDown(int *state)
{
	bool up; bool down;
	up = true;
	down = false;
	int floor;
	floor = GoingDownToFloor();
	if ((GetCallLight || GetPanelFloorLight) == true)
	{
		if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
		{
			SetMotorPower(0);
			SetDoor(floor, true);
			*state = DoorOpen;
			if (floor == 1)
			{
				SetCallLight(floor, up, false);
			}
			else
			{
				SetCallLight(floor, down, false);
			}
			SetPanelFloorLight(GetNearestFloor(), false);
		}
	}
	if ((GetOpenDoorLight || GetCloseDoorLight) == true)
	{
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);
	}
}

void StateDoorOpen(int *state)
{
	bool up; bool down;
	up = true;
	down = false;
	if (GetCloseDoorLight() == true)
	{
		SetDoor(GetNearestFloor(), false);
		SetCloseDoorLight(false);
		*state = DoorClosing;
	}
	if (IsDoorOpen(GetNearestFloor()) == true)
	{
		SetDoor(GetNearestFloor(), false);
		*state = DoorClosing;
	}
	if (GetOpenDoorLight() == true)
	{
		SetOpenDoorLight(false);
	}
}

void StateDoorClosing(int *state)
{
	bool up; bool down;
	up = true;
	down = false;
	if (GetOpenDoorLight() == true)
	{
		SetDoor(GetNearestFloor(), true);
		SetOpenDoorLight(false);
		*state = DoorOpen;
	}
	if (GetCloseDoorLight() == true)
	{
		SetCloseDoorLight(false);
	}
	if (IsBeamBroken() == true)
	{
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;
	}
	if (IsDoorClosed(GetNearestFloor()) == true)
	{
		*state = Idle;
	}
}

/***********************************************
* ״̬����ÿ��һ��ʱ��(�磬100ms)������һ�Σ��ɼ�ϵͳ������״̬
***********************************************/
void main_control(int *state)
{
	if (IsElevatorRunning())  // ������������
	{
		switch (*state)
		{
		case Idle:
			// Idle״̬��һ��ʱ���޶������Զ���һ¥
			if (GetNearestFloor() != 1) {
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