#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ElevatorLib.h"

/**********************************************
 * Idle״̬������ֹͣ��ĳ¥�㣬���ǹرյģ����ھ�ֹ״̬���ȴ�����¼��ķ������Ӷ�ת������һ��״̬��
 **********************************************/
bool up;
void StateIdle(int *state)             //����״̬����
{
	int floor;
    bool up;
	floor = IdleWhatFloorToGoTo(&up);  // �ж�Ŀ��¥���Լ��������Ĳ�������

     if (floor > 0 && up)
	   {
		SetMotorPower(1);
		*state = MovingUp;                                // MovingUp״̬
	   }
	    floor = IdleWhatFloorToGoTo(&up);
      if (floor > 0 && !up)
	   {
		SetMotorPower(-1);
	 	*state = MovingDown;                              // MovingDown״̬
	   }
	  if (GetOpenDoorLight())                               //����״̬�¿���
	   {
		 SetDoor(GetNearestFloor(), 1);
		 SetOpenDoorLight(0);
		 *state = DoorOpen;                                //DoorOpen״̬
	   }
	  if (GetCallLight(GetNearestFloor(), 1))            //����
	   {
		SetDoor(GetNearestFloor(), 1);                 //����
		SetCallLight(GetNearestFloor(), 1, 0);     //�ص�
		*state = DoorOpen;
	   }
	  if (GetCallLight(GetNearestFloor(), 0))           //�������
       {
		SetDoor(GetNearestFloor(), 1);                 //����
		SetCallLight(GetNearestFloor(),0, 0);     //�������
		*state = DoorOpen;
	   }
	  if (GetCloseDoorLight())
	   {
		SetCloseDoorLight(0);                         //���Ѹù��ܰ�������ֹ��ѭ��
		return;
	   }
}

void StateMovingUp(int *state)     //��������״̬����
{
	int floor;
	floor = GoingUpToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);                                      //���ֹͣ��ת������ͣ�µȺ���һ��ָ��
		SetDoor(GetNearestFloor(), true);                      //����
		*state = DoorOpen;                                     //����DoorOpen״̬
		if (GetNearestFloor() != Lib_FloorNum)
			SetCallLight(GetNearestFloor(), true, false);      //����������¥��
		else (SetCallLight(GetNearestFloor(), false, false));  //���Ѷ�����¥��
	}
	  SetPanelFloorLight(GetNearestFloor(),false);               //���������ƶ�¥�㰴��
	  if (GetOpenDoorLight())
		   SetOpenDoorLight(false);
	  if (GetCloseDoorLight())
		   SetCloseDoorLight(false);
}

void StateMovingDown(int *state)        //�����½�״̬����
{
	int floor;
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);                                     //���ֹͣ��ת������ͣ�µȺ���һ��ָ��
		SetDoor(GetNearestFloor(), true);                     //����
		*state = DoorOpen;                                    //����DoorOpen״̬
		if (GetNearestFloor() != 1)
			SetCallLight(GetNearestFloor(), false, false);
		else SetCallLight(1, true, false);                                 //����1��up��
	}
	SetPanelFloorLight(GetNearestFloor(), false);             //�������ڸ�¥�㰴��
	if (GetOpenDoorLight())
		SetOpenDoorLight(false);                              //���ڵ���
	if (GetCloseDoorLight())
		SetCloseDoorLight(false);
}

void StateDoorOpen(int *state)             //����״̬����
{
	int floor;
	floor = GetNearestFloor();                               //���س����ȡ��ǰ¥��
	if (GetCloseDoorLight())
	 {
		SetDoor(floor,false);                                //����
		SetCloseDoorLight(false);
		*state = DoorClosing;                                //����DoorClosing״̬
	 }
	if (IsDoorOpen(floor))
	 {
	 	SetDoor(floor,false);                               //����
		*state = DoorClosing;                               //����DoorClosing״̬

	 }
	if (GetOpenDoorLight())
		SetOpenDoorLight(false);                            //�������ڿ��ŵ�
}

void StateDoorClosing(int *state)         //����״̬����
{
	int floor;
	floor = GetNearestFloor();                               //��ȡ��ǰ¥��
	if (GetOpenDoorLight())//���ֹͣ��ת������ͣ�µȺ���һ��ָ��
	{
		SetDoor(floor, true);                                //����
		SetOpenDoorLight(false);                             //�������ڿ��ŵ�
		*state = DoorOpen;                                   //����DoorOpen״̬
	}
	if (GetCloseDoorLight())
		SetCloseDoorLight(false);                            //�������ڹ��ŵ�
	if (IsBeamBroken())                                      //����̽�⺯��
	{
		SetDoor(floor, true);
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
