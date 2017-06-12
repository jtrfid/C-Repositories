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
	floor=IdleWhatFloorToGoTo(&up);//ȷ��Ŀ��¥��
	if (floor>0&&up)//���Ҫ��������
	{
		
		SetMotorPower(1);
		*state = MovingUp;
	}
	if (floor>0&&!up)//���Ҫ��������
	{ 
		SetMotorPower(-1);
		*state = MovingDown;
	}
	if (GetOpenDoorLight())//���ڰ��˿��ŵ�
	{
		SetOpenDoorLight(false);//���ѿ��ŵ�
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;
	}
	if (GetCallLight(GetNearestFloor(), true))//������ⰴ������
	{
		SetDoor(GetNearestFloor(), true);
		SetCallLight(GetNearestFloor(), true, false);
		*state = DoorOpen;
	}
	if (GetCallLight(GetNearestFloor(), false))//������ⰴ������
	{
		SetDoor(GetNearestFloor(), true);
		SetCallLight(GetNearestFloor(), false, false);
		*state = DoorOpen;
	}
	if (GetCloseDoorLight())//���Źر�ʱ�������ڹ��ż�
	{ 
		SetCloseDoorLight(false); 
	return;
	}
}

void StateMovingUp(int *state)
{
	int floor;
	floor = GoingUpToFloor();
	SetOpenDoorLight(false);
	SetCloseDoorLight(false);
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)//����Ƿ�Ŀ��¥��
	{
		SetMotorPower(0);
		SetDoor(GetNearestFloor(), true);
		SetCallLight(GetNearestFloor(), true, false);
		SetCallLight(GetNearestFloor(), false, false);
		SetPanelFloorLight(GetNearestFloor(), false);
		if (GetNearestFloor() == Lib_FloorNum)//������߲㣬��������down��ť
		    SetCallLight(GetNearestFloor(), false, false);
		*state = DoorOpen;
	}

}

void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();
	SetOpenDoorLight(false);
	SetCloseDoorLight(false);
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(GetNearestFloor(), true);
		SetCallLight(GetNearestFloor(), false, false);
		SetCallLight(GetNearestFloor(), true, false);
		SetPanelFloorLight(GetNearestFloor(), false);
		*state = DoorOpen;
	}
}      

void StateDoorOpen(int *state)
{
	if (GetCloseDoorLight())
	{
		SetDoor(GetNearestFloor(), false);
		SetCloseDoorLight(false);
		*state = DoorClosing;
	}
	if (IsDoorOpen(GetNearestFloor()))//�����ǰ¥�����Ѿ���
	{
		SetDoor(GetNearestFloor(), false);//�Զ����й���
		*state = DoorClosing;
	}
	if (GetOpenDoorLight())//������ڿ���ʱ�����˿��ż�
		SetOpenDoorLight(false);//���ѿ��ż�
}

void StateDoorClosing(int *state)
{
	if (GetOpenDoorLight() )//���ڹ���ʱ���˿��ż�
	{
		SetDoor(GetNearestFloor(), true);
		SetOpenDoorLight(false);
		*state = DoorOpen;
	}
	if (GetCloseDoorLight() )//���ڹ���ʱ���˹��ŵ�
		SetCloseDoorLight(false);
	if (IsBeamBroken())//��������ϰ���
	{
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;
	}
	if (IsDoorClosed(GetNearestFloor()) )//����Źغ���
		*state = Idle;


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
