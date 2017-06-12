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
	bool up;
	int floor;
	floor = IdleWhatFloorToGoTo(&up);//��̬��⽫Ҫ�����Ŀ��¥��
	if (floor>0 && up)
	{
		SetMotorPower(1);//��������¥�㰴ť����������״̬
		*state = MovingUp;
		printf("Transition:from Idle to Moving up\n");
	}
	if (floor>0 && !up)
	{
		SetMotorPower(-1);//��������¥�㰴ť�������½�״̬
		*state = MovingDown;
		printf("Transition:from Idle to Moving Down\n");
	}
	if (GetOpenDoorLight())
	{
		SetDoor(GetNearestFloor(), 1);
		SetOpenDoorLight(0);//�������ڿ��Ű�ť�����뿪��״̬
		*state = DoorOpen;
		printf("Transition:from Idle to Door Open\n");
	}
	if (GetCallLight(GetNearestFloor(), 1))
	{
		SetDoor(GetNearestFloor(), 1);
		SetCallLight(GetNearestFloor(), 1, 0); //�����������Ϻ��а�ť�����뿪��״̬
		*state = DoorOpen;
		printf("Transition:from Idle to Door Open\n");
	}
	if (GetCallLight(GetNearestFloor(), 0))
	{
		SetDoor(GetNearestFloor(), 1);
		SetCallLight(GetNearestFloor(), 0, 0);//����������а�ť�����뿪��״̬
		*state = DoorOpen;
		printf("Transition:from Idle to Door Open\n");
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(0);//�������ڹ��Ű�ť������Ӧ
		return;
		printf("Transition:from Idle to Idle\n");
	}
}

void StateMovingUp(int *state)
{
	int floor;
	floor = GoingUpToFloor();//��̬��⣬Ŀ��¥��
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)//����Ŀ��¥��
	{
		SetMotorPower(0);//�رյ��
		SetDoor(floor, 1);//����
		SetPanelFloorLight(floor, 0);//��������¥�㰴ťΪ��ǰ¥��İ�ť
		SetCallLight(floor, 1, 0);//���ѵ�ǰ¥���������Ϻ��а�ť
		if (floor == Lib_FloorNum)
		{
			SetCallLight(floor, 0, 0);//������߲㣬������߲��������º��а�ť
		}
		*state = DoorOpen;//���뿪��״̬
		printf("Transition:from Moving up to Door Open\n");
	}
	if (GetOpenDoorLight() || GetCloseDoorLight())
	{
		SetOpenDoorLight(0);//�˶������У����ڿ����Ű�ťʧЧ
		SetCloseDoorLight(0);
		printf("Transition:from Moving Up to Moving Up\n");
	}
}

void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();//��̬���Ŀ��¥��
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)//����Ŀ��¥��
	{
		SetMotorPower(0);//�رյ��
		SetDoor(floor, 1);//����
		SetCallLight(floor, 0, 0);//���ѵ�ǰ¥���������º��а�ť
		if (floor == 1)
			SetCallLight(floor, 1, 0);//����һ�㣬����һ���������Ϻ��а�ť
		SetPanelFloorLight(floor, 0);//��������¥�㰴ťΪ��ǰ¥��İ�ť
		*state = DoorOpen;//���뿪��״̬
		printf("Transition:from Moving Down to Door Open\n");
	}
	if (GetOpenDoorLight() || GetCloseDoorLight())
	{
		SetOpenDoorLight(0);
		SetCloseDoorLight(0);//�˶�״̬�����ڿ����Ű�ťʧЧ
		printf("Transition:from Moving Down to Moving Down\n");
	}
}

void StateDoorOpen(int *state)
{
	int floor;
	floor = GetNearestFloor();//��̬��⵱ǰ¥��
	if (GetCloseDoorLight())//�����Ű�ť
	{
		SetDoor(floor, 0);//�رյ�����
		SetCloseDoorLight(0);//���ѹ��Ű�ť
		*state = DoorClosing;//�������״̬
		printf("Transition:from Door Open to Door Closing\n");
	}
	if (IsDoorOpen(floor))//���Ž�����
	{
		SetDoor(floor, 0);//�Զ����й���
		*state = DoorClosing;//�������״̬
		printf("Transition:from Door Open to Door Closing\n");
	}
	if (GetOpenDoorLight())//���鿪�Ű�ť
	{
		SetOpenDoorLight(0);//���ڿ��ţ����Ű�ťʧЧ
		printf("Transition:from Door Open to Door Open\n");
	}
}

void StateDoorClosing(int *state)
{
	int floor;
	floor = GetNearestFloor();//��̬��⵱ǰ¥��
	if (GetOpenDoorLight())//���鿪�Ű�ť
	{
		SetDoor(floor, 1);//����
		SetOpenDoorLight(0);//���ѿ��Ű�ť
		*state = DoorOpen;//���뿪��״̬
		printf("Transition:from Door Closing to Door Open\n");
	}
	if (GetCloseDoorLight())//������Ű�ť
	{
		SetCloseDoorLight(0);//���ڹ��ţ����Ű�ťʧЧ
		printf("Transition:from Door Closing to Door Closing\n");
	}
	if (IsBeamBroken())//����Ƿ����ϰ���
	{
		SetDoor(floor, 1);//����
		*state = DoorOpen;//���뿪��״̬
		printf("Transition:from Door Closing to Door Open\n");
	}
	if (IsDoorClosed(floor))//���Ž���
	{
		*state = Idle;//����ldle״̬
		printf("Transition:from Door Closing to  Idle\n");
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


