#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ElevatorLib.h"

/**********************************************
 * Idle״̬������ֹͣ��ĳ¥�㣬���ǹرյģ����ھ�ֹ״̬���ȴ�����¼��ķ������Ӷ�ת������һ��״̬��
 **********************************************/
void StateIdle(int *state)
{   int floor; bool up;
      floor = IdleWhatFloorToGoTo(&up);
	  
	if (floor > 0 && up)
	 {
	SetMotorPower(1);
	 *state = MovingUp;
	 printf("�Ӿ�ֹ״̬����");
	 }                                              
    if (floor > 0 && !up)
	{
    SetMotorPower(-1);
	*state = MovingDown;
	printf("�Ӿ�ֹ״̬�½�");                 
	 } 
	if (GetOpenDoorLight())
	{
	SetDoor(floor, true);
	SetOpenDoorLight(false);
	*state = DoorOpen;
	printf("�����ڰ�ť����");
	}
    else if (GetCallLight(GetNearestFloor(), true) || GetCallLight(GetNearestFloor(), false))
	{
	SetDoor(floor, true);
	SetCallLight( GetNearestFloor(), false, false);
	SetCallLight( GetNearestFloor(), true, false);
	*state = DoorOpen;
	printf("��������п���");
	}                                         
	if (GetCloseDoorLight()) 
	{
	SetCloseDoorLight(false); 
	return; 
	}                                            
}

void StateMovingUp(int *state)
{
	int floor;
	floor = GoingUpToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
	SetMotorPower(0);
	SetOpenDoorLight(false);
	SetCloseDoorLight(false);
	SetCallLight(GetNearestFloor(), true, false);
	SetCallLight(GetNearestFloor(), false, false);
	SetPanelFloorLight(floor, false);
	SetMotorPower(0);
	SetDoor(floor, true);
	GetOpenDoorLight();
	GetCloseDoorLight();
	*state = DoorOpen;
	printf("������״̬����");
	}                                            

}

void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
	SetOpenDoorLight(false);
	SetCloseDoorLight(false);
	SetCallLight(GetNearestFloor(), true, false);
	SetCallLight(GetNearestFloor(), false, false);
	SetPanelFloorLight(GetNearestFloor(), false);
	SetMotorPower(0);
	SetDoor(floor, true);
	GetOpenDoorLight();
	GetCloseDoorLight();
    *state = DoorOpen;
	printf("���½�״̬����");
	}                                          
}
void StateDoorOpen(int *state)
{
int floor = GetNearestFloor();
if (GetCloseDoorLight()||IsDoorOpen(floor))
{
   SetCloseDoorLight(false);
   SetOpenDoorLight(false);
   SetDoor(floor, false);
   *state = DoorClosing;  
   printf("�ӿ��ŵ�����");
}                                                    
	

}

void StateDoorClosing(int *state)
{
	int floor = GetNearestFloor();
	if (GetOpenDoorLight())
	{
	SetOpenDoorLight(false);
	SetCloseDoorLight(false);
	SetDoor(floor, true);
	*state = DoorOpen;
	printf("�ӹ��ŵ�����");
	}                                         
	if (IsBeamBroken())
	{
	SetDoor(floor, true);
	*state = DoorOpen;
	printf("��⵽���￪��");
	}
	if (IsDoorClosed(floor))
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
