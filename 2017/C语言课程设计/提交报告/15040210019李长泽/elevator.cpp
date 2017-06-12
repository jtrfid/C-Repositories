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
	floor = IdleWhatFloorToGoTo(&up);  /*ȷ�����ݵ����Ŀ��¥��*/ 
	/*��¥����ȷ��������ť����ʱ������������״̬ת�Ƶ�����״̬*/ 
	if (floor > 0 && up) 			 
	{
		SetMotorPower(1);
		*state = MovingUp;
	}
	/*��¥����ȷ���½���ť����ʱ�������½���״̬ת�Ƶ��½�״̬*/ 
	else if (floor > 0 && !up) 		
	{
		SetMotorPower(-1);
		*state = MovingDown;
	}
	/*�������ڲ����ŵ���ʱ������¥��ĵ����Ŵ򿪣�״̬ת�Ƶ��ſ�״̬�����ѿ��ŵ�*/ 
	if (GetOpenDoorLight()) 		
	{
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;
		 SetOpenDoorLight(false); return;
	}
	/*����¥����������µ��ݵ�����ʱ�򣬴򿪱�¥����ţ�״̬ת�Ƶ��ſ�״̬���������µ��ݰ�ť��*/ 
	else if (GetCallLight(GetNearestFloor(), true))
	{
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;
		SetCallLight(GetNearestFloor(), true, false);

	}
	else if (GetCallLight(GetNearestFloor(), false))
	{
		SetDoor(GetNearestFloor(), true);
		SetCallLight(GetNearestFloor(), false, false);
		*state = DoorOpen;

	}
	/*�������ڲ����ŵ�����ʱ�򣬲������������������ѹ��ŵ�*/ 
	if (GetCloseDoorLight()) 
	{ SetCloseDoorLight(false); return; }

}

void StateMovingUp(int *state)
{
	int floor;
	floor = GoingUpToFloor();		/*���������Ҫ��������¥��*/ 
	
	/*����ǰ¥����Ҫ����¥��Ĳ�ֵС���������ʱ������ֹͣ�˶��������Ŵ�*/ 
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(floor,true);
	/*����ǰ¥��Ϊ���㣬���ѵ������½���ť���������ѵ�����������ť��״̬�л����ſ�״̬*/ 
	 if (GetNearestFloor() == 3)
	{
		SetCallLight(floor, false, false);
		*state = DoorOpen;
	}
	else
		SetCallLight(floor, true, false);
		*state = DoorOpen;
	}
	/*�����������У�������Ź��ŵƱ����£��������ѵ�����ִ����������*/ 
	if (GetOpenDoorLight())
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
	floor = GoingDownToFloor();		/*���������Ҫ��������¥��*/ 
	/*����ǰ¥����Ҫ����¥��Ĳ�ֵС���������ʱ������ֹͣ�˶��������Ŵ�*/ 
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(floor, true);
		/*����ǰ¥��Ϊһ�㣬���ѵ�����������ť���������ѵ������½���ť��״̬�л����ſ�״̬*/ 
		if (GetNearestFloor() == 1)
		{
			SetCallLight(floor, true, false);
		}
		else
			SetCallLight(floor, false, false);
		*state = DoorOpen;
	}
	/*���½������У�������Ź��ŵƱ����£��������ѵ�����ִ����������*/ 
	if (GetOpenDoorLight())
	{
		SetOpenDoorLight(false);
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
	}
}

inline void StateDoorOpen(int *state)
{
	/*������״̬�¹��ŵƴ򿪣����������ţ����ѹ��ŵƣ�״̬ת�Ƶ����ڹ���*/ 
	if (GetCloseDoorLight())
	{
		SetDoor(GetNearestFloor(),false);
		SetCloseDoorLight(false);
		*state = DoorClosing;
	}
	/*������״̬�¿��ŵƴ򿪣����ѿ��ŵ�*/ 
	if (GetOpenDoorLight())
	{
		SetOpenDoorLight(false);
	}
	/*����ſ��ţ��ص��ò���ţ�״̬ת�Ƶ����ڹ���״̬*/ 
	if (IsDoorOpen(GetNearestFloor()))
	{
		SetDoor(GetNearestFloor(), false);
		*state = DoorClosing;
	}
	/*��������¥��ư�ť*/ 
	if (GetPanelFloorLight(GetNearestFloor()))
	{
		SetPanelFloorLight(GetNearestFloor(), false);
	}
}

void StateDoorClosing(int *state)
{
	/*���ŵư�ť���£��������ţ����ѿ��ŵư�ť��״̬ת�Ƶ��ſ�״̬*/ 
	if (GetOpenDoorLight())
	{
		SetDoor(GetNearestFloor(),true);

		SetOpenDoorLight(false);
		*state = DoorOpen;
	}
	/*���ŵư�ť���£����ѹ��ŵư�ť*/ 
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
	}
	/*���Ź�����������ϰ�����Ŵ򿪣�״̬ת�Ƶ��ſ�״̬*/ 
	if (IsBeamBroken())
	{
		SetDoor(GetNearestFloor(),true);
		*state = DoorOpen;
	}
	/*�Źرպ�ת�Ƶ�Idle״̬*/ 
	if (IsDoorClosed(GetNearestFloor()))
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
