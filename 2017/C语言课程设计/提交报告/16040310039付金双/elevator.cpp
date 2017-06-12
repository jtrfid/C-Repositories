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
	floor = IdleWhatFloorToGoTo(&up);//Event
	if (floor > 0 && up)
	{ 
		SetMotorPower(1);//Transition
		*state = MovingUp;
		printf("Transition:from Idle to MovingUp.\n");
		
	}
	if (floor > 0 && !up)//Event
	{
		SetMotorPower(-1);//Transition
		*state = MovingDown;
		printf("Transition:from Idle to MovingDown.\n");

	}

	if (GetOpenDoorLight())//Event
	{
		SetDoor(GetNearestFloor(), true);//Transition
		SetOpenDoorLight(false);
		*state = DoorOpen;
		printf("Transition:from Idle to DoorOpen .\n");
	}
	if (GetCallLight(GetNearestFloor(),true))//Event
	{
		SetDoor(GetNearestFloor(),true);//Transition
		SetCallLight(GetNearestFloor(),up,false);
		*state = DoorOpen;
		printf("Transition:from Idle to DoorOpen .\n");
	}
	if (GetCloseDoorLight())//Event
	{
		SetCloseDoorLight(false);//Transition
		return;
	}
}

void StateMovingUp(int *state)
{
	int floor;
	floor = GoingUpToFloor();//Event
	if (fabs(GetFloor()-floor)<Lib_FloorTolerance)//Event�� ��̬��⣬������������ʱ����⽫Ҫ�����Ŀ��¥��
	{
		SetMotorPower(0);//Transition
		SetDoor(GetNearestFloor(),true);
		*state = DoorOpen;
		SetCallLight(GetNearestFloor(),true,false);
		SetCallLight(GetNearestFloor(), false, false);
		if (floor == 3){ SetCallLight(GetNearestFloor(),true,false); };//Event
		SetPanelFloorLight(GetNearestFloor(),false);//Transition
		
	}
	if (GetOpenDoorLight()){ SetOpenDoorLight(false); };
	if (GetCloseDoorLight()){ SetCloseDoorLight(false); };//�޶��������Ѱ�����Ϊ
}

void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)//Event����̬��⣬���������½�ʱ����⽫Ҫ�����Ŀ��¥��
	{
		SetMotorPower(0);//Transition
		SetDoor(GetNearestFloor(),true);
		*state = DoorOpen;
		SetCallLight(GetNearestFloor(),false,false);
		SetCallLight(GetNearestFloor(), true, false);
		if (floor == 1){ SetCallLight(GetNearestFloor(),false,false); };//Event
		SetPanelFloorLight(GetNearestFloor(),false);//Transition
		
		if (GetOpenDoorLight()){ SetOpenDoorLight(false); };
		if (GetCloseDoorLight()){ SetCloseDoorLight(false); };//�޶��������Ѱ�����Ϊ
	}

}

void StateDoorOpen(int *state)
{
	GetNearestFloor();//��ȡ��ǰ��
	if (GetCloseDoorLight())//Event
	{
		SetDoor(GetNearestFloor(),false);//Transition
		SetCloseDoorLight(false);
		*state = DoorClosing;

	}
	if (IsDoorOpen(GetNearestFloor()))//Event
	{
		SetDoor(GetNearestFloor(),false);//Transition
		*state = DoorClosing;

	}
	if (GetOpenDoorLight()){ SetOpenDoorLight(false); };
}

void StateDoorClosing(int *state)
{
	
	if (GetOpenDoorLight())//Event
	{
		SetDoor(GetNearestFloor(),true);//Transition
		SetOpenDoorLight(false);
		*state = DoorOpen;
	}
	if (GetCloseDoorLight()){ SetCloseDoorLight(false); };
	if (IsBeamBroken())//Event������̽�⵽�ڵ�����
	{
		SetDoor(GetNearestFloor(),true);//Transition
		*state = DoorOpen;
	}
	if (IsDoorClosed(GetNearestFloor())){ *state = Idle; };

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
