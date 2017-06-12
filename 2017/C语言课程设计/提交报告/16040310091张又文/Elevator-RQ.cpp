#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ElevatorLib.h"

/**********************************************
 ������ݷ������״̬����
 ѧ�ţ�16040310091
 ������������
 ���ڣ�2017.5.18
 **********************************************/
/*����״̬����*/
void StateIdle(int *state)
{
	int floor;
	bool up;
	floor = IdleWhatFloorToGoTo(&up);/*��̬��⣬bool up;*/
    if (GetNearestFloor() != 1)
	{
		AutoTo1Floor();
	}/*1�����ϣ�һ��ʱ���޶������Զ��½���1¥��*/
	if (GetPanelFloorLight(floor) && floor > GetNearestFloor())
	{
		SetMotorPower(1);
		*state = MovingUp;
	}
	else if (GetCallLight(floor, true) && floor > GetNearestFloor())
	{
		SetMotorPower(1);
		*state = MovingUp;
	}
	else if (GetCallLight(floor, false) && floor > GetNearestFloor())
	{
		SetMotorPower(1);
		*state = MovingUp;
	}
	else if (GetOpenDoorLight() || (GetCallLight(GetNearestFloor(), true) || GetCallLight(GetNearestFloor(), false)))
	{
		SetDoor(GetNearestFloor(), true);
		SetOpenDoorLight(false);
		*state = DoorOpen;
	}
	else if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);/*����*/
	}
	else if (GetCallLight(floor, true) && floor < GetNearestFloor())
	{
		SetMotorPower(-1);
		*state = MovingDown;
	}
	else if (GetCallLight(floor, false) && floor < GetNearestFloor())
	{
		SetMotorPower(-1);
		*state = MovingDown;
	}
	else if (GetPanelFloorLight(floor)&&floor < GetNearestFloor())
	{
		SetMotorPower(-1);
		*state = MovingDown;
	}
}/*Idle״̬������ֹͣ��ĳ¥�㣬���ǹرյģ����ھ�ֹ״̬���ȴ�����¼��ķ������Ӷ�ת������һ��״̬��*/
void StateMovingUp(int *state)
{
	int floor;
	floor = GoingUpToFloor();/*��̬���*/
	if (GetOpenDoorLight())
	{
		SetOpenDoorLight(false);/*����*/
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);/*����*/
	}
	if (floor - GetFloor() < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(floor, true);
		SetCallLight(floor,true,false);
		SetCallLight(floor, false, false);
		SetPanelFloorLight(floor,false);
		*state = DoorOpen;
	}/*����Ŀ��¥�㣬ֹͣ������     ��������up��ť; ������߲�Lib_FloorNum, ��������down��ť����������¥�㰴ť��*/
}/*Moving up״̬*/
void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();/*��̬���*/
	if (GetOpenDoorLight())
	{
		SetOpenDoorLight(false);/*����*/
	}
	if (GetCloseDoorLight())
	{
			SetCloseDoorLight(false);/*����*/
	}
	if (GetFloor() - floor < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(floor,true);
		SetPanelFloorLight(floor,false);
		SetCallLight(floor, true, false);
		SetCallLight(floor, false, false);
		*state = DoorOpen;
	}/*��moving up״̬����*/
}/*Moving down״̬*/
void StateDoorOpen(int *state)
{
	int floor;
	floor = GetNearestFloor();
	if (GetCloseDoorLight())/*ת������*/
	{
		SetDoor(floor,false);
		SetCloseDoorLight(false);
		*state = DoorClosing;
	}
	if (IsDoorOpen(floor))/*���Ž������Զ��������״̬*/
	{
		SetDoor(floor, false);
		*state = DoorClosing;
	}
}/*�����Ŵ�*/
void StateDoorClosing(int *state)
{
	int floor;
	floor = GetNearestFloor();
	if (GetOpenDoorLight())/*ת������*/
	{
		SetDoor(floor,true);
		*state = DoorOpen;
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
	}
	if (IsDoorClosed(floor))/*���Ž����󣬽���Idle״̬��*/
	{
		*state = Idle;
	}
}/*���ڹ���*/
/***********************************************
 * ����ѭ���� ״̬����ÿ��һ��ʱ��(�磬100ms)������һ�Σ��ɼ�ϵͳ������״̬
 ***********************************************/
void main_control(int *state)
{  
	if (IsElevatorRunning())
	{
		switch (*state)
		{
		case Idle:
			StateIdle(state);
			break;
		case MovingUp:
			StateMovingUp(state);
			break;
		case MovingDown:
			StateMovingDown(state);
			break;
		case DoorOpen:
			StateDoorOpen(state);
			break;
		case DoorClosing:
			StateDoorClosing(state);
			break;
		default:
			printf("û������״̬!!!\n");
		}
	}
}