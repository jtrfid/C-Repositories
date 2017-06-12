#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ElevatorLib.h"

/**********************************************
* Idle״̬������ֹͣ��ĳ¥�㣬���ǹرյģ����ھ�ֹ״̬���ȴ�����¼��ķ������Ӷ�ת������һ��״̬��
**********************************************/
bool up;
void StateIdle(int *state)
{
	int floor;
	floor = IdleWhatFloorToGoTo(&up);//Ҫ�����Ŀ��¥��
	if (floor > 0 && up) 
		SetMotorPower(1); //��������
	else if (floor > 0 && !up) 
		SetMotorPower(-1);//��������
	else if (GetOpenDoorLight())//���ŵ�״̬
	{
		SetOpenDoorLight(false);
		SetDoor(floor, true);//����
	}
	else if (GetCallLight(floor, false))
	{
		SetCallLight(floor, false, false);
		SetDoor(floor, true);//����
	}
	else if (GetCallLight(floor, true))
	{
		SetCallLight(floor, up, false);
		SetDoor(floor, true);//����
	}
	else if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
		return;
	}
}

void StateMovingUp(int *state)
{
	int floor;
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor)<Lib_FloorTolerance)//���ݵ���
	{
		SetMotorPower(0);//ֹͣ
		SetDoor(floor, true);//����
	    if (floor == Lib_FloorNum)//¥��Ϊ��߲�
		SetCallLight(floor, false, false);
		else if (floor != Lib_FloorNum)
			SetCallLight(floor, true, false);
	}
	if (fabs(GetFloor() - floor)>Lib_FloorTolerance)
	{
		SetCloseDoorLight(false);
		SetOpenDoorLight(false);
	}
}

void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor)<Lib_FloorTolerance)//���ݴ�������״̬
	{
		SetMotorPower(0);//ֹͣ
		SetDoor(floor, true);//����
	    if (floor == 1)
		SetCallLight(floor, true, false);
		if (floor > 1) 
			SetCallLight(floor, false, false);
		SetPanelFloorLight(floor, false);
	}
	if (fabs(GetFloor() - floor)>Lib_FloorTolerance)//���ݴ�������״̬
	{
		SetCloseDoorLight(false);
		SetOpenDoorLight(false);
	}
}

void StateDoorOpen(int *state)
{
	int floor;
	if (GetOpenDoorLight())//���Ű�ť����
	{
		SetDoor(floor, true);//����
		SetOpenDoorLight(false);
	}
	else if (IsDoorOpen(floor))
		SetDoor(floor, false);//����
	else if (GetCloseDoorLight())//���Ű�ť����
	{
		SetDoor(floor, false);//����
		SetCloseDoorLight(false);
	}
}

void StateDoorClosing(int *state)
{
	int floor;
	if (GetOpenDoorLight())//���Ű�ť״̬
	{
		SetOpenDoorLight(false);
		SetDoor(floor, true);//����
	}
	else if (GetCloseDoorLight())//���Ű�ť״̬
	{
		SetCloseDoorLight(false);
		SetDoor(floor, false);//����
	}
	else if (IsBeamBroken())//����Ƿ����������ż�
		SetDoor(floor, true);//����
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

