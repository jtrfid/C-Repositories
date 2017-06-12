#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ElevatorLib.h"
/**********************************************
* Idle״̬������ֹͣ��ĳ¥�㣬���ǹرյģ����ھ�ֹ״̬���ȴ�����¼��ķ������Ӷ�ת������һ��״̬��
**********************************************/
void StateIdle(int *state)//ƽ��״̬������0������ʱ״̬Ϊ���� ��������ʱ�����������ڿ��ţ����ź�ťϨ��״̬Ϊ���š�
{                                                                     // ���������ڹ��ţ������ź�ťϨ��״̬Ϊ����
	int floor; bool up;
	floor = IdleWhatFloorToGoTo(&up);//���Ҫ����¥�� ��Ŀ��¥�� 
	if (floor > 0 && up){  //¥�㡷0��up 
		SetMotorPower(1);//���� 
		*state = MovingUp;//״̬Ϊmovingup 
	}
	else if (floor > 0 && !up){  //Ŀ��¥�� 
		SetMotorPower(-1);  //�½� 
		*state = Movingopen;//����moving open״̬ 
	}
	if (GetOpenDoorLight()){  //������ڿ��ذ�ť 
		SetDoor(floor, true);   //���� 
		SetOpenDoorLight(false);//���ѿ��ذ�ť
		*state = DoorOpen;  //���� door open״̬ 
	}
	if (GetCloseDoorLight()){  //������ڿ��ذ�ť 
		SetCloseDoorLight(false);//���ѹ��Ű�ť 
		return;
	}
}

void StateMovingUp(int *state)//����ǰ¥����Ŀ��¥�����ֵ��С��ֹͣ���������ţ�������а�ťϨ�� ������ 
{                              //���Ű�ť�����Ű�ť��Ϩ�� 
	int floor; bool up, s;
	floor = GoingUpToFloor();  //Ŀ��¥�� 
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance){//����Ŀ��¥�� 
		SetMotorPower(0);        //ֹͣ 
		SetDoor(floor, true);  //���� 
		*state = DoorOpen;    //����door open״̬ 
		if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
			SetCallLight(floor, false, false);//��������up��ť 
		SetCallLight(floor, true, false);  
		SetPanelFloorLight(floor, false);//��������¥�㰴ťΪ��ǰ¥��İ�ť 
	}
	else {
		GetOpenDoorLight(); SetOpenDoorLight(false);//������ڿ��ذ�ť�����ѿ��Ű�ť 
		GetCloseDoorLight(); SetCloseDoorLight(false);// ������ڿ��ذ�ť�����ѹ��Ű�ť 
	}
}

void StateMovingDown(int *state)//����ǰ¥����Ŀ��¥�����ֵ��С��ֹͣ�½������ţ�������а�ťϨ�� ������ 
{                                //���Ű�ť�����Ű�ť��Ϩ��
	int floor; bool up;
	floor = GoingDownToFloor();  //Ŀ��¥�� 
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance){//����Ŀ��¥�� 
		SetMotorPower(0);   //ֹͣ 
		SetDoor(floor, true);//���� 
		*state = DoorOpen;  //����door open״̬ 
		if (fabs(GetFloor() - 1) < Lib_FloorTolerance)//����Ŀ��¥�� 
		SetCallLight(floor, true, false);//��������down��ť 
		SetupLight(floor, false, false);//����1�� ����������up��ť 
		SetPanelFloorLight(floor, false);//��������¥�㰴ťΪ��ǰ¥�㰴ť 
		GetNearestFloor();//��ȡ��ǰ¥�� 
	}
	else {
		GetOpenDoorLight(); SetOpenDoorLight(false);//������ڿ��Ű�ť���������ڿ��Ű�ť 
		GetCloseDoorLight(); SetCloseDoorLight(false);// ������ڹ��Ű�ť���������ڹ��Ű�ť 
	}
}

void StateDoorOpen(int *state)
{                                
	int floor;
	floor = GetNearestFloor();//��ȡ��ǰ¥�� 
	if (GetCloseDoorLight()){  //���ڿ��ţ����˹��ŵƣ�ת�����š� 
		SetCloseDoorLight(false);//���ѹ��Ű�ť 
		*state = DoorClosing;   //����door closing״̬ 
	}
	else if (IsDoorOpen(floor)){    //���Ž��� 
		SetDoor(floor, false);  //�Զ����й��� 
		*state = DoorClosing;  //����door closing״̬ 
	}
	else if (GetOpenDoorLight())//������ڿ��ذ�ť�����ڿ��ţ� ���˹��ŵ� 
		SetOpenDoorLight(false);//���ѿ��Ű�ť 
}

void StateDoorClosing(int *state)
{
	int floor;
	floor = GetNearestFloor();//��ȡ��ǰ¥�� 
	if (GetCloseDoorLight()){// ������ڿ��ذ�ť  
		SetDoor(floor, true);//���˿��Ű�ť��ת������ 
		SetOpenDoorLight(false);//���ѿ��Ű�ť 
		*state = DoorOpen;//����door open״̬ 
	}
	else if (GetCloseDoorLight()) SetCloseDoorLight(false);//�����Ű�ť�����˹��Ű�ť���޶��������ѹ��Ű�ť 
	else if (IsBeamBroken()){    //���������̽�⵽�ڵ��� 
		SetDoor(floor, true);  //ת������ 
		*state = DoorOpen;    //����door open״̬ 
	}
	else if (IsDoorClosed(floor)) *state = Idle;//���Ž���������idle״̬ 
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
