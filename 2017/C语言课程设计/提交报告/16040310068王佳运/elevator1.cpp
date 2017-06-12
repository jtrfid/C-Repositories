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
	int floor, bool up;
	floor = IdleWhatFloorToGoTo(&up);
	if (floor > 0 && up)
	{
		SetMotorPower(1);
		*state = MovingUp;
	}//����������Movingup״̬
	if (floor > 0 && !up)
	{
		SetMotorPower(-1);
		*state = MovingDown;
	}//����������Movingdown״̬
	if (GetOpenDoorLight())
	{
		SetDoor(GetNearestFloor(), true);
		SetOpenDoorLight(true);
		*state = DoorOpen;
	}//���ڿ��Ű�ť�����ţ����ѿ��Ű�ť������Dooropen״̬
	if (GetCallLight(GetNearestFloor(), true))
	{
		SetDoor(GetNearestFloor(), true);
		SetCallLight(GetNearestFloor(), true, false);
		SetCallLight(GetNearestFloor(), false, false);
		*state = DoorOpen;
	}//���ں��а�ť�����ţ�����������а�ť������Dooropen״̬
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
		return;
	}//��ʱ��Ӧ���ǹرյģ���˽���ȡ���ŵƣ����رչ��ŵƣ������Ѱ�����Ϊ����ֹ��һ�����ظ�����ð�ť����Ϊ
}

void StateMovingUp(int *state)
{
	int floor;
	floor = GoingUpToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;
		SetCallLight(GetNearestFloor(), false, false);//����Ŀ��¥�㣬ֹͣ�����ţ�����Dooropen״̬����������down��ť
		if (floor = Lib_FloorNum)
		{
			SetCallLight(GetNearestFloor(), false, false);//������߲���������down��ť
		}
		SetPanelFloorLight(GetNearestFloor(), false);//��������¥��Ϊ��ǰ¥��İ�ť
		if (GetOpenDoorLight() || GetCloseDoorLight())
		{
			SetOpenDoorLight(false);
			SetCloseDoorLight(false);
		}//�˶�״̬��������ʧЧ�����ѿ�/�ذ�ť
	} 
}

void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;
		SetCallLight(GetNearestFloor(), false, false);////����Ŀ��¥�㣬ֹͣ�����ţ�����Dooropen״̬����������down��ť
		if (floor == 1)
		{
		   SetCallLight(GetNearestFloor(),true,false);
		}//����1����������up��ť
		SetPanelFloorLight(GetNearestFloor(), false);//��������¥��Ϊ��ǰ¥��İ�ť
	}
	if (GetOpenDoorLight() || GetCloseDoorLight())
	{
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);
	}//�˶�״̬��������ʧЧ�����ѿ�/���Ű�ť
}

void StateDoorOpen(int *state)
{
	if (GetCloseDoorLight())
	{
		SetDoor(GetNearestFloor(), false);
		SetCloseDoorLight(false);
		*state = DoorClosing;//���ڿ��š����˹��ţ����ţ�����Doorclosing״̬
	}
	if (IsDoorOpen(GetNearestFloor()))
	{
		SetDoor(GetNearestFloor(), true);
		*state = DoorClosing;
	}//���Ž��������й��ţ�����Doorclosing״̬
	if (GetOpenDoorLight())
	{
		SetOpenDoorLight(false);
	}//���ڿ��Ű��˿��ţ��޶��������ѿ��Ű�ť
}

void StateDoorClosing(int *state)
{
	if (GetOpenDoorLight())
	{
		SetDoor(GetNearestFloor(), true);
		SetOpenDoorLight(false);
		*state = DoorOpen;//���ڹ��Ű��˿��ŵƣ����ţ����ѿ��Ű�ť������Dooropen״̬
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
		*state = DoorClosing;//���ڹ��Ű��˹��ŵƣ��޶��������ѹ��Ű�ť
	}
	if (IsBeamBroken())
	{
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;//����̽�⵽�ڵ������ţ�����Dooropen״̬
	}
	if (IsDoorClosed(GetNearestFloor()))
	{
		*state = Idle;//���Ž��������Idle״̬
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
