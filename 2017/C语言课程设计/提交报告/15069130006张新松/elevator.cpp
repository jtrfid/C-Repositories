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
	floor = IdleWhatFloorToGoTo(&up);
	if (floor > 0 && up)                                            /* ת��״̬��movingup*/
	{
		SetMotorPower(1);
		*state = MovingUp;
	}
    if (floor > 0 && !up)                                        /*ת��״̬��movingdown*/
	{
		SetMotorPower(-1);
		*state = MovingDown;
	}
	if (GetOpenDoorLight())                                       /*�����ڿ��Ű�ťת����dooropen״̬*/
	{
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;
		SetOpenDoorLight(false);
	}
	if (GetCallLight(GetNearestFloor(), true))                                     /*������а�ťת����dooropen״̬*/
	{
		SetDoor(GetNearestFloor(), true);
		SetCallLight(GetNearestFloor(), true, false);
		*state = DoorOpen;
	}
	if (GetCallLight(GetNearestFloor(), false))
	{
		SetDoor(GetNearestFloor(), true);
		SetCallLight(GetNearestFloor(), false, false);
		*state = DoorOpen;
		
	}
	
	if (GetCloseDoorLight())                                  /*���ڹرհ�ťת����doorclose״̬*/
	{
		SetCloseDoorLight(false);
	}
}

void StateMovingUp(int *state)
{
	int floor;
	floor = GoingUpToFloor();

	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)                           /*����Ƿ񵽴�Ŀ��¥��*/
	{
		if (GetNearestFloor() == Lib_FloorNum)                                            /*����Ƿ񵽴ﶥ��*/
		{
			SetMotorPower(0);
			SetDoor(floor, true);
			SetCallLight(floor, false, false);
			SetPanelFloorLight(floor, false);
			*state = DoorOpen;

		}
		else
		{
			SetMotorPower(0);
			SetDoor(GetNearestFloor(), true);
			SetPanelFloorLight(GetNearestFloor(), false);
			SetCallLight(GetNearestFloor(), true, false);
			SetCallLight(GetNearestFloor(), false, false);
			*state = DoorOpen;
			GetNearestFloor();  /*������ڲ���*/
		}
	}
	

	if (GetOpenDoorLight())                                               /*���ѿ����Ű�ť*/
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

	if (fabs(GetFloor() - floor)< Lib_FloorTolerance)                            /*����Ƿ�쵽Ŀ��¥��*/
	{
		if (floor == 1)                                                            /*����Ƿ񵽴�1�� */
		{
			SetMotorPower(0);
			SetDoor(floor, true);
			SetCallLight(floor, true, false);
			SetPanelFloorLight(floor, false);
			*state = DoorOpen;
		}
		else
		{
			SetMotorPower(0);
			SetDoor(GetNearestFloor(), true);
			SetPanelFloorLight(GetNearestFloor(), false);
			SetCallLight(GetNearestFloor(), false, false);
			SetCallLight(GetNearestFloor(), true, false);
			*state = DoorOpen;
			GetNearestFloor();
		}
	}
	
	if (GetOpenDoorLight())                                               /*���ѿ����Ű�ť*/
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
	int floor;
	floor = GetNearestFloor();
	if (GetCloseDoorLight())                                         /*���ڿ��ţ����¹��ŵƣ�ת������*/
	{
		SetDoor(floor, false);
		SetCloseDoorLight(false);
		*state = DoorClosing;
	}
	if (GetOpenDoorLight())                                           /*���ѿ��Ű�ť*/
	{
		SetOpenDoorLight(false);
	}
	
	if (IsDoorOpen(floor))                                            /*���Ž������Զ����й���*/
	{
		SetDoor(floor, false);
		*state = DoorClosing;
	}

}
void StateDoorClosing(int *state)
{
	int floor;
	floor = GetNearestFloor();
	if (GetOpenDoorLight())                                     /*���ڹ��ţ����¿��Ű�ť��תΪ����״̬*/
	{
		SetDoor(GetNearestFloor(), true);
		SetOpenDoorLight(false);
		*state = DoorOpen;
	}
	if (GetCloseDoorLight())                                      /*���ѹ��Ű�ť*/
	{
		SetCloseDoorLight(false);
	}
	if (IsBeamBroken())                                         /*����Ƿ����ϰ������ת��Ϊ����״̬*/
	{
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;
	}
	if (IsDoorClosed(floor))                                    /*���Ž������л�Ϊ����״̬*/
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
