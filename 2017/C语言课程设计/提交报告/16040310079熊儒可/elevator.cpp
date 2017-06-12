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
	floor = IdleWhatFloorToGoTo(&up);//��ȡ��Ҫ����¥��
	if (floor > 0 && up)
	{
		SetMotorPower(1);
		*state = MovingUp;
		printf("Transition: from Idle to MovingUp.\n");
	}
	if (floor > 0 && !up)
	{
		SetMotorPower(-1);
		*state = MovingDown;
		printf("Transition: from Idle to MovingDown.\n");
	}
	if (GetOpenDoorLight())
	{
		SetOpenDoorLight(false);
		SetDoor(floor, true);
		*state = DoorOpen;
		printf("Transition: from Idle to DoorOpen.\n");
	}
	floor = GetNearestFloor();//ʹ����floor��Ӧ��¥��
	if (GetCallLight(floor, true))//����ò�¥�����ϵ��������ţ��������ϵ�
	{
		SetCallLight(floor, true, false);
		SetDoor(floor, true);
		*state = DoorOpen;
		printf("Transition: from Idle to DoorOpen.\n");
	}
	if (GetCallLight(floor, false))//����ò�¥�����µ��������ţ����������µ�
	{
		SetCallLight(floor, false, false);
		SetDoor(floor, true);
		*state = DoorOpen;
		printf("Transition: from Idle to DoorOpen.\n");
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
		return;
	}
}

void StateMovingUp(int *state)
{
	int floor;
	floor = GoingUpToFloor();//��ȡ����ʱ���������¥��
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		if (GetCallLight(floor, true))//��һ��if��������Ŀ��¥�����ϵư�ť�����ڰ�ť
		{
			SetCallLight(floor, true, false);
			SetPanelFloorLight(floor, false);
			SetMotorPower(0);
			SetDoor(floor, true);
			*state = DoorOpen;
			printf("Transition: from MovingUp to DoorOpen.\n");
			return;
		}
		if (GetCallLight(floor, false))//�ڶ���if��������Ŀ��¥�����µư�ť�����ڰ�ť
			SetCallLight(floor, false, false);
		SetPanelFloorLight(floor, false);
		SetMotorPower(0);
		SetDoor(floor, true);
		*state = DoorOpen;
		printf("Transition: from MovingUp to DoorOpen.\n");
	}
	if (GetOpenDoorLight())
	{
		SetOpenDoorLight(false);
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
		return;
	}
}

void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();//��ȡ�½�ʱ���������¥��
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		if (GetCallLight(floor, true))//��һ��if��������Ŀ��¥�����ϵư�ť�����ڰ�ť
		{
			SetCallLight(floor, true, false);
			SetPanelFloorLight(floor, false);
			SetMotorPower(0);
			SetDoor(floor, true);
			*state = DoorOpen;
			printf("Transition: from MovingDown to DoorOpen.\n");
			return;
		}
		if (GetCallLight(floor, false))//�ڶ���if��������Ŀ��¥�����µư�ť�����ڰ�ť
			SetCallLight(floor, false, false);
		SetPanelFloorLight(floor, false);
		SetMotorPower(0);
		SetDoor(floor, true);
		*state = DoorOpen;
		printf("Transition: from MovingDown to DoorOpen.\n");
	}
	if (GetOpenDoorLight())
	{
		SetOpenDoorLight(false);
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
		return;
	}
}

void StateDoorOpen(int *state)
{
	int floor;
	floor = GetNearestFloor();
	if (GetCloseDoorLight())
	{
		SetDoor(floor, false);
		SetCloseDoorLight(false);
		*state = DoorClosing;
		printf("Transition: from DoorOpen to DoorClosing.\n");
	}
	if (IsDoorOpen(floor))//������Ƿ���ȫ��
	{
		SetDoor(floor, false);
		*state = DoorClosing;
		printf("Transition: from DoorOpen to DoorClosing.\n");
	}
	if (GetOpenDoorLight())
	{
		SetOpenDoorLight(false);
		return;
	}
}

void StateDoorClosing(int *state)
{
	int floor;
	floor = GetNearestFloor();
	if (GetOpenDoorLight())
	{
		SetDoor(floor, true);
		SetOpenDoorLight(false);
		*state = DoorOpen;
		printf("Transition: from DoorClosing to DoorOpen.\n");
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
		return;
	}
	if (IsBeamBroken())//����Ƿ����ϰ���
	{
		SetDoor(floor, true);
		SetOpenDoorLight(false);
		*state = DoorOpen;
		printf("Transition: from DoorClosing to DoorOpen.\n");
	}
	if (IsDoorClosed(floor))//������Ƿ���ر�
	{
		*state = Idle;
		printf("Transition: from DoorClosing to Idle.\n");
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
