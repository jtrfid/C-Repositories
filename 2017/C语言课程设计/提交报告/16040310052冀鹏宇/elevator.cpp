#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ElevatorLib.h"

/**********************************************
 * Idle״̬������ֹͣ��ĳ¥�㣬���ǹرյģ����ھ�ֹ״̬���ȴ�����¼��ķ������Ӷ�ת������һ��״̬��
 **********************************************/
/* Idle״̬���� */
void StateIdle(int *state)
{
	int floor;
	bool up;
	floor = IdleWhatFloorToGoTo(&up);
	if (floor > 0 && up)
	{
		SetMotorPower(1);// Transition
		*state = MovingUp;
		return;
	}
	if (floor > 0 && !up)
	{
		SetMotorPower(-1);// Transition
		*state = MovingDown;
		return;
	}
	if (GetOpenDoorLight())// Event
	{
		SetDoor(GetNearestFloor(), true);// Transition
		*state = DoorOpen;
		SetOpenDoorLight(false);
		return;
	}
	if (GetCallLight(GetNearestFloor(), true))// Event
	{
		SetDoor(GetNearestFloor(), true);// Transition
		*state = DoorOpen;
		SetCallLight(GetNearestFloor(), true, false);
		return;
	}
	if (GetCallLight(GetNearestFloor(), false))// Event
	{
		SetDoor(GetNearestFloor(), true);// Transition
		*state = DoorOpen;
		SetCallLight(GetNearestFloor(), false, false);
		return;
	}
	if (GetCloseDoorLight())// Event
	{
		SetCloseDoorLight(false);
		return;
	}
}

/* MovingUp״̬���� */
void StateMovingUp(int *state)
{
	int floor;
	floor = GoingUpToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)// Event �Ƿ񵽴�ָ��¥��
	{
		SetMotorPower(0);
		SetDoor(floor, true);// Transition
		*state = DoorOpen;
		SetPanelFloorLight(floor, false);
		if (GetCallLight(GetNearestFloor(), true))// Event
		{
			SetCallLight(floor, true, false);
			return;
		}
		if (GetCallLight(GetNearestFloor(), false))// Event
		{
			SetCallLight(floor, false, false);
			return;
		}
	}
	if (GetOpenDoorLight())// Event
	{
		SetOpenDoorLight(false);
		return;
	}
	if (GetCloseDoorLight())// Event
	{
		SetCloseDoorLight(false);
		return;
	}
}

/* MovingDown״̬���� */
void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)// Event
	{
		SetMotorPower(0);
		SetDoor(floor, true);// Transition
		*state = DoorOpen;
		SetPanelFloorLight(floor, false);
		if (GetCallLight(GetNearestFloor(), false))// Event
		{
			SetCallLight(floor, false, false);
			return;
		}
		if (GetCallLight(GetNearestFloor(), false))// Event
		{
			SetCallLight(floor, true, false);
			return;
		}
	}
	if (GetOpenDoorLight())// Event
	{
		SetOpenDoorLight(false);
		return;
	}
	if (GetCloseDoorLight())// Event
	{
		SetCloseDoorLight(false);
		return;
	}
}

/* DoorOpen״̬���� */
void StateDoorOpen(int *state)
{
	int floor;
	floor = GetNearestFloor();
	if (GetCloseDoorLight())// Event
	{
		SetDoor(floor, false);// Transition
		*state = DoorClosing;
		SetCloseDoorLight(false);
		return;
	}
	if (IsDoorOpen(floor))// Event
	{
		SetDoor(floor, false);// Transition
		*state = DoorClosing;
		return;
	}
	if (GetOpenDoorLight())// Event
	{
		SetOpenDoorLight(false);
		return;
	}
}

/* DoorClosing״̬���� */
void StateDoorClosing(int *state)
{
	int floor;
	floor = GetNearestFloor();
	if (GetOpenDoorLight())// Event
	{
		SetDoor(floor, true);// Transition
		*state = DoorOpen;
		SetOpenDoorLight(false);
		return;
	}
	if (GetCloseDoorLight())// Event
	{
		SetCloseDoorLight(false);
		return;
	}
	if (IsBeamBroken())// Event �������Ƿ�̽�⵽�ڵ���
	{
		SetDoor(floor, true);// Transition
		*state = DoorOpen;
		return;
	}
	if (IsDoorClosed(floor))// Event
	{
		*state = Idle;
		return;
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
