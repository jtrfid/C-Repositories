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
		SetMotorPower(1);
		*state = MovingUp;
	}//����׼������
	else if (floor > 0 && !up)
	{
		SetMotorPower(-1);
		*state = MovingDown;
	}//����׼���½�
	if (GetOpenDoorLight())
	{
		SetDoor(floor, 1);
		SetOpenDoorLight(0);
		*state = DoorOpen;
	}// �����ڿ���
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(0);
		return;
	}//���ݹر�
}


void StateMovingUp(int *state)
{
	int floor; bool up;
	floor = GoingUpToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(floor, 1);
		*state = DoorOpen;
		if (fabs(GetFloor() - 3) < Lib_FloorTolerance)
			SetCallLight(floor, 0, 0);
		SetCallLight(floor, 1, 0);
		SetPanelFloorLight(floor, 0);
	}//����
	else {
		GetOpenDoorLight(); SetOpenDoorLight(0);
		GetCloseDoorLight(); SetCloseDoorLight(0);

	}//��ȫ���ã��޶���
}

void StateMovingDown(int *state)
{
	int floor; bool up;
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(floor,1);
		*state = DoorOpen;
		if (fabs(GetFloor() - 1) < Lib_FloorTolerance)
			SetCallLight(floor, 1, 0);
		SetCallLight(floor, 0, 0);
		SetPanelFloorLight(floor, 0);
		GetNearestFloor();
	}//�½�����
	else {
		GetOpenDoorLight(); SetOpenDoorLight(0);
		GetCloseDoorLight(); SetCloseDoorLight(0);
	}//�ص�


}

void StateDoorOpen(int *state)
{
	int floor; bool up;
	floor=GetNearestFloor();
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(0);
		SetDoor(floor, 0);
		*state = DoorClosing;
	}//���ڿ��ţ������ţ�����
	if (IsDoorOpen(floor))
	{
		SetDoor(floor, 0);
		*state = DoorClosing;
	}//�����ֱ꣬�ӹ���
	if (GetOpenDoorLight())
	{
		SetOpenDoorLight(0);
	}//����ʱ�������ţ��޶���

}

void StateDoorClosing(int *state)
{
	int floor; bool up;
	floor=GetNearestFloor();
	if (GetOpenDoorLight())
	{
		SetOpenDoorLight(0);
		SetDoor(floor, 1);
		*state = DoorOpen;
	}//���ڹ��ţ������ţ�����
	else if (GetCloseDoorLight())
	{
		SetCloseDoorLight(0);
	}//���ڹ��ţ������ţ��޶���
	else if (IsBeamBroken())
	{
		SetDoor(floor,1);
		*state = DoorOpen;
	}//�ſ��ж�����ס���Զ�����
	else if (IsDoorClosed(floor))
	{
		*state = Idle;
	}//���ź����


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
