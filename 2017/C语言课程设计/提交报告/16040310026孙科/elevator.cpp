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
	floor = IdleWhatFloorToGoTo(&up); //��̬���
	if (floor > 0 && up)
	{
		SetMotorPower(1);  //�����������
		*state = MovingUp;  //����MovingUp״̬
	}
	floor = IdleWhatFloorToGoTo(&up);
	if (floor > 0 && !up)
	{
		SetMotorPower(-1);
		*state = MovingDown;
	}
	floor=GetNearestFloor();  //��ȡ���¥��
	if (GetOpenDoorLight())
	{
		SetDoor(floor, true);
		SetOpenDoorLight(false);  //���ѿ��Ű�ť
		*state = DoorOpen;
	}
	if (GetCallLight(floor, true)||GetCallLight(floor, false))//����up��down���а�ť����
	{
		SetDoor(floor, true);
		SetCallLight(floor, true, false);
		SetCallLight(floor, false, false);//���ţ�����������up��down��ť����ֹ��һ�����ظ����д˰�ť��Ϊ
		*state = DoorOpen;
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
		return;
	}
}

void StateMovingUp(int *state)
{
	int floor; bool up;
	floor = GoingUpToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)//�������е�Ŀ��¥��
	{
		SetMotorPower(0);
		SetDoor(floor, true);
		*state = DoorOpen;
		SetCallLight(floor, true, false);

	}
	if (floor == Lib_FloorNum)//������߲㣬��������down��ť
	{
		SetCallLight(floor, false, false);
	}
	
	SetPanelFloorLight(floor, false);
		GetNearestFloor();
	if (GetOpenDoorLight() || GetCloseDoorLight())//�˶�״̬��������ʧЧ���޶���
	{
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);
	}

}

void StateMovingDown(int *state)
{
	int floor; bool up;
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(floor, true);
		*state = DoorOpen;
		SetCallLight(floor, false, false);
		if (floor == 1)                          //����1�㣬��������up��ť
		{
			SetCallLight(floor, true, false);
		}
	}
	SetPanelFloorLight(floor, false);
	GetNearestFloor();
	if (GetOpenDoorLight() || GetCloseDoorLight())
	{
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);//�޶��������ѿ����Ű�ť
	}
}


void StateDoorOpen(int *state)
{
	int floor; bool up;
	floor = GetNearestFloor();
	if (GetCloseDoorLight())
	{
		SetDoor(floor, false);
		SetCloseDoorLight(false);
		*state = DoorClosing;
	}
	if (IsDoorOpen(floor))//���Ž������Զ����й��ţ�����DoorClosing״̬
	{
		SetDoor(floor, false);
	    *state = DoorClosing;
    }
	if (GetOpenDoorLight())
	{
		SetOpenDoorLight(false);
	}
}

void StateDoorClosing(int *state)
{
	int floor; bool up;
	floor = GetNearestFloor();
	if (GetOpenDoorLight())
	{
		SetDoor(floor, true);
		SetOpenDoorLight(false);
		*state = DoorOpen;
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
	}
	if (IsBeamBroken())//�������̽�⵽�ڵ���ת�����ţ�����DoorOpen״̬
	{
		SetDoor(floor, true);
		*state = DoorOpen;
	}
	if(IsDoorClosed(floor))
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
