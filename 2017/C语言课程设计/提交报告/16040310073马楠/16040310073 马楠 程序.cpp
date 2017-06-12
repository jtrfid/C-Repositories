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
	floor = IdleWhatFloorToGoTo(&up);//��̬��⽫Ҫ�����Ŀ��¥��
	if (floor>0 && up)//Event
	{
		SetMotorPower(1);//Transition
		*state = MovingUp;//��������״̬
		printf("Transition:from Idle to MovingUp.\n");
	}
	else if (floor>0 && !up)//Event
	{
		SetMotorPower(-1);//Transition
		*state = MovingDown;//�����½�״̬
		printf("Transition:from Idle to MovingDown.\n");
	}
	else if (GetOpenDoorLight())//���ڿ��Ű�ť����
	{
		SetDoor(GetNearestFloor(), true);//�ڵ�ǰ�㿪��
		SetOpenDoorLight(false);//���ѵ����Ű�ť
		*state = DoorOpen;//���뿪��״̬
		printf("Transition:from Idle to DoorOpen.\n");
	}
	else if (GetCallLight(floor, up))//ĳ��������б�����
	{
		SetDoor(floor, true);//����Ŀ��¥�㣬������
		SetCallLight(GetNearestFloor(), up, false);//�ڵ�ǰ¥�����ѵ�������а�ť
		*state = DoorOpen;//���뿪��״̬
		printf("Transition: from Idle to DoorOpen.\n");
	}
	else if (GetCloseDoorLight())//���ڹ��Ű�ť������
	{
		SetCloseDoorLight(false);//���ѵ����Ű�ť����ֹ��һ�����ظ�����
		return;
	}
}
/*MovingUp״̬����*/
void StateMovingUp(int *state)
{
	int floor;
	floor = GoingUpToFloor();//��̬��⽫Ҫ�����¥��
	if (fabs(GetFloor() - floor)<Lib_FloorTolerance)//�ж��Ƿ񵽴�Ŀ��¥��
	{
		SetMotorPower(0);//����ֹͣ����
		SetDoor(floor, true);//������¥�㿪��
		SetPanelFloorLight(floor, false);//��������¥�㰴ťΪ��ǰ��İ�ť
		*state = DoorOpen;// ���뿪��״̬
		if (floor == 3)//����������¥�㣬�����ѵ������down��ť
			SetCallLight(floor, false, false);
		else
			SetCallLight(floor, true, false);//��������up��ť
		printf("Transition: from MovingUp to DoorOpen.\n");
	}
	else if (GetOpenDoorLight() || GetCloseDoorLight())//���ڿ�����Ű�ť������
	{
		SetOpenDoorLight(false);//�޶��������ѿ����Ű�ť
		SetCloseDoorLight(false);
	}
}
/*MovingDown״̬����*/
void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();//��̬��⽫Ҫ�����¥��
	if (fabs(GetFloor() - floor)<Lib_FloorTolerance)//�ж��Ƿ񵽴�Ŀ��¥��
	{
		SetMotorPower(0);//����ֹͣ����
		SetDoor(floor, true);//������¥�㿪��
		SetPanelFloorLight(floor, false);//��������¥�㰴ťΪ��ǰ��İ�ť
		*state = DoorOpen;//���뿪��״̬
		if (floor == 1)//���������һ��
			SetCallLight(floor, true, false);//��������up��ť
		else
			SetCallLight(floor, false, false);//��������down��ť
		printf("Transition: from MovingDown to DoorOpen.\n");

	}
	else if (GetOpenDoorLight() || GetCloseDoorLight())//���ڿ�����Ű�ť������
	{
		SetOpenDoorLight(false);//�޶��������ѿ����Ű�ť
		SetCloseDoorLight(false);
	}
}
/*DoorOpen״̬����*/
void StateDoorOpen(int *state)
{
	if (GetCloseDoorLight())//���ڹ��Ű�ť����
	{
		SetDoor(GetNearestFloor(), false);//�ڵ�ǰ¥�����
		SetCloseDoorLight(false);//�������ڹ��Ű�ť
		*state = DoorClosing;//�������״̬
		printf("Transition:from DoorOpen to DoorClosing.\n");
	}
	else if (IsDoorOpen(GetNearestFloor()))//���Ž�����
	{
		SetDoor(GetNearestFloor(), false);//�Զ����й���
		*state = DoorClosing;//�������״̬
		printf("Transition:from DoorOpen to DoorClosing.\n");
	}
	else if (GetOpenDoorLight())//���ڿ��Ű�ť����
	{
		SetOpenDoorLight(false);//���ѿ��Ű�ť
	}
}
/*DoorClosing״̬����*/
void StateDoorClosing(int *state)
{
	if (GetOpenDoorLight())//���ڿ��Ű�ť����
	{
		SetDoor(GetNearestFloor(), true);//�ڵ�ǰ¥�㿪��
		SetOpenDoorLight(false);//�������ڿ��Ű�ť
		*state = DoorOpen;//���뿪��״̬
		printf("Transition:from DoorClosing to DoorOpen.\n");
	}
	else if (GetCloseDoorLight())//���ڹ��Ű�ť����
	{
		SetCloseDoorLight(false);//���ѹ��Ű�ť
	}
	else if (IsBeamBroken())//�����⵽���ϰ���
	{
		SetDoor(GetNearestFloor(), true);//�ڵ�ǰ¥�㿪��
		*state = DoorOpen;//���뿪��״̬
		printf("Transition:from DoorClosing to DoorOpen.\n");
	}
	else if (IsDoorClosed(GetNearestFloor()))//���Ž�����
	{
		*state = Idle;//�������״̬
		printf("Transition:from DoorClosing to Idle.\n");
	}
}

/***********************************************
* ״̬����ÿ��һ��ʱ��(�磬100ms)������һ�Σ��ɼ�ϵͳ������״̬
***********************************************/
void main_control(int *state)
{
	if (IsElevatorRunning())  // ������������
	{
		switch (*state)
		{
		case Idle:
			// Idle״̬��һ��ʱ���޶������Զ���һ¥
			if (GetNearestFloor() != 1) {
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