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
	bool s;
	int floor_now = GetNearestFloor();
	floor = IdleWhatFloorToGoTo(&up);//Event

	if (floor>0 && up)
	{
		SetMotorPower(1);
		*state = MovingUp;//Transition
		printf("Transition:from Idles to MovingUp.\n");//s1
		return;

	}
	
	else if (floor >0 && !up)
	{
		SetMotorPower(-1);
		*state = MovingDown;//Transition
		printf("Transition��from Idle to MovingDown.\n");//s2
		return;

	}
	if (GetOpenDoorLight())//�������ڿ��Ű�ť
	{
		SetDoor(floor, true);
		*state = DoorOpen;//Transition
		printf("Transition:from Idle to Dooropen.\n");//s3
		return;
	}

	else if (GetCallLight(floor_now, true))//������UP��ť
	{
		*state = DoorOpen;
		printf("Transition:from Idle to Dooropen.\n");
		return;
	}


	else if (GetCallLight(floor_now, false))//������Down��ť
	{
		*state = DoorOpen;
		printf("Transition:from Idle to Dooropen.\n");
		return;
	}

	else if (GetCloseDoorLight())//Idel״̬���ڹ��Ű���ʧЧ
	{
		SetCloseDoorLight(false);
		*state = Idle;
		printf("Transition:from Idle to Idle.\n");
		return;
	}


}

void StateMovingUp(int *state)
{
	int floor;
	bool up;
	bool s;
	floor = GoingUpToFloor();//Event
	if (GetOpenDoorLight())//�˶�״̬������ʧЧ�����ѿ��Ű�ť
	{
		SetOpenDoorLight(false);

	}

	if (GetCloseDoorLight())//�˶�״̬������ʧЧ�����ѹ��Ű�ť
	{
		SetCloseDoorLight(false);
	}

	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)//UP s4

	{
		SetMotorPower(0);//ֹͣ
		SetDoor(floor, true);
		*state = DoorOpen;
		printf("Transition:from MovingUp to DoorOpen.\n");

		if (GetCallLight(floor, true))//��������UP��ť

		{

			SetCallLight(floor, true, false);
			
		}
		return;

	}
	

	
	else if (floor = Lib_FloorNum)//������߲���������down��ť
	{
		if (GetCallLight(floor, true))
		{
			SetCallLight(floor, false, false);
		}
	}

	floor = GetNearestFloor();	//��ȡ��ǰ¥��
	if (GetPanelFloorLight(floor))
	{
		SetPanelFloorLight(floor, false);//�������ڰ�ť

	}


	return;
}

		void StateMovingDown(int *state)
{
	int floor;
	bool up;
	bool s;
	floor = GoingDownToFloor();
	if (GetOpenDoorLight())//�˶�״̬��������ʧЧ�����ѿ�����
	{
		SetOpenDoorLight(false);
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
	}
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)//s5
	{
		SetMotorPower(0);
		SetDoor(floor, true);
		*state = DoorOpen;
		printf("Transition:from MovingDown to DoorOpen.\n");

		if (GetCallLight(floor, false))//��������down��ť
		{
			SetCallLight(floor, false, false);
		}
		return;

	}
	floor = GetNearestFloor();
	if (floor==1 )//����1����������up��ť 
	{ 
		if (GetCallLight(floor, true))
		{
			SetCallLight(floor, true, false);
		}
	
	}
	floor = GetNearestFloor();	//��ȡ��ǰ¥��
	if (GetPanelFloorLight(floor))
	{
		SetPanelFloorLight(floor, false);//�������ڰ�ť

	}

	return;
}

void StateDoorOpen(int *state)
{
	int floor;
	bool up; bool s;
	floor = GetNearestFloor();//��ȡ��ǰ¥��
	if (GetOpenDoorLight())//���ڿ����������ڿ��Ű�ť
	{
		SetOpenDoorLight(false);
	}
	if (GetCallLight(floor, true))//����ʱ�������ⰴť(up)
	{
		SetDoor(floor, true);
		SetCallLight(floor, true, false);
		*state = DoorOpen;
		return;
	}
	if (GetCallLight(floor, false))//����ʱ�������ⰴť(down)
	{
		SetDoor(floor, true);
		SetCallLight(floor, false, false);
		*state = DoorOpen;
		return;
	}

	if (GetCloseDoorLight())// Event:�������ڹ��Ű�ť s6
	{
		SetDoor(floor, false);
		//SetCloseDoorLight(false);
		*state = DoorClosing;
		printf("Transition:from DoorOpen to DoorClosing./n");
		return;
	}
	if (IsDoorOpen(floor))//Event:���Ž������Զ����� s6
	{
		SetDoor(floor, false);
		*state = DoorClosing;
		printf("Transition:from DoorOpen to DoorClosing./n");
		return;
	}

}

void StateDoorClosing(int *state)
{
	int floor;
	bool up; bool s;
	floor = GetNearestFloor();//��ȡ��ǰ¥��
	if (GetCloseDoorLight())//Event:���ڹ���ʱ�����ŵ�p5
	{
		SetCloseDoorLight(false);//���ѹ��Ű�ť
	}


	if (GetOpenDoorLight())//Event:���ڹ���ʱ�����ڿ��Ű�ťs7
	{
		SetDoor(floor, true);
		*state = DoorOpen;
		printf("Transition:from DoorClosing to DoorOpen.\n");
		return;
	}
	if (IsBeamBroken())//���ڹ��Ÿ�Ӧ���ϰ���
	{
		SetDoor(floor, true);
		*state = DoorOpen;
		printf("Transition:from DoorClosing to DoorOpen.\n");
		return;

	}


	else if (IsDoorClosed(floor))//���Ž���
	{
		SetDoor(floor, false);
		*state = Idle;
		printf("Transition:from DoorClosing to Idel.\n");
		return;

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

