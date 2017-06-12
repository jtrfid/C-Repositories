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
	floor = IdleWhatFloorToGoTo(&up);//��̬���
	if (floor > 0 && !up)
	{
		SetMotorPower(-1);//Transition���½�
		printf("from Idle to MovingDown.\n");
		*state = MovingDown;
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);//Transition���ص�
		return;
	}
	if (floor > 0 && up)
	{
		SetMotorPower(1);//Transition������
		printf("from Idle to MovingUp.\n");
		*state = MovingUp;

	}
	if (GetOpenDoorLight())//Event
	{
		SetDoor(GetNearestFloor(), true);//��ǰ¥�����¥����
		SetOpenDoorLight(false);//�ص�
		printf("from Idle to DoorOpen .\n");
		*state = DoorOpen;
	}
	if (GetCallLight(GetNearestFloor(), true))//Event
	{
		SetDoor(GetNearestFloor(), true);//��ǰ¥�����¥����
		SetCallLight(GetNearestFloor(), up, false);//��ǰ¥�����¥������ť����
		printf("from Idle to DoorOpen .\n");
		*state = DoorOpen;
	}
	
}

void StateMovingUp(int *state)
{
	int floor;
	floor = GoingUpToFloor();//Event�����ȥ��¥��
	if (fabs(GetFloor() - floor)<Lib_FloorTolerance)// ��̬��⣬������������ʱ����⽫Ҫ�����Ŀ��¥��
	{
		SetMotorPower(0);//Transition��ֹͣ
		SetDoor(GetNearestFloor(), true);//��ǰ¥�����¥����
		*state = DoorOpen;
		SetCallLight(GetNearestFloor(), true, false);//��ǰ¥�����¥������ť����
		SetPanelFloorLight(GetNearestFloor(), false);//Transition����ǰ¥�����¥�ص�
		SetCallLight(GetNearestFloor(), false, false); //��ǰ¥�����¥�½���ť����
		if (floor == 3) { SetCallLight(GetNearestFloor(), true, false); };//Event
		SetPanelFloorLight(GetNearestFloor(), false);//Transition����ǰ¥�����¥�ص�

	}
	if (GetOpenDoorLight()) { SetOpenDoorLight(false); };
	if (GetCloseDoorLight()) { SetCloseDoorLight(false); };//�޶��������Ѱ�����Ϊ
}

void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)//��̬��⣬���������½�ʱ����⽫Ҫ�����Ŀ��¥��
	{
		SetMotorPower(0);//Transition��ֹͣ
		SetDoor(GetNearestFloor(), true);//��ǰ¥�����¥����
		*state = DoorOpen;
		SetCallLight(GetNearestFloor(), false, false);//��ǰ¥�����¥�½���ť����
		SetCallLight(GetNearestFloor(), true, false);//��ǰ¥�����¥������ť����
		if (floor == 1) { SetCallLight(GetNearestFloor(), false, false); };//Event
		SetPanelFloorLight(GetNearestFloor(), false);//Transition����ǰ¥�����¥�ص�

		if (GetOpenDoorLight()) { SetOpenDoorLight(false); };
		if (GetCloseDoorLight()) { SetCloseDoorLight(false); };//�޶��������Ѱ�����Ϊ
	}
}

void StateDoorOpen(int *state)
{
	GetNearestFloor();//��ȡ��ǰ��
	if (IsDoorOpen(GetNearestFloor()))//Event
	{
		SetDoor(GetNearestFloor(), false);//��ǰ¥�����¥����
		*state = DoorClosing;

	}
	if (GetCloseDoorLight())//Event
	{
		SetDoor(GetNearestFloor(), false);//��ǰ¥�����¥����
		SetCloseDoorLight(false);//�ص�
		*state = DoorClosing;

	}
	
	if (GetOpenDoorLight()) { SetOpenDoorLight(false); };
}

void StateDoorClosing(int *state)
{

	if (GetOpenDoorLight())//Event
	{
		SetDoor(GetNearestFloor(), true);//��ǰ¥�����¥����
		SetOpenDoorLight(false);//�ص�
		*state = DoorOpen;
	}
	if (GetCloseDoorLight()) { SetCloseDoorLight(false); };
	if (IsBeamBroken())//������̽�⵽�ڵ�����
	{
		SetDoor(GetNearestFloor(), true);//��ǰ¥�����¥����
		*state = DoorOpen;
	}
	if (IsDoorClosed(GetNearestFloor())) { *state = Idle; };
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
