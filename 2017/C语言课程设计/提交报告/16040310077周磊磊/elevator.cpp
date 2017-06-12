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
	floor = IdleWhatFloorToGoTo(&up);//��̬���
	if (floor > 0 && up)//s1
	{
		SetMotorPower(1);
		*state = MovingUp;
		printf("Transition: from Idle to MovingUp.\n");
	}
	if (floor > 0 && !up)//�¼�s2
	{
		SetMotorPower(-1);
		*state = MovingDown;
		printf("Transition: from Idle to MovingDown.\n");
	}
	if (GetOpenDoorLight())//�¼�s3�е�E1
	{
		SetDoor(GetNearestFloor(), 1);
		SetOpenDoorLight(0);
		*state = DoorOpen;
		printf("Transition: from Idle to DoorOpen.\n");
	}
	if (up&&GetCallLight(GetNearestFloor(), 1))//�¼�s3�е�E4
	{
		SetDoor(GetNearestFloor(), 1);
		SetCallLight(GetNearestFloor(), 1, 0);
		*state = DoorOpen;
		printf("Transition: from Idle to DoorOpen.\n");
	}
	if (!up&&GetCallLight(GetNearestFloor(), 1))//�¼�s3�е�E5
	{
		SetDoor(GetNearestFloor(), 1);
		*state = DoorOpen;
		printf("Transition: from Idle to DoorOpen.\n");
	}
	if (GetCloseDoorLight())//�¼�s4
	{
		SetCloseDoorLight(0);
		return;
	}

}

void StateMovingUp(int *state)
{
	int floor;
	bool up;
	floor = GoingUpToFloor();//��̬���
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)//s5  
	{
		SetMotorPower(0);//
		SetDoor(floor, 1);
		SetPanelFloorLight(GetNearestFloor(), 0);
		SetCallLight(floor, 1, 0);// ����
		SetCallLight(floor, 0, 0);
		*state = DoorOpen;
		printf("Transition: from MovingUp to DoorOpen.\n");
	}
	if (GetNearestFloor() == 3)
		SetCallLight(floor, 1, 0);
	if (GetOpenDoorLight()) //�¼�D 1
		SetOpenDoorLight(0);
	if (GetCloseDoorLight())//�¼�D 2
		SetOpenDoorLight(0);
}
void StateMovingDown(int *state)
{
	int floor;
	bool up;
	floor = GoingDownToFloor();//��̬���
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)//s6  
	{
		SetMotorPower(0);
		SetDoor(floor, 1);
		SetPanelFloorLight(GetNearestFloor(), 0);//��������¥�㰴ť
		SetCallLight(GetNearestFloor(),1|| 0, 0);//��������up/down��ť
		*state = DoorOpen;
		printf("Transition: from MovingDown to DoorOpen.\n");
	}
	if (GetNearestFloor() == 1)
		SetCallLight(floor, 0, 0);
	if (GetOpenDoorLight())//�¼�D
		SetCloseDoorLight(0);
	if (GetCloseDoorLight())//�¼�D
		SetOpenDoorLight(0);
}

void StateDoorOpen(int *state)
{
	int floor;
	floor = GetNearestFloor();
	if (GetCloseDoorLight())//�¼�s7   1
	{
		SetDoor(GetNearestFloor(), 0);
		SetCloseDoorLight(0);//����
		*state = DoorClosing;
		printf("Transition: from  DoorOpen to DoorClosing.\n");
	}
	if (IsDoorOpen(floor))//�¼�s7  2
	{
		SetDoor(floor, 0);
		*state = DoorClosing;
		printf("Transition: from  DoorOpen to DoorClosing.\n");
	}
	if (GetOpenDoorLight())//�¼�s7   3
	{
		SetOpenDoorLight(0);//����
	}
}
void StateDoorClosing(int *state)
{
	int floor;
	floor = GetNearestFloor();
	if (GetOpenDoorLight())//�¼�s8     1
	{
		SetDoor(floor, 1);
		SetOpenDoorLight(0);
		*state = DoorOpen;
		printf("Transition: from DoorClosing to DoorOpen.\n");
	}
	if (GetCloseDoorLight())//�¼�s8    2
	{
		SetCloseDoorLight(0);
		return;
	}
	if (IsBeamBroken())//�¼�s8   3
	{
		SetDoor(floor, 1);
		*state = DoorOpen;
		printf("Transition: from DoorClosing to DoorOpen.\n");
	}
	if (IsDoorClosed(floor))
	{
		*state = Idle;
		printf("Transition: from DoorClosing to DoorOpen.\n");
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
