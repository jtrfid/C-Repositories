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
	int floor;
	bool up;
	bool s;
	floor = IdleWhatFloorToGoTo(&up); 
	if (GetCallLight(1, true))
	{
		SetDoor(1, true);
		SetCallLight(1, true, false);
		*state = DoorOpen;
	}
	if (floor > 0 && up) // Event
	{
		SetMotorPower(1); // Transition
			*state = MovingUp;
			printf("Transition: from Idle to MovingUp.\n");
	}
	if (floor > 0 && !up) //Event
	{
		SetMotorPower(-1); // Transition
		*state = MovingDown;
		printf("Transition: from Idle to MovingDown.\n");
	}
	if (GetOpenDoorLight()) //Event
	{
		SetDoor(GetNearestFloor(), true);
		SetOpenDoorLight(false);//Transition
		*state = DoorOpen;
		printf("Transition: from Idle to DoorOpen.\n");
	}
	if (GetCloseDoorLight()) //Event
	{
		SetDoor(GetNearestFloor(), false);
		SetCloseDoorLight(false);//Transition
		*state = DoorClosing;
		printf("Transition: from Idle to Idle.\n");
			return;
	}
}

void StateMovingUp(int *state)
{
	int floor;
	bool up;
	bool s;
	floor = GoingUpToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance) //Event
	{
		SetMotorPower(0);// Transition
		SetDoor(GetNearestFloor(), true);
		SetCallLight(GetNearestFloor(), true&& false, false);
		*state = DoorOpen;
		SetPanelFloorLight(GetNearestFloor(), false);
		printf("Transition: from MovingUp to DoorOpen.\n");
	}
	if (GetOpenDoorLight() || GetCloseDoorLight())
	{
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);
	}

}

void StateMovingDown(int *state)
{
	int floor;
	bool up;
	bool s;
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(floor, true);
		*state = DoorOpen;
		SetCallLight(floor, true&&false, false);
		SetPanelFloorLight(floor, false);
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
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight( false);
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
