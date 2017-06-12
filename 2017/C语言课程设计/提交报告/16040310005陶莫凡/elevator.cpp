#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ElevatorLib.h"

/**********************************************
 * Idle״̬������ֹͣ��ĳ¥�㣬���ǹرյģ����ھ�ֹ״̬���ȴ�����¼��ķ������Ӷ�ת������һ��״̬��
 **********************************************/
void StateIdle(int *state)                                                       //Idle״̬��
{   int floor; bool up;
      floor = IdleWhatFloorToGoTo(&up);
	if (floor > 0 && up)
	 {
		 SetMotorPower(1);
		 *state = MovingUp;
		 printf("��ldle��MovingUp");
	 }                                                                           //S1
    if (floor > 0 && !up)
	{
		 SetMotorPower(-1);
		 *state = MovingDown;
		 printf("��ldle��MovingDown");                                           //S2
	 } 
    if (GetOpenDoorLight()||GetCallLight(floor=GetNearestFloor(),true)||GetCallLight(floor=GetNearestFloor(),false))
	{
		 SetDoor(floor, true);
		 SetOpenDoorLight(false);
		 SetCallLight(floor=GetNearestFloor(), false, false);
		 SetCallLight(floor = GetNearestFloor(), true, false);
		 *state = DoorOpen;
		 printf("��ldle��DoorOpen");
	}                                                                            //S3
	if (GetCloseDoorLight()) 
	{
		SetCloseDoorLight(false); 
		return; 
		printf("��ldle��ldle");
	}                                                                            //S4
}

void StateMovingUp(int *state)                                                   //����״̬��
{
	int floor;
	floor = GoingUpToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(floor, true);
		SetCallLight(floor,true,false);
		SetCallLight(floor, false, false);
		SetPanelFloorLight(floor = GetNearestFloor(), false);
		printf("��MovingUp��DoorOpen");
		GetOpenDoorLight();
		GetCloseDoorLight();
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);                                         //D
		*state = DoorOpen;
	}                                                                             //S5                

}

void StateMovingDown(int *state)                                                 //�½�״̬��
{
	int floor;
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(floor, true);
		SetCallLight(floor, true, false);
		SetCallLight(floor, false, false);
		SetPanelFloorLight(floor=GetNearestFloor(), false);
		printf("��MovingDown��DoorOpen");
		GetOpenDoorLight();
		GetCloseDoorLight();
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);                                          //D
		*state = DoorOpen;
	}                                                                              //S6
}
void StateDoorOpen(int *state)                                                     //����״̬��
{
	int floor = GetNearestFloor();
	if (GetCloseDoorLight()||IsDoorOpen(floor))
	{
		SetDoor(floor, false);
		SetCloseDoorLight(false);
		SetOpenDoorLight(false);
		*state = DoorClosing; 
		printf("��DoorOpen��DoorClosing");                                         //S7
	}                                                    
	

}

void StateDoorClosing(int *state)
{
	int floor = GetNearestFloor();
	if (GetOpenDoorLight()||IsBeamBroken())
	{
		SetDoor(floor, true);
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);
		*state = DoorOpen;
		printf("��DoorClosing��DoorOpen");
	}                                                                              //S8
	if (IsDoorClosed(floor))
	{
		*state = Idle;
		printf("��DoorClosing��Idle");
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
