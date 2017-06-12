include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ElevatorLib.h"

/**********************************************
* Idle״̬������ֹͣ��ĳ¥�㣬���ǹرյģ����ھ�ֹ״̬���ȴ�����¼��ķ������Ӷ�ת������һ��״̬��
**********************************************/
bool up;
void StateIdle(int *state)
{
	int floor;
	floor = IdleWhatFloorToGoTo(&up);//Ŀ��¥��
	if (floor > 0 && up)  SetMotorPower(1); //����
	else if (floor > 0 && !up)  SetMotorPower(-1);//�½�
	else if (GetOpenDoorLight() )//��鿪�ŵ�״̬
	{
		SetDoor(GetNearestFloor(),true);//����
		SetOpenDoorLight(false);//���ѿ��Ű�ť
	}
	else if (GetCallLight(GetNearestFloor(),true))//��ȡ��ǰ¥������CallLight��ť״̬
	{
		SetDoor(GetNearestFloor(),true);//����
		SetCallLight(GetNearestFloor(),up,false);//���ѵ�ǰ¥��UP��ť
	}
	else if (GetCallLight(GetNearestFloor(),false ))
	{
		SetDoor(GetNearestFloor(), true);
		SetCallLight(GetNearestFloor(), false, false);//��������down��ť
	}

	else if(GetCloseDoorLight())
	{
		SetCloseDoorLight(false);//���ѹ��Ű�ť
		return;
	}
	

}

void StateMovingUp(int *state)
{
	int floor;
    floor = GoingDownToFloor();
	if (fabs(GetFloor()-floor)>Lib_FloorTolerance)
	{
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);
	}
	if (fabs(GetFloor()-floor)<Lib_FloorTolerance)//����
	{
		SetMotorPower(0);//ֹͣ
		SetDoor(floor, true);//����
		floor = GetNearestFloor();//��ȡ��ǰ¥��;
		if (floor < Lib_FloorNum)//����UP ��ť
			SetCallLight(floor, true, false);
		else if (floor == Lib_FloorNum)//��߲� ����DOWN��ť
			SetCallLight(floor, false, false);
		SetPanelFloorLight(floor, false);//���ѵ�ǰ¥�㰴ť
	}
}

void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor)>Lib_FloorTolerance)//�����½�
	{
		SetOpenDoorLight(false);//���ѿ��Ű�ť
		SetCloseDoorLight(false);//���ѹ��Ű�ť
    }
	if (fabs(GetFloor() - floor)<Lib_FloorTolerance)
	{
		SetMotorPower(0);//ֹͣ
		SetDoor(floor, true);//����
		floor = GetNearestFloor();
		if (floor > 1) //����DOWN��ť
			SetCallLight(floor, false, false);
		else if (floor == 1)//1¥����UP ��ť
			SetCallLight(floor, true, false);
		SetPanelFloorLight(floor,false);//���ѵ�ǰ¥�㰴ť
     }
	
}

void StateDoorOpen(int *state)
{
	if (GetCloseDoorLight())//���˹��ŵƣ�
		SetDoor(GetNearestFloor(), false);
	else if (GetOpenDoorLight())//���˿���
	{
		SetDoor(GetNearestFloor(), true);//����
		SetOpenDoorLight(false);//���ѿ��Ű�ť
	}
	else if (IsDoorOpen(GetNearestFloor()))//���Ž������Զ�����
		SetDoor(GetNearestFloor(), false);//����
	
}

void StateDoorClosing(int *state)
{
	if (GetOpenDoorLight())//����Ƿ��˿��ŵ�
	{
		SetDoor(GetNearestFloor(), true);//���ڹ��ţ�ת������
		SetOpenDoorLight(false);//���ѿ��Ű�ť
	}
	else if (GetCloseDoorLight())// ����Ƿ��˹��Ű�ť
	{
		SetDoor(GetNearestFloor(), false);//���ڿ��ţ��ﶯ��
		SetCloseDoorLight(false);//���ѹ��Ű�ť
	}
	else if (IsBeamBroken())//����Ƿ����ڵ���
		SetDoor(GetNearestFloor(), true);//ֹͣ���ţ�ת������
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

