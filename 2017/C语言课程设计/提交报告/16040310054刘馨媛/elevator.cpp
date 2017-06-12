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
	floor = IdleWhatFloorToGoTo(&up);//Event
	if (floor>0 && up)
	{
		SetMotorPower(1);//Transition
		*state = MovingUp;
		printf("ע�⣬��������");
	}//������Ч¥�㣬��������
	else if (floor>0 && !up)
	{
		SetMotorPower(-1);//Transition
		*state = MovingDown;
		printf("ע�⣬�����½�");
	}//������Ч¥�㣬�����½�
	if (GetOpenDoorLight())//Event
	{
		SetDoor(GetNearestFloor(), true);//Transition
		SetOpenDoorLight(false);//���ѿ��Ű���
		printf("������");
		*state = DoorOpen;//���¿��Ű��������ݴ�
	}
	if (GetCallLight(GetNearestFloor(), true))//Event
	{
		SetDoor(GetNearestFloor(), true);//Transition
		SetOpenDoorLight(false);//���ѿ��Ű���
		*state = DoorOpen;
		printf("������");
	}

	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);//������Ч����
		return;
	}
}

void StateMovingUp(int *state)
{
	int floor;
	floor = GoingUpToFloor();
	if (fabs(GetFloor() - floor)<Lib_FloorTolerance)//Event
	{
		SetMotorPower(0);
		SetDoor(GetNearestFloor(), true);
		if (GetNearestFloor()<Lib_FloorNum)
			SetCallLight(GetNearestFloor(), true, false);
		else SetCallLight(GetNearestFloor(), false, false);
		SetPanelFloorLight(GetNearestFloor(), false);//���Ѱ���
		*state = DoorOpen;
		printf("������");//�����ƶ���ָ��λ�ÿ���
	}
	if (GetOpenDoorLight())
		SetOpenDoorLight(false);
	if (GetCloseDoorLight())
		SetCloseDoorLight(false);//�����˶�ʱ���ѿ��ţ����ŵ���Ч����
}

void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor)<Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;
		printf("������");
	}
	if (GetNearestFloor() == 1)
		SetCallLight(1, true, false);//������һ¥ʱ�������������������
	else SetCallLight(GetNearestFloor(), false, false);
	SetPanelFloorLight(GetNearestFloor(), false);//��������Ϊ��ǰ¥��İ���
	if (GetOpenDoorLight())
		SetOpenDoorLight(false);
	if (GetCloseDoorLight())
		SetCloseDoorLight(false);//�����˶�ʱ���ѿ��ţ����ŵ���Ч����
}

void StateDoorOpen(int *state)
{
	if (GetCloseDoorLight())
	{
		SetDoor(GetNearestFloor(), false);
		SetCloseDoorLight(false);
		*state = DoorClosing;
		printf("ע�⣬������");//��⵽����������ɹ��ţ������ѹ��Ű���
	}
	if (IsDoorOpen(GetNearestFloor()))
	{
		SetDoor(GetNearestFloor(), false);
		*state = DoorClosing;
		printf("ע�⣬������");
	}
	if (GetOpenDoorLight())
		SetOpenDoorLight(false);//������Ч�Ŀ��Ű���
}

void StateDoorClosing(int *state)
{
	if (GetOpenDoorLight())
	{
		SetDoor(GetNearestFloor(), true);
		SetOpenDoorLight(false);
		*state = DoorOpen;
		printf("ע�⣬������");

	}

	if (GetCloseDoorLight())
		SetCloseDoorLight(false);//������Ч�Ĺ��Ű���
	if (IsBeamBroken())
	{
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;
		printf("ע�ⰲȫ");//����ʱ��⵽���ڵ�����Ͽ���
	}
	if (IsDoorClosed(GetNearestFloor()))
	{
		*state = Idle;//������ɣ��ָ�ԭʼ״̬
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
