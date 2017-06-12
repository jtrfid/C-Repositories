#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ElevatorLib.h"

/**********************************************
* Idle״̬������ֹͣ��ĳ¥�㣬���ǹرյģ����ھ�ֹ״̬���ȴ�����¼��ķ������Ӷ�ת������һ��״̬��
**********************************************/
void StateIdle(int *state)//����״̬����
{
	int floor;
	bool up;
	floor = IdleWhatFloorToGoTo(&up); // Event��⽫Ҫ����Ŀ��¥��
	if (floor > 0 && up)//���ݴӾ�ֹ״̬����
	{
		SetMotorPower(1);
		*state = MovingUp;
		printf("from Idle to MovingUp.\n");
	}
	else if (floor>0 && !up)//�޲������½�
	{
		SetMotorPower(-1);
		*state = MovingDown;
		printf("from Idle to MovingDown.\n");
	}
	else if (GetOpenDoorLight())//���ѿ��Ű�ť
	{
		SetDoor(GetNearestFloor(), true);
		SetOpenDoorLight(false);
		*state = DoorOpen;
	}
	else if (GetCallLight(GetNearestFloor(), true))//��������up/down��ť
	{
		SetDoor(GetNearestFloor(), true);
		SetCallLight(GetNearestFloor(), true, false);
		*state = DoorOpen;
	}
	else if (GetCallLight(GetNearestFloor(), false))//�رհ�ť�ƣ���ֹ��һ�����ظ�����˰�ť��Ϊ��
	{
		SetDoor(GetNearestFloor(), true);
		SetCallLight(GetNearestFloor(), false, false);
		*state = DoorOpen;
	}
	else if (GetCloseDoorLight())//��˽���ȡ���ŵƣ����رչ��ŵ�
	{
		SetCloseDoorLight(false);
		return;
	}
}


void StateMovingUp(int *state)//����״̬����
{
	int floor = GoingUpToFloor();
	bool up;
	up = GetCallLight(floor, true);
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)// ����Ŀ��¥��,����
	{
		SetMotorPower(0);
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;
		SetCallLight(GetNearestFloor(), true, false);
		if (floor = Lib_FloorNum)//�ж��Ƿ�Ϊ��߲�
		{
			SetCallLight(GetNearestFloor(), false, false);
		}
		SetPanelFloorLight(GetNearestFloor(), false);

	}

	if (GetOpenDoorLight() || GetCloseDoorLight())//�˶�״̬��������ʧЧ
	{
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);

	}


}

void StateMovingDown(int *state)//�½�״̬����
{
	int floor = GoingDownToFloor();
	bool up;
	up = GetCallLight(floor, true);
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)//�ж��Ƿ񵽴�Ŀ��¥��
	{
		SetMotorPower(0);
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;
		SetCallLight(GetNearestFloor(), false, false);
		if (floor = 1)//��������up��ť
		{
			SetCallLight(GetNearestFloor(), true, false);
		}
		SetPanelFloorLight(GetNearestFloor(), false);
	}
	if (GetOpenDoorLight() || GetCloseDoorLight())//�˶�״̬��������ʧЧ
	{
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);

	}
}

void StateDoorOpen(int *state)//����״̬����
{
	int floor = GetNearestFloor();
	bool up;
	if (GetCloseDoorLight())//�����а����ż������й��Ų���
	{
		SetDoor(GetNearestFloor(), false);
		SetCloseDoorLight(false);
		*state = DoorClosing;
	}
	if (IsDoorOpen(floor))//���Ž������Զ�����
	{
		SetDoor(GetNearestFloor(), false);
		*state = DoorClosing;
	}
	if (GetOpenDoorLight())//�����У������ź��޶���
	{
		SetOpenDoorLight(false);
	}
}

void StateDoorClosing(int *state)//����״̬����
{
	int floor = GetNearestFloor();
	bool up;
	if (GetOpenDoorLight())//�����а����ż������п��Ų���
	{
		SetDoor(GetNearestFloor(), true);
		SetOpenDoorLight(false);
		*state = DoorOpen;
	}
	if (GetCloseDoorLight())//���ţ����˹��ŵ�
	{
		SetCloseDoorLight(false);
	}
	if (IsBeamBroken())//�ж��Ƿ�������赲
	{
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;
	}
	if (IsDoorClosed(floor))//���Ž����󣬵����ֹ���ȴ���һ��ָ��
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
