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
	if (floor > 0 && up) 
	{
		SetMotorPower(1);
		*state = MovingUp;
	}
	else if (floor = floor > 0 && !up)
	{
		SetMotorPower(-1);
		*state = MovingDown;
	}
	else if (GetOpenDoorLight())
	{
		SetOpenDoorLight(false);
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;
	}
	else if (GetCallLight(floor, up))
	{
		SetDoor(floor, true);
		SetCallLight(floor, up, false);
		*state = DoorOpen;
	}
	else if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
		return;
	}
	//else { printf("�������һ������ԳС��Ҳ��֪���Ĵ�Ԫ"); }
}
void StateMovingUp(int *state)
{
	int floor; bool up;
	floor = GoingDownToFloor();
	up = GetCallLight(floor, true);
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance) 
	{
		SetMotorPower(0);
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;
		SetCallLight(GetNearestFloor(), true, false);
		if (GetNearestFloor() == Lib_FloorNum)  { SetCallLight(GetNearestFloor(), false, false); }
		SetPanelFloorLight(GetNearestFloor(), false);
	}

	else if (GetOpenDoorLight() == true)  { SetOpenDoorLight(false); }
	else if (GetCloseDoorLight() == true)	{ SetCloseDoorLight(false); }
}
void StateMovingDown(int *state)
{
	int floor; bool up;
	floor = GoingUpToFloor();
	up = GetCallLight(floor, true);
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;
		SetCallLight(GetNearestFloor(), false, false);
		if (GetNearestFloor() == 1){ SetCallLight(GetNearestFloor(), true, false); }
		SetPanelFloorLight(GetNearestFloor(), false);
	}
	else if (GetOpenDoorLight() == true)  { SetOpenDoorLight(false); }
	else if (GetCloseDoorLight() == true)	{ SetCloseDoorLight(false); }

}

void StateDoorOpen(int *state)
{
	if (GetCloseDoorLight())
	{
		SetDoor(GetNearestFloor(), false);
		SetCloseDoorLight(false);
		*state = DoorClosing;
	}
	else if (IsDoorOpen(GetNearestFloor())) 
	{
		SetDoor(GetNearestFloor(), false);
		*state = DoorClosing;
	}
	else if (GetOpenDoorLight()) {
		SetOpenDoorLight(false);
	}
}

void StateDoorClosing(int *state)
{
	if (GetOpenDoorLight())
	{
		SetDoor(GetNearestFloor(), false);
		SetOpenDoorLight(false);
		*state = DoorOpen;
	}
	else if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
	}
	else if (IsBeamBroken()){
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;
	}
	else if (IsDoorClosed(GetNearestFloor()))
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
