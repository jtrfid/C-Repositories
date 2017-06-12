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
		SetMotorPower(1);                                                                  //��������
		*state = MovingUp;                                                                 //ת��movingup״̬
		printf("Transition: from Idle to MovingUp.\n");
	}
	else if (floor > 0 && !up)
	{
		SetMotorPower(-1);                                                              //�����½�
		*state = MovingDown;                                                         //ת��movingdown״̬
		printf("Transition: from Idle to MovingUp.\n");
	}
	else if (GetOpenDoorLight())
	{
		SetDoor(GetNearestFloor(),true);
		SetOpenDoorLight(false);                                                     //���ѿ��Ű�ť
		*state = DoorOpen;                                                          //ת��dooropen״̬
		printf("Transition: from Idle to MovingUp.\n");
	}
	else if (GetCallLight(floor,true))
	{
		SetDoor(floor,true);              //����
		SetCallLight(floor,true,false);             //�رտ��ŵ�
		*state = DoorOpen;                    //ת��dooropen״̬
	}
	else if (GetCallLight(floor, false))        //����
	{
		SetDoor(floor, true);             //����
		SetCallLight(floor, false, false);          //�ص�
		*state = DoorOpen;             //ת��dooropen״̬
	}
	else if (GetCloseDoorLight)
	{
		SetCloseDoorLight(false);
		return;                                     //���Ѱ���
	}
}

void StateMovingUp(int *state)
{
		int floor;
		floor = GoingUpToFloor();
		if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
		{
			SetMotorPower(0);             //����ֹͣ
			SetDoor(GetNearestFloor(), true);        //����
			*state = DoorOpen;                  //ת��dooropen״̬
		}
		if (GetCallLight(GetNearestFloor(),true))           //����up��
			SetCallLight(GetNearestFloor(),true,false);      
		else if (Lib_FloorNum)
		{
			SetCallLight(GetNearestFloor(), false,false);           //����3��down��
		}
		SetPanelFloorLight(GetNearestFloor(), false);               //��������¥�㰴ť
		if (GetOpenDoorLight())
			SetOpenDoorLight(false);                                
		if (GetCloseDoorLight())
			SetCloseDoorLight(false);                                //�������ڿ��ŵ��Լ����ŵ�
}

void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();                                //����floorΪ��ǰ��
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);            //����ֹͣ
		SetDoor(GetNearestFloor(), true);       //����
		*state = DoorOpen;           //ת��dooropen״̬
	}
	if (GetCallLight(GetNearestFloor(), true))
		SetCallLight(GetNearestFloor(), true, false);            //��������up��
	else if (Lib_FloorNum)
	{
		SetCallLight(GetNearestFloor(), false, false);
	}
	SetPanelFloorLight(GetNearestFloor(), false);     //�������ڸò㰴ťi
	SetCallLight;
	if (Lib_FloorNum)
		SetCallLight;

	SetPanelFloorLight;
}

void StateDoorOpen(int *state)
{
	if (GetCloseDoorLight())
	{
		SetDoor(GetNearestFloor(), false);                         //����
		SetCloseDoorLight(false);           //�������ڹ��ŵ�

		*state = DoorClosing;              //ת��doorclosing״̬
	}
	if (IsDoorOpen(GetNearestFloor()))
	{
		SetDoor(GetNearestFloor(),false);                //���й���
		*state = DoorClosing;                  //ת��doorclosing״̬
	}
	else if (GetOpenDoorLight())
		SetOpenDoorLight(false);                 //�������ڿ��ŵ�
}

void StateDoorClosing(int *state)
{
	if (GetOpenDoorLight())
	{
		SetDoor(GetNearestFloor(),false);                  //����
		SetOpenDoorLight(false);                           //�������ſ��ŵ�
		*state = DoorOpen;                                //ת��dooropen״̬
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);                      //�������Ź��ŵ�
	}
	if (IsBeamBroken())
	{
		SetDoor(GetNearestFloor(), true);                //�������߼�⵽������δͨ����ֹͣ���Ų�����
		*state = DoorOpen;                                //ת��dooropen״̬
	}
	if (IsDoorClosed(GetNearestFloor()))
		*state=Idle;                               //ת��idle״̬
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
