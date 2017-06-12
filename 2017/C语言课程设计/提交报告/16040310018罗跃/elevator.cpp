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
	int floor;
	bool up;
	floor = IdleWhatFloorToGoTo(&up);   
	if (floor > 0 && up)  //Event
	{
		SetMotorPower(1);  //Transition
		*state = MovingUp;  //����MovingDown״̬     
	}
	floor = IdleWhatFloorToGoTo(&up);
	if (floor>0 && !up)  //Event
	{
		SetMotorPower(-1);  //Transition
		*state = MovingDown;  //����MovingDown״̬            
	}
	if (GetOpenDoorLight())  //Event
	{
		SetDoor(GetNearestFloor(), true);  //���ѿ��Ű���
		SetOpenDoorLight(false);
		*state = DoorOpen;  //����DoorOpen״̬
	}
	if (GetCallLight(GetNearestFloor(), true))  //Event
	{
		SetDoor(GetNearestFloor(), true);  //���ѿ��Ű���
		SetCallLight(GetNearestFloor(), true, false);  //��������up/down����
		*state = DoorOpen;  //����DoorOpen״̬
	}
	if (GetCallLight(GetNearestFloor(), false))  //Event
	{
		SetDoor(GetNearestFloor(), true);   //���ѿ��Ű���
		SetCallLight(GetNearestFloor(), false, false);
		*state = DoorOpen;  //����DoorOpen״̬
	}
	if (GetCloseDoorLight())  //Event ���ѹ��ŵư���
	{
		SetCloseDoorLight(false);
		return;
	}
}

/* MovingUp״̬���� */
void StateMovingUp(int *state)
{
	int floor;
	floor = GoingUpToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)  //Event �ж��Ƿ񵽴�Ŀ��¥��
	{
		SetMotorPower(0);  //Transition  ֹͣ 
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;  
		if (GetNearestFloor() != Lib_FloorNum)  //Event  ��ȡ��ǰ¥��
			SetCallLight(GetNearestFloor(), true, false);  //��������up��ť
		else (SetCallLight(GetNearestFloor(), false, false));   //��������down��ť
	}
	SetPanelFloorLight(GetNearestFloor(), false);  //��������¥�㰴ť
	if (GetOpenDoorLight())  //Event
		SetOpenDoorLight(false);   //���ѿ��Ű���
	if (GetCloseDoorLight())  //Event   
		SetCloseDoorLight(false);  //���ѹ��Ű���
}

/* IMovingDown״̬���� */
void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)  //Event
	{
		SetMotorPower(0);  //Transition
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;  //����DoorOpen״̬
	} 
	if (GetCallLight(GetNearestFloor(), true))  //Event  
		SetCallLight(GetNearestFloor(), true, false);  //����down��ť
	else if (Lib_FloorNum)  //Event
	{
		SetCallLight(GetNearestFloor(), false, false);
	}
	SetPanelFloorLight(GetNearestFloor(), false);  //��������¥�㰴ť
	SetCallLight;
	if (Lib_FloorNum)   //Event
		SetCallLight;
	SetPanelFloorLight;
}

/* DoorOpen״̬���� */
void StateDoorOpen(int *state)
{
	int floor;
	floor = GetNearestFloor();
	if (GetCloseDoorLight())  //Event
	{
		SetDoor(floor, false);
		SetCloseDoorLight(false);
		*state = DoorClosing;   //����DoorClosing״̬
	}
	if (IsDoorOpen(floor))  //Event
	{
		SetDoor(floor, false);  //�Զ����й���
		*state = DoorClosing;  //����DoorClosing״̬
	}
	if (GetCloseDoorLight())  //Event
		SetOpenDoorLight(false);  //���ѿ��Ű�ť
}

/*DoorClosing״̬���� */
void StateDoorClosing(int *state)
{

	int floor;
	floor = GetNearestFloor();
	if (GetOpenDoorLight())  //Event
	{
		SetDoor(floor, true);
		SetOpenDoorLight(false);
		*state = DoorOpen;  //����DoorOpen״̬
	}
	if (GetCloseDoorLight())  //Event
		SetCloseDoorLight(false);  //���ѹ��Ű�ť
	if (IsBeamBroken())  //Event �Ƿ����̽�⵽�ڵ���
	{
		SetDoor(floor, true);
		*state = DoorOpen;  //����DoorOpen״̬
	}
	if (IsDoorClosed(floor))  //Event
		*state = Idle;  //����Idle״̬
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
