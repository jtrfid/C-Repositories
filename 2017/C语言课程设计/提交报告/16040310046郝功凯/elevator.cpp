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
	floor = IdleWhatFloorToGoTo(&up);//Event
	if (floor>0&&up)
	{
		SetMotorPower(1);//Transition
		*state = MovingUp;
	}
	if (floor > 0 && !up)
	{
		SetMotorPower(-1);//Transition
		*state = MovingDown;
	}
	if (GetOpenDoorLight() || GetCallLight(GetNearestFloor(), true) || GetCallLight(GetNearestFloor(), false))
	{
		SetDoor(floor, true);
		SetOpenDoorLight(false);
		SetCallLight(GetNearestFloor(), true, false);
		SetCallLight(GetNearestFloor(), false, false);//Transition
		*state = DoorOpen;
	}
}

void StateMovingUp(int *state)
{
	if (fabs(GetFloor() - GoingUpToFloor())<Lib_FloorTolerance)
	{
		SetMotorPower(false);
		SetCallLight(GetNearestFloor(), true, false);
		SetCallLight(GetNearestFloor(), false, true);//Transition
		*state = DoorOpen;
	}
	if (GetOpenDoorLight()||GetCloseDoorLight())
	{
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);
	}

}

void StateMovingDown(int *state)
{
	int floor; bool up;
	if (fabs(GetFloor() - GoingDownToFloor()) < Lib_FloorTolerance)
	{
		SetMotorPower(false);
		SetDoor(GetNearestFloor(), true);
		SetCallLight(GetNearestFloor(), false, false);
		SetCallLight(GetNearestFloor(), true, true);//Transition
		*state = DoorOpen;
	}
	if (GetOpenDoorLight() || GetCloseDoorLight())
	{

		SetOpenDoorLight(false);
		SetCloseDoorLight(false);
	}
}

void StateDoorOpen(int *state)
{
	if (GetCloseDoorLight())
	{
		SetDoor(GetNearestFloor(), true);
		SetCloseDoorLight(false);
		*state = DoorClosing;
	}
	if (IsDoorOpen(true))
	{
		SetDoor(GetNearestFloor(), false);
		*state = DoorClosing;
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
		SetDoor(GetNearestFloor(), true);
		SetOpenDoorLight(false);
		*state = DoorOpen;
	}
	if (IsBeamBroken())
	{
		SetDoor(GetNearestFloor(), true);
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
