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
	bool s;
	floor = IdleWhatFloorToGoTo(&up); 
	if (floor > 0 && up)  
	{
		SetMotorPower(1);  //�������¥�㰴ť������
		*state = MovingUp;
		printf("Transition: from Idle to Moving up.\n");
	}
	else if (floor > 0 && !up)  
	{
		SetMotorPower(-1); //ͬ�ϣ��½�
		*state = MovingDown;
		printf("Transition: from Idle to Moving down.\n");
	}
	else if (GetOpenDoorLight())  
	{
		SetDoor(floor, true);
		SetOpenDoorLight(false);//������ڿ��Ű�ť  
		*state = DoorOpen;
		printf("Transition: from Idle to Door open.\n");
	}
	else if (GetCallLight(1, true)) 
	{
		SetDoor(1, true);
		SetCallLight(1, true, false); //���������а�ť
		*state = DoorOpen;
		printf("Transition: from Idle to Door open.\n");
	}
	else if (GetCallLight(GetNearestFloor(), true)) 
	{
		SetDoor(floor, true);
		SetCallLight(floor, true, false); 
		*state = DoorOpen;
		printf("Transition: from Idle to Door open.\n");
	}
	else if (GetCallLight(GetNearestFloor(), false)) 
	{
		SetDoor(floor, true);
		SetCallLight(floor, false, false);
		*state = DoorOpen;
		printf("Transition: from Idle to Door open.\n");
	}
	else if (GetCloseDoorLight()) 
	{
		SetCloseDoorLight(false); 
		return;
	}
}

void StateMovingUp(int *state)
{
#define Lib_FloorNum  3
#define Lib_FloorTolerance 0.01
	int floor;
	floor = GoingUpToFloor();//��̬���
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetCallLight(floor, false, false);//���Ѱ�ť
		SetPanelFloorLight(floor, false);//���Ѱ�ť
		SetDoor(floor, true);
		if (GetNearestFloor() == Lib_FloorNum) 
		{
			SetCallLight(floor, false, false);//��Ϊ�������°�ť
		}
		*state = DoorOpen;
		printf("Transition: from MovingUp to Door open.\n");

	}
	if (GetOpenDoorLight() || GetCloseDoorLight())
	{
		SetOpenDoorLight(false);
		SetCloseDoorLight(false)
		return;
	}
}

void StateMovingDown(int *state)
{
#define Lib_FloorTolerance 0.01
	int floor;
	floor = GoingDownToFloor();
	int GetNearestFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)  
		SetMotorPower(0);
		SetDoor(floor, true); 
		*state = DoorOpen;
		printf("Transition: from MovingDown to Door open.\n");
		if (floor > 1)
			SetCallLight(floor, false, false);
		else   SetCallLight(floor, true, false);//��Ϊ�������ϰ�ť
		SetPanelFloorLight(floor, false);
	}
	else if (GetOpenDoorLight() || GetCloseDoorLight()) 
	{
		SetOpenDoorLight(false);  
		SetCloseDoorLight(false);
		return;
	}
}

void StateDoorOpen(int *state)
{
	int floor;
	if (GetCloseDoorLight())  
	{
		SetDoor(GetNearestFloor(), false);
		SetCloseDoorLight(false); 
		*state = DoorClosing;
		printf("Transition: from Door open to Door closing.\n");
	}
	else if (IsDoorOpen(GetNearestFloor()))  
	{
		SetDoor(GetNearestFloor(), false);  
		*state = DoorClosing;
		printf("Transition: from Door open to Door closing.\n");
	}
	else if (GetOpenDoorLight())  
	{
		SetOpenDoorLight(false);  
		*state = DoorClosing;
		printf("Transition: from Door open to Door closing.\n");
	}
}

void StateDoorClosing(int *state)
{
	int floor;
	if (GetOpenDoorLight())  
	{
		SetDoor(floor, true);
		SetOpenDoorLight(false);  
		*state = DoorOpen;
		printf("Transition: from Door closing to Door open.\n");
	}
	else if (GetCloseDoorLight())  
	{
		SetCloseDoorLight(false);  
		return;
	}
	else if (IsBeamBroken())  
	{
		SetDoor(floor, true); 
		*state = DoorOpen;
		printf("Transition: from Door closing to Door open.\n");
	}
	else if (IsDoorClosed(GetNearestFloor()))
		*state = Idle;
	printf("Transition: from Door closing to Idle.\n");
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
