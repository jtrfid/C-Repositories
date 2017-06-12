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
	int Floor,PresentFloor;
	bool up;
	Floor = IdleWhatFloorToGoTo(&up);
	PresentFloor = GetNearestFloor();
	if (Floor > 0 && up)
	{
		SetMotorPower(1);
		*state = MovingUp;
	}
	if (Floor > 0 && !up)
	{
		SetMotorPower(-1);
		*state = MovingDown;
	}
	if (GetOpenDoorLight())/*������ڿ��Ű�ť*/
	{
			SetDoor(PresentFloor, true);/*����*/
			SetOpenDoorLight(false);/*���ѿ��Ű�ť*/
			*state = DoorOpen;
	}
	if (GetCallLight(PresentFloor, up))/*�������up��ť*/
	{
		SetDoor(PresentFloor, true);/*����*/
			SetCallLight(PresentFloor, up, false);/*���Ѱ�ť*/
			*state = DoorOpen;
	}
	if (GetCallLight(PresentFloor, !up))/*�������down��ť*/
	{
		SetDoor(PresentFloor, true);/*����*/
			SetCallLight(PresentFloor, !up, false);/*���Ѱ�ť*/
			*state = DoorOpen;
	}
	if (GetCloseDoorLight())/*������ڹ��Ű�ť*/
	{
			SetCloseDoorLight(false);
			*state = DoorClosing;
			return;
	}
	
}
/*MovingUp״̬*/
void StateMovingUp(int *state)
{

	SetMotorPower(1);     /*�����ƶ�*/
	int Floor;
	Floor = GoingUpToFloor();
	if (fabs(GetFloor() - Floor) < Lib_FloorTolerance)/*����Ŀ��¥�㣬ֹͣ������*/
	{
		SetMotorPower(0);
		SetDoor(Floor, true);
		*state = DoorOpen;
		SetCallLight(Floor, true, false);/*��������up��ť*/
		SetPanelFloorLight(Floor, false);/*��������¥�㰴ť��*/
	}
	if (fabs(GetFloor() - 3) < Lib_FloorTolerance)/*����3¥*/
	{
		SetMotorPower(0);
		SetDoor(Floor, true);
		*state = DoorOpen;
		SetCallLight(3, false, false);/*��������down��ť*/
		SetPanelFloorLight(3, false);/*�������ڰ�ť*/
	}
	if (GetOpenDoorLight())     /*��������ŵ�*/
		SetOpenDoorLight(false);
	if (GetCloseDoorLight())
		SetCloseDoorLight(false);
}
void StateMovingDown(int *state)
{
	SetMotorPower(-1);     /*�����ƶ�*/
	int Floor;
	Floor = GoingDownToFloor();
	if (fabs(GetFloor() - Floor) < Lib_FloorTolerance)/*����Ŀ��¥�㣬ֹͣ������*/
	{
		SetMotorPower(0);
		SetDoor(Floor, true);
		*state = DoorOpen;
		SetCallLight(Floor, true, false);/*��������down��ť*/
		SetPanelFloorLight(Floor, false);/*�������ڰ�ť*/
	}
	if (fabs(GetFloor() - 1) < Lib_FloorTolerance)/*����1��*/
	{
		SetMotorPower(0);
		SetDoor(Floor, true);
		*state = DoorOpen;
		SetCallLight(1, false, false);/*��������up��ť*/
		SetPanelFloorLight(1, false);/*�������ڵ�ǰ¥��İ�ť*/
	}

	if (GetOpenDoorLight())     /*��������ŵ�*/
		SetOpenDoorLight(false);
	if (GetCloseDoorLight())
		SetCloseDoorLight(false);

	
}
/*DoorOpen״̬*/
void StateDoorOpen(int *state)
{
	int PresentFloor;
	PresentFloor = GetNearestFloor();
	if (GetCloseDoorLight())/*������ڹ��Ű�ť*/
	{
		SetDoor(PresentFloor, false);/*����*/
		SetCloseDoorLight(false);/*���ѹ��Ű�ť*/
		*state = DoorClosing;/*ת�����״̬*/
	}
	if (IsDoorOpen(PresentFloor))/*����ŵ�״̬���Զ�����*/
	{
		SetDoor(PresentFloor, false);
		*state = DoorClosing;
	}
	if (GetOpenDoorLight())
		SetOpenDoorLight(false);
}
/*DoorClosing״̬*/
void StateDoorClosing(int *state)
{
	int PresentFloor;
	PresentFloor = GetNearestFloor();
 if (GetOpenDoorLight())    /*������ڿ��ذ�ť*/
	{
		SetDoor(PresentFloor, true);
		SetCloseDoorLight(false);   /*���ѿ��ŵ�*/
		*state = DoorOpen;    /*����*/
	}
	if (GetCloseDoorLight())    /*�����ŵƣ����ڹ��ţ��޶��������ѹ��ŵ�*/
		SetCloseDoorLight(false);
	if (IsBeamBroken())   /*����Ƿ����ϰ���*/
	{
		SetDoor(PresentFloor, true);/*���ϰ������*/
		*state = DoorOpen;
	}
	if (IsDoorClosed(PresentFloor))
		*state = Idle;
}
/***********************************************
* ����ѭ���� ״̬����ÿ��һ��ʱ��(�磬100ms)������һ�Σ��ɼ�ϵͳ������״̬
***********************************************/
void main_control(int *state)
{
	if (IsElevatorRunning())  // ������������
	{
		switch (*state)
		{
		case Idle: 
			if (GetNearestFloor() != 1) 
			{
				AutoTo1Floor();// Idle״̬��һ��ʱ���޶������Զ���һ¥
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