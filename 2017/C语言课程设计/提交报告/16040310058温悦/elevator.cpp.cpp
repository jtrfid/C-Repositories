#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ElevatorLib.h"

/**********************************************
 * Idle״̬������ֹͣ��ĳ¥�㣬���ǹرյģ����ھ�ֹ״̬���ȴ�����¼��ķ������Ӷ�ת������һ��״̬��
 **********************************************/
/*Idle״̬����*/
void StateIdle(int *state)
{
	int floor; bool up;
	floor = IdleWhatFloorToGoTo(&up);
	if (floor > 0 && up)//Event �������¥�㰴ť��������а�ť
	{
		SetMotorPower(1);//Transition
		*state = MovingUp;
		printf("transition:from Idle to MovingUp.\n");
		SetCallLight(floor, true, false);//�ر��������up��
		SetCallLight(floor, false, false);//�ر��������down��

	}
	if (floor > 0 && !up)
	{
		SetMotorPower(-1);//Transition
		*state = MovingDown;
		printf("transition:from Idle to MovingDown.\n");
		SetCallLight(floor, true, false);//�ر��������up��
		SetCallLight(floor, false, false);//�ر��������down��
	}
	if (GetOpenDoorLight())//Event ������ڿ��Ű�ť
	{
		SetDoor(floor,true);//Transition
		*state = DoorOpen;
		printf("transition:from Idle to DoorOpen.\n");
		SetOpenDoorLight(false);//���ѿ��Ű�ť
	}
	floor = GetNearestFloor();
	if ((up&&GetCallLight(floor,true))||(!up&&GetCallLight(floor,false)))//Event �������up/down��ť
	{
		SetDoor(floor,true);//Transition
		*state = DoorOpen;
		printf("transition:from Idle to DoorOpen.\n");
		SetCallLight(floor, true, false);//��������up��ť
		SetCallLight(floor, false, false);//��������down��ť
	}
	if (GetCloseDoorLight())//Event
	{
		SetCloseDoorLight(false);//Transition���ѹ��Ű�ť
		return;
	}
}
void StateMovingUp(int *state)
{
	int floor;
	floor = GoingUpToFloor();//Ŀ��¥��
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)//Event ����Ŀ��¥��
	{
		SetMotorPower(0);
		SetDoor(floor,true);//Transition
		*state = DoorOpen;
		printf("transition:from Moving Up to DoorOpen.\n");
		SetCallLight(GetNearestFloor(), true, false);//��������up��ť
		if (GetNearestFloor() == Lib_FloorNum)
			SetCallLight(GetNearestFloor(), false, false);//��������down��ť
		SetPanelFloorLight(floor, false);//��������¥�㰴ť
	}
	/*��ȫ���ã��޶��������ѿ�/���Ű�ť*/
	if(GetOpenDoorLight())
	SetOpenDoorLight(false);
	if(GetCloseDoorLight())
	SetCloseDoorLight(false);
}
void StateMovingDown(int *state)
{
	int floor;
	floor = GoingUpToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)//Event ����Ŀ��¥��
	{
		SetMotorPower(0);//ֹͣ
		SetDoor(floor,true);//Transition
		*state = DoorOpen;
		printf("transition:from Moving Down to DoorOpen.\n");
		SetCallLight(GetNearestFloor(), false, false);//��������down��ť
		if (GetNearestFloor()==1)
			SetCallLight(GetNearestFloor(), true, false);//��������up��ť
		SetPanelFloorLight(floor,false);//���ѵ�ǰ¥�㰴ť
	}
	/*��ȫ�����޶��������ѿ�/���Ű�ť*/
	if (GetOpenDoorLight())
		SetOpenDoorLight(false);
	if (GetCloseDoorLight())
		SetCloseDoorLight(false);
}
void StateDoorOpen(int *state)
{
	int floor;
	floor = GetNearestFloor();
	if (GetCloseDoorLight())//Event
	{
		SetDoor(floor,false);//Transition
		*state = DoorClosing;
		printf("transition:from DoorOpen to DoorClosing.\n");
		SetCloseDoorLight(false);//���ѹ��Ű�ť
	}
	if (IsDoorOpen(floor))//Event
	{
		SetDoor(floor, false);//Transition���Ž������Զ�����
		*state = DoorClosing;
		printf("transition:from DoorOpen to DoorClosing.\n");
	}
	if (GetOpenDoorLight())
	{
		SetOpenDoorLight(false);//���ڿ��ţ����˿��ŵƣ��޶������ѿ��Ű�ť
	}
}
void StateDoorClosing(int *state)
{
	int floor;
	floor = GetNearestFloor();
	if (GetOpenDoorLight())//Event
	{
		SetDoor(floor,true);//Transition���ڹ��ţ����˿��ŵƣ�ת������
		*state = DoorOpen;
		printf("transition:from Doorclosing to Dooropen.\n");
		SetOpenDoorLight(false);//���ѿ��Ű�ť
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);//���ڹ��ţ����˹��ŵƣ��޶��������ѹ��Ű�ť
	}
	if (IsBeamBroken())//Event̽�⵽�ϰ�
	{
		SetDoor(floor,true);//Transition����
		*state = DoorOpen;
		printf("transition:from Doorclosing to Dooropen.\n");
	}
	if (IsDoorClosed(floor))//Event���Ž���
	{
		*state = Idle;
		printf("transition:from Doorclosing to Idle.\n");
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
