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
	floor = IdleWhatFloorToGoTo(&up);//���м��Ŀ��¥��������һ�����з���
	if (GetFloor()>1)//10s�޶����Զ���һ¥
		AutoTo1Floor();
	if (floor>-1 && up == 1)//�ǵ�ǰ¥���������
	{
		SetCallLight(GetNearestFloor(), 1, 0);
		*state = MovingUp;
	}
	if (floor>-1 && up == 0)
	{
		SetCallLight(GetNearestFloor(), 0, 0);
		*state = MovingDown;
	}
	if (GetOpenDoorLight())//����״̬�¿���
	{
		SetDoor(GetFloor(), 1);
		SetOpenDoorLight(0);
		*state = DoorOpen;
	}

	if (up&&GetCallLight(GetNearestFloor(), 1))//��ǰ¥�����
	{
		SetDoor(GetNearestFloor(), 1);
		SetCallLight(GetNearestFloor(), 1, 0);
		*state = DoorOpen;
	}
	if (!up&&GetCallLight(GetNearestFloor(), 1))
	{
		SetDoor(GetNearestFloor(), 1);
		SetCallLight(GetNearestFloor(), 0, 0);
		*state = DoorOpen;
	}

	if (GetCloseDoorLight())//���ţ��޷�Ӧ
	{
		SetCloseDoorLight(0);
		*state = Idle;
	}

}

void StateMovingUp(int *state)
{
	int floor = GoingUpToFloor();//��̬���ȥ��¥��
	CancelTo1Floor();
	SetMotorPower(1);
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)//��Ŀ�ĵأ�����
	{
		SetMotorPower(0);
		SetDoor(floor, 1);
		SetCallLight(floor, 1 || 0, 0);
		SetPanelFloorLight(floor, 0);
		*state = DoorOpen;
	}
	if (GetOpenDoorLight() || GetCloseDoorLight())//����״̬�¿��Ź��Ŷ�û��
	{
		SetOpenDoorLight(0);
		SetCloseDoorLight(0);
	}
	
}

void StateMovingDown(int *state)
{
	int floor = GoingDownToFloor();//��̬���Ҫȥ¥��
	CancelTo1Floor();
	SetMotorPower(-1);
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)//����Ŀ�ĵأ�����
	{
		SetMotorPower(0);
		SetDoor(floor, 1);
		SetCallLight(floor, 1 || 0, 0);
		SetPanelFloorLight(floor, 0);
		*state = DoorOpen;
	}
	if (GetOpenDoorLight() || GetCloseDoorLight())//����״̬�¿��Ź��Ŷ�û��
	{
		SetOpenDoorLight(0);
		SetCloseDoorLight(0);
	}

}

void StateDoorOpen(int *state)
{
	int floor = GetFloor();
	CancelTo1Floor();
	if (GetCloseDoorLight())//�ֶ�����
	{
		SetDoor(floor, 0);
		SetCloseDoorLight(0);
		*state = DoorClosing;
	}
	if (IsDoorOpen(floor))//���Ź����Զ�����
	{
		SetDoor(floor, 0);
		*state = DoorClosing;
	}
	if (GetOpenDoorLight())//����״̬�°������޶���
	{
		SetOpenDoorLight(0);
	}

}

void StateDoorClosing(int *state)
{
	int floor = GetFloor();
	CancelTo1Floor();
	if (GetOpenDoorLight())//�ֶ�����
	{
		SetDoor(floor, 1);
		SetOpenDoorLight(0);
		*state = DoorOpen;
	}
	if (GetCloseDoorLight())//����״̬�°������޷�Ӧ
	{
		SetCloseDoorLight(0);
	}
	if (IsDoorClosed(floor))//���Ž������������״̬
		*state = Idle;
}

void main_control(int *state)//����ѭ��
{
	if (IsElevatorRunning())
	{
		switch (*state)
		{
		case Idle:
			StateIdle(state); break;
		case MovingUp:
			StateMovingUp(state); break;
		case MovingDown:
			StateMovingDown(state); break;
		case DoorOpen:
			StateDoorOpen(state); break;
		case DoorClosing:
			StateDoorClosing(state); break;
		default:
			printf("û������״̬!!!\n");
		}
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
