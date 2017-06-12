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
	 if (floor > 0 && !up)
	{
		SetMotorPower(-1);
		*state = MovingDown;
	}
	if (GetOpenDoorLight())
	{
		SetDoor(floor, true);
		SetOpenDoorLight(false);
		*state = DoorOpen;
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
		*state = Idle;
		return;
	}
}
void StateMovingUp(int *state)
{
	 int floor;
	 floor= GoingUpToFloor();
	 if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	 {
		 SetMotorPower(0);
		 SetDoor(floor, true);
		 SetCallLight(floor, true, false);
		 SetCallLight(floor, false, false);
		 SetPanelFloorLight(floor, false);
		 *state = DoorOpen;
	 }
	 if (GetOpenDoorLight())
	 {
		 SetOpenDoorLight(false);
	 }
	 if (GetCloseDoorLight())
	 {
		 SetCloseDoorLight(false);
	 }
}

void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(floor, true);
		SetCallLight(floor, true, false);
		SetCallLight(floor, false, false);
		GetPanelFloorLight(GetNearestFloor());
		*state = DoorOpen;
	}
	if (GetOpenDoorLight());
    {
	    SetOpenDoorLight(false); 
	}
	 if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
	}
}


void StateDoorOpen(int *state)
{
	int floor = GetFloor();
	if (GetCloseDoorLight())
	{
		SetDoor(floor, false);
		SetCloseDoorLight(false);
		*state = DoorClosing;
	}
	else if (IsDoorOpen(GetNearestFloor()))
	{
		SetDoor(GetNearestFloor(), false);
		*state = DoorClosing;
	}	
	if (GetOpenDoorLight());
	{
		SetOpenDoorLight(false);
	}
}
void StateDoorClosing(int *state)
{
	int floor = GetFloor();
	if (GetOpenDoorLight())
	{
		SetDoor(floor, 1);
		SetOpenDoorLight(0);
		*state = DoorOpen;
	}
	if (GetCloseDoorLight())//����״̬�°������޷�Ӧ
	{
		SetCloseDoorLight(0);
	}
	if (IsBeamBroken())
	{
		SetDoor(floor,true);
		*state = DoorOpen;
	}
	if (IsDoorClosed(floor))//���Ž������������״̬
		*state = Idle;
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
