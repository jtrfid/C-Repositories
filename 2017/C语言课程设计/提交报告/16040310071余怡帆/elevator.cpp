#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ElevatorLib.h"

/****************************************************************************************/
/*Idle״̬������ֹͣ��ĳ¥�㣬���ǹرյģ����ھ�ֹ״̬���ȴ�����¼��ķ������Ӷ�ת������һ��״̬��*/
/****************************************************************************************/
void StateIdle(int *state)
{
	if (PanelFloorLight()||CallLight())
	{
		int floor; bool up;
		floor=IdleWhatFloorToGoTo(&up);
		if (floor>0&&up)
		{
			SetMotorPower(1);
			*state = MovingUp;
			printf("Transition:form Idle to MovingUp");
		}
		else
		{
			SetMotorPower(-1);
			*state = MovingDown;
			printf("Transition:from Idle to MovingDown");

		}
		if (GetOpenDoorLight())/*�����ڰ�����*/
		{
			SetDoor(floor,1);
			SetOpenDoorLight(floor,0);
			*state = DoorOpen;
			printf("Transition:from Idle to DoorOpen");
		}
		if (GetCallLight())/*�����ⰴ����*/
		{
			SetDoor(floor,1);
			SetCallLight(floor,0);
			*state = DoorOpen;
			printf("Transition:from Idle to DoorOpen");
		}
		if (GetCloseDoorLight())
		{
			SetCloseDoorLight(false);
			return;
		}
	}
}

void StateMovingUp(int *state)
{
	if (PanelFloorLight() || CallLight())
	{
		int floor;
		floor = GoingUpToFloor();
		if(fabs(GetFloor()-floor)<Lib_FloorTolerance)
		{
			SetMotorPower(0);
			SetDoor(floor,1);
			*state = DoorOpen;
			printf("Transition:from MovingUp to DoorOpen");
			SetCallLight(floor,1,0);/*�������ϰ�ť*/
			if (Lib_FloorNum)
			{
				SetCallLight(floor,0,0);/*�������°�ť*/
				SetPanelFloorLight(floor,0);
				GetNearestFloor();
			}
        }

	}
	if (GetOpenDoorLight() || GetCloseDoorLight())
	{
		SetOpenDoorLight(0);
		SetCloseDoorLight(0);
	}

}
void StateMovingDown(int *state)
{
	if (PanelFloorLight() || CallLight())
	{
		int floor;
		floor = GoingDownToFloor();
		if (fabs(GetFloor() - floor) < Lib_FloorTolerance)/*��Ŀ��¥��*/
		{
			SetMotorPower(0);
			SetDoor(floor,1);
			*state = DoorOpen;
			printf("Transition:from MovingDown to DoorOpen");
			SetCallLight(floor,0,0);/*�����°�ť*/
        }
		if (Lib_FloorTolerance)/*1��*/
			SetUpLight(floor,0);/*�����ϰ�ť*/
		SetPanelFloorLight(floor);
		GetNearestFloor(floor);
	}
	if (GetOpenDoorLight() || GetCloseDoorLight())/*�½����̰�������*/
	{
		SetOpenDoorLight(0);
		SetCloseDoorLight(0);
	}

}

void StateDoorOpen(int *state)/*����״̬*/
{
	int floor;
	floor=GetNearestFloor();
	if (GetCloseDoorLight())/*�ֶ�����*/
	{
		SetDoor(floor,0);/*����*/
		SetCloseDoorLight(0);
		*state = DoorCloseing;
		printf("Transition:from DoorOpen to DoorCloseing");
    }
	if (IsDoorOpen())/*���Ž���*/
	{
		SetDoor(floor,0);/*����*/
		*state = DoorCloseing;
		printf("Transition:from DoorOpen to DoorCloseing");
	}
	if (GetOpenDoorLight())
		SetOpenDoorLight(0);
}

void StateDoorClosing(int *state)
{
	int floor;
	floor=GetNearestFloor();
	if (GetOpenDoorLight())/*����״̬�°����ż�*/
	{
		SetDoor(floor,1);/**/����
		SetOpenDoorLight(0);
		*state = DoorOpen;
		printf("Transition:from DoorCloseing to DoorOpen");
	}
	if (GetCloseDoorLight())/*����״̬ʱ������*/
		SetCloseDoorLight(0);
	if (IsBeamBroken())/*���ϰ���*/
	{
		SetDoor(floor,1);/*����*/
		*state = DoorOpen;
		printf("Transition:from DoorCloseing to DoorOpen");
	}
	if (IsDoorClosed())/*���Ž���*/
	{
		*state = Idel;
		printf("Transition:from DoorCloseing to Idel");

	}
}

 /***********************************************/
 /*״̬����ÿ��һ��ʱ�䱻����һ�Σ��ɼ�ϵͳ������״̬*/
 /**********************************************/
void main_control(int *state)
{  
	if (IsElevatorRunning()) /*������������*/ 
    {
		switch(*state)
		{
		case Idle:/*Idle״̬��һ��ʱ���޶������Զ���һ¥*/
			if(GetNearestFloor() !=1 ) {
				AutoTo1Floor();
			}
			StateIdle(state);
			break;
		case MovingUp:
			CancelTo1Floor(); /*����״̬��ȡ���Զ���һ¥*/ 
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
