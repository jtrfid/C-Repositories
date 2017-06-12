m#include "stdafx.h"
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
	floor = IdleWhatFloorToGoTo(&up);                                       //��⽫Ҫ�����¥�㡣
	if (floor > 0 && up)                                                     //¥����ڵ�ǰ¥�㣬������
	{
		SetMotorPower(1);                                                  
		*state = MovingUp;                                                 //  ״̬����Idle״̬��ΪMovingUp��
	}
	if (floor>0 && !up)
	{
		SetMotorPower(-1);
		*state = MovingDown;                                              //  ״̬����Idle״̬��ΪMovingDown��      

	}
	if (GetOpenDoorLight())                                                //�ȴ����Ű�ť��ָ�
	{
		SetOpenDoorLight(false);
		SetDoor;
		*state = DoorOpen;
	}
	if (GetCallLight(floor = GetNearestFloor(), true) || GetCallLight(floor = GetNearestFloor(), false))
	{
		SetCallLight(floor = GetNearestFloor(), true, false);
		SetCallLight(floor = GetNearestFloor(), false, false);
		SetDoor;
		*state = DoorOpen;                                                 //�������ⰴť�����½���DoorOpen״̬
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
		*state = Idle;                                                    //���ڹ��Ű�ťָ�����Idle״̬
	}
}

void StateMovingUp(int *state)
{
	int  floor;
	GetFloor();
	floor = GoingUpToFloor();
	if (fabs(GetFloor() - floor)<Lib_FloorTolerance)                       //�������Ƿ񵽴�Ŀ��¥�㡣
	{
		SetMotorPower(0); SetDoor(floor, true);
		*state = DoorOpen;
		SetPanelFloorLight(floor, false);

	}
	if (GetOpenDoorLight() || GetCloseDoorLight())
	{
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);                                         //���Ѱ�ť����ֹ��һ���ڼ����ð�ť
		*state = DoorOpen;
	}
}

void StateMovingDown(int *state)
{
	int  floor; bool up;
	GetFloor();
	floor = GoingUpToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0); SetDoor();
		*state = DoorOpen;
		SetPanelFloorLight(floor,false);                                 //���ѵ�ǰ����¥�㰴ť
	}
	if (GetOpenDoorLight() || GetCloseDoorLight())
	{
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);
		*state = DoorOpen;
	}
}

}

void StateDoorOpen(int *state)
{
	if (GetCloseDoorLight())
	{
		SetDoor(floor,false);
		SetCloseDoorLight(false);
		*state = DoorOpen;                                              //�ɲ����ⲿ���ŵĿ���״̬
	}
	if (IsDoorOpen())
	{
		SetDoor(floor,true);
		*state = DoorOpen;
	}
	if (GetOpenDoorLight())
	{
		SetOpenDoorLight(false);
		*state = DoorOpen;
	}
}

}

void StateDoorClosing(int *state)
{
	int Floor; Floor = GetNearestFloor();
	if (GetOpenDoorLight() || IsBeamBroken())                          //�����������Ƿ����ϰ���
	{
		SetDoor(floor,true);
		SetOpenDoorLight(false);
		*state = DoorOpen;
	}

	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
		*state = DoorOpen;
	}
	if (IsDoorClosed())
	{
		*state = Idle;
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
