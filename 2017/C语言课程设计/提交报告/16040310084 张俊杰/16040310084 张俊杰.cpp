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
		floor = IdleWhatFloorToGoTo(&up);
	if (floor > 0 && up)
	{
		SetMotorPower(1);//��Ǩ
		*state = MovingUp;
		printf("Idle��MovingUp.\n");
	}
	else if (floor > 0 && !up)//�¼�
	{
		SetMotorPower(-1);//��Ǩ
		*state = MovingDown;
		printf("Idle��MovingDown.\n");
	}
	else if (GetOpenDoorLight())//�¼�
	{
		SetOpenDoorLight(false);//����
		SetDoor(GetNearestFloor(), true);//��Ǩ
		*state = DoorOpen;
		printf("Idle��DoorOpen.\n");
	}
	else if (GetCallLight(GetNearestFloor() ,false))//�¼�
	{
		SetCallLight(GetNearestFloor(), true, false);//����
		SetDoor(GetNearestFloor(), true);//��Ǩ
		*state = DoorOpen;
		printf("Idle��DoorOpen.\n");
	}
	 if (GetCloseDoorLight())//�¼�
	{
		SetCloseDoorLight(false); //���ѣ����䣩
		*state = Idle;
		return;
	}


	
}

void StateMovingUp(int *state)
{
	int floor;
	floor = GoingDownToFloor();//�¼�
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)//�¼�
	{
		SetMotorPower(0);//��Ǩ
		SetDoor(GetNearestFloor(), true);//��Ǩ
		*state = DoorOpen;
		SetCallLight(GetNearestFloor(), true, false);//����
		SetCallLight(Lib_FloorNum, false, false);//����
		SetPanelFloorLight(GetNearestFloor(), false);//����
		*state = DoorOpen;
	}
	else  GetOpenDoorLight();//���
		GetCloseDoorLight();//���
		SetOpenDoorLight(false);//����
		SetCloseDoorLight(false);//����



}

void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)//�¼�
	{
		SetMotorPower(0);//��Ǩ
		SetDoor(GetNearestFloor(), true);//��Ǩ
		*state = DoorOpen;
		SetCallLight(GetNearestFloor(), false, false);//����
		SetCallLight(Lib_FloorNum, true, false);//����
		SetPanelFloorLight(GetNearestFloor(), false);//����
		*state = DoorOpen;
	}
	else  GetOpenDoorLight();//���
	GetCloseDoorLight();//���
	SetOpenDoorLight(false);//����
	SetCloseDoorLight(false);//����
}

void StateDoorOpen(int *state)
{

	GetNearestFloor();
	if (GetCloseDoorLight())//�¼�
	{
		SetDoor(GetNearestFloor(), false);//��Ǩ
		SetCloseDoorLight(false);//����
		*state = DoorClosing;

	}
	if (IsDoorOpen(GetNearestFloor()))//�¼�
	{
		SetDoor(GetNearestFloor(), false);//��Ǩ
		*state = DoorClosing;

	}
	if (GetOpenDoorLight())//�¼�
	{ 
		SetOpenDoorLight(false);//����
	}

}


void StateDoorClosing(int *state)
{
	if (GetOpenDoorLight())//�¼�
	{
		SetDoor(GetNearestFloor(), true);//��Ǩ
		SetOpenDoorLight(false);//����
		*state = DoorOpen;
	}
	if (GetCloseDoorLight())//�¼�
	{ 
		SetCloseDoorLight(false); //��Ǩ
	}
	if (IsBeamBroken())//�¼�
	{
		SetDoor(GetNearestFloor(), true);//��Ǩ
		*state = DoorOpen;
	}
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
