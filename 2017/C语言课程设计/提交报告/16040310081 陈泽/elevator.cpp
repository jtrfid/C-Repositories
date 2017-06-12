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
	int floor;
	bool up;
	floor = IdleWhatFloorToGoTo(&up);  // ��̬���(��⽫Ҫ����Ŀ��¥��)
	if (floor > 0 && up)
	{
	SetMotorPower(1);
	*state = MovingUp;                                // ��Ǩ�ɾ�ֹ����MovingUp״̬
	}
	floor = IdleWhatFloorToGoTo(&up);
	if (floor > 0 && !up)
	{
	SetMotorPower(-1); 
	*state = MovingDown;                              // ��Ǩ�ɾ�ֹ����MovingDown״̬
	}
	if (GetOpenDoorLight())                               //�����ڿ��ż�
	{
	SetDoor(GetNearestFloor(), true);                 //����
	SetOpenDoorLight(false);                          //�رտ��ż�
	*state = DoorOpen;                                //��Ǩ�ɾ�ֹ����DoorOpen״̬
	}
	if (GetCallLight(GetNearestFloor(), true))            //��������а�ť
	{
	SetDoor(GetNearestFloor(), true);                 
	SetCallLight(GetNearestFloor(), true, false);    
	*state = DoorOpen;                                //��Ǩ�ɾ�ֹ����DoorOpen״̬
	}
	if (GetCallLight(GetNearestFloor(), false))           //��������а�ť
	{
		SetDoor(GetNearestFloor(), true);                 
		SetCallLight(GetNearestFloor(),false, false);     
		*state = DoorOpen;                                //��Ǩ�ɾ�ֹ����DoorOpen״̬  
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);                         //�رհ���
		return;                        
	}
}

void StateMovingUp(int *state)
{
	int floor;
	floor = GoingUpToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
	SetMotorPower(0);                                      //ֹͣ�˶�
	SetDoor(GetNearestFloor(), true);                      //����
	*state = DoorOpen;                                     //��Ǩ��DoorClosing����DoorOpen״̬
	if (GetNearestFloor() != Lib_FloorNum)
	SetCallLight(GetNearestFloor(), true, false);      //�رպ��а�ť
	else (SetCallLight(GetNearestFloor(), false, false));  //�رպ��а�ť
	}
	SetPanelFloorLight(GetNearestFloor(),false);               //�رպ��а�ť    
	if (GetOpenDoorLight())
		SetOpenDoorLight(false);                              //�رպ��а�ť
	if (GetCloseDoorLight())
		SetCloseDoorLight(false);                             //�رպ��а�ť
}

void StateMovingDown(int *state)
{
int floor;
floor = GoingDownToFloor();
if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
SetMotorPower(0);                                         //ֹͣ�˶�
SetDoor(GetNearestFloor(), true);                        //����
*state = DoorOpen;                                     //��Ǩ����DoorOpen״̬
if (GetNearestFloor() != 1)
	
SetCallLight(GetNearestFloor(), false, false);           //�ر�������а�ť
else( SetCallLight(1, true, false));                        //�ر�������а�ť
	}
SetPanelFloorLight(GetNearestFloor(), false);             //�ر�������а�ť      
if (GetOpenDoorLight())
	SetOpenDoorLight(false);                              //�ر����ں��а�ť
	SetCloseDoorLight(false);                             //�ر����ں��а�ť
}

void StateDoorOpen(int *state)
{
int floor;
floor = GetNearestFloor();                               //��̬���(��⽫Ҫ����Ŀ��¥��)
if (GetCloseDoorLight())
	{
SetDoor(floor,false);                                //����
SetCloseDoorLight(false);                            
*state = DoorClosing;                                //��Ǩ����DoorClosing״̬
	}
if (IsDoorOpen(floor))
	{
SetDoor(floor,false);                               //�Զ����ϵͳ���Զ��ر�
*state = DoorClosing;                               //��Ǩ����DoorClosing״̬
	}
	if (GetOpenDoorLight())
		SetOpenDoorLight(false);                       //�ر����ں��а�ť
}

void StateDoorClosing(int *state)
{int floor;
	floor = GetNearestFloor();                        //��̬���(��⽫Ҫ����Ŀ��¥��)
	if (GetOpenDoorLight())
	{
		SetDoor(floor, true);                         //����
		SetOpenDoorLight(false);                        //�ر����ں��а�ť
		*state = DoorOpen;                            //��Ǩ����DoorOpen״̬
	}
	if (GetCloseDoorLight())
		SetCloseDoorLight(false);                      //�ر����ں��а�ť
	if (IsBeamBroken())                                 //����ϰ������̽��
	{
		SetDoor(floor, true);                           //���޷��ر�
		*state = DoorOpen;                              //��Ǩ����DoorOpen״̬
	}
	if (IsDoorClosed(floor))
		*state = Idle;                                  //��Ǩ����Idle״̬
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
