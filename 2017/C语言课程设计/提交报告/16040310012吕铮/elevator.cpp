#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ElevatorLib.h"

/**********************************************
 * Idle״̬������ֹͣ��ĳ¥�㣬���ǹرյģ����ھ�ֹ״̬���ȴ�����¼��ķ������Ӷ�ת������һ��״̬��
 **********************************************/
bool up;
void StateIdle(int *state)
{
	int floor; bool up;
	floor = IdleWhatFloorToGoTo(&up);  // Event
	if (floor > 0 && up)
	{
		SetMotorPower(1);
		*state = MovingUp;                                // ����MovingUp״̬
	}
	floor = IdleWhatFloorToGoTo(&up);
	if (floor > 0 && !up)
	{
		SetMotorPower(-1); 
		*state = MovingDown;                              // ����MovingDown״̬
	}
	if (GetOpenDoorLight())                               //����
	{
		SetDoor(GetNearestFloor(), true);                 //����
		SetOpenDoorLight(false);                          //���ѿ��ż�
		*state = DoorOpen;                                //����DoorOpen״̬
	}
	if (GetCallLight(GetNearestFloor(), true))            //����
	{
		SetDoor(GetNearestFloor(), true);                 //����
		SetCallLight(GetNearestFloor(), true, false);     //�ص�
		*state = DoorOpen;                                //����DoorOpen״̬
	}
	if (GetCallLight(GetNearestFloor(), false))           //����
	{
		SetDoor(GetNearestFloor(), true);                 //����
		SetCallLight(GetNearestFloor(),false, false);     //�ص�
		*state = DoorOpen;                                //����DoorOpen״̬  
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);                         //���Ѱ���
		return;                        
	}
}

void StateMovingUp(int *state)
{
	int floor;
	floor = GoingUpToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);                                      //ֹͣ
		SetDoor(GetNearestFloor(), true);                      //����
		*state = DoorOpen;                                     //����DoorOpen״̬
		if (GetNearestFloor() != Lib_FloorNum)
			SetCallLight(GetNearestFloor(), true, false);      //��������up��
		else (SetCallLight(GetNearestFloor(), false, false));  //���Ѷ���down��
	}
	SetPanelFloorLight(GetNearestFloor(),false);               //�������ڸ�¥�㰴��        
	if (GetOpenDoorLight())
		SetOpenDoorLight(false);                              //�������ڿ��ŵ�
	if (GetCloseDoorLight())
		SetCloseDoorLight(false);                             //�������ڹ��ŵ�
}

void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);                                     //ֹͣ
		SetDoor(GetNearestFloor(), true);                     //����
		*state = DoorOpen;                                    //����DoorOpen״̬
		if (GetNearestFloor() != 1)
			SetCallLight(GetNearestFloor(), false, false);    //��������up��
		else SetCallLight(1, true, false);                                 //����1��up��
	}
	SetPanelFloorLight(GetNearestFloor(), false);             //�������ڸ�¥�㰴��        
	if (GetOpenDoorLight())
		SetOpenDoorLight(false);                              //�������ڿ��ŵ�
	if (GetCloseDoorLight())
		SetCloseDoorLight(false);                             //�������ڹ��ŵ�
}

void StateDoorOpen(int *state)
{
	int floor;
	floor = GetNearestFloor();                               //��ȡ��ǰ¥��
	if (GetCloseDoorLight())
	{
		SetDoor(floor,false);                                //����
		SetCloseDoorLight(false);                            //�������ڹ��ŵ�
		*state = DoorClosing;                                //����DoorClosing״̬
	}
	if (IsDoorOpen(floor))
	{
		SetDoor(floor,false);                               //�Զ�����
		*state = DoorClosing;                               //����DoorClosing״̬
	}
	if (GetOpenDoorLight())
		SetOpenDoorLight(false);                            //�������ڿ��ŵ�
}

void StateDoorClosing(int *state)
{
	int floor;
	floor = GetNearestFloor();                               //��ȡ��ǰ¥��
	if (GetOpenDoorLight())
	{
		SetDoor(floor, true);                                //����
		SetOpenDoorLight(false);                             //�������ڿ��ŵ�
		*state = DoorOpen;                                   //����DoorOpen״̬
	}
	if (GetCloseDoorLight())
		SetCloseDoorLight(false);                            //�������ڹ��ŵ�
	if (IsBeamBroken())                                      //����̽��
	{
		SetDoor(floor, true);                                //�Զ�����
		*state = DoorOpen;                                   //����DoorOpen״̬
	}
	if (IsDoorClosed(floor))
		*state = Idle;                                       //����Idle״̬
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
