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
	floor=IdleWhatFloorToGoTo(&up);
	if (floor > 0 && up)
	{
		SetMotorPower(1);//���������������
		*state =MovingUp;//ת��
		printf("Transition:from Idel to MovingUp.\n");//transition
	}
	else if (floor > 0 && !up)
	{
		SetMotorPower(-1);//����������½���
		*state = MovingDown;//ת��
		printf("Transition��from Idel to MovingDown.\n");//transition
	}
	else if (GetOpenDoorLight())
	{
		SetDoor(GetNearestFloor(), true);//����
		SetOpenDoorLight(false);//�������ڰ�ť��
		*state = DoorOpen;//ת��
		printf("Transition:from Idel to DoorOpen.\n");//transition
	}
	else if (GetCallLight(floor, up))
	{
		SetDoor(floor, true);//����
		SetCallLight(GetNearestFloor(), up, false);//�������ⰴť��
		*state = DoorOpen;//ת��
		printf("Transition: from Idle to DoorOpen.\n");//transition
	}
	
	else if (GetCloseDoorLight()) 
	{ 
		SetCloseDoorLight(false); //���Ѱ�ť��
	    return; //����
	}



}

void StateMovingUp(int *state)
{
	int floor; 
	bool up;
	floor = GoingDownToFloor();
	up = GetCallLight(floor, true);
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);//ֹͣ���
		SetDoor(GetNearestFloor(), true);//����
		*state = DoorOpen;//ת��
		printf("Transition:from MovingUp to DooorOpen.\n");//transition
		SetCallLight(GetNearestFloor(), true, false);//��������up��ť��
		if (GetNearestFloor() == Lib_FloorNum)//������¥
		{
			SetCallLight(GetNearestFloor(), false, false);//��������down��ť��
		}
		SetPanelFloorLight(GetNearestFloor(), false);//���ѵ�ǰ¥�㰴ť��
	}
	else if (GetOpenDoorLight || GetCloseDoorLight)
	{
		SetOpenDoorLight(false);//�������ڿ��Ű�ť��
		SetCloseDoorLight(false);//�������ڹ��Ű�ť��
	}
}

void StateMovingDown(int *state)
{
	int floor; bool up;
	floor = GoingDownToFloor();
	up = GetCallLight(floor, true);
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);//ֹͣ���
		SetDoor(GetNearestFloor(), true);//����
		*state = DoorOpen;//ת��
		printf("Transition :from MovingDown to DoorOpen./n");//transition
		SetCallLight(GetNearestFloor(), false, false);//��������down��ť��
		if (GetNearestFloor() == 1)//����һ¥
		{
			SetCallLight(GetNearestFloor(), true, false);//��������up��ť��
		}
		SetPanelFloorLight(GetNearestFloor(), false);//���ѵ�ǰ¥�㰴ť��
	}
	else if (GetOpenDoorLight() || GetCloseDoorLight())//�������⿪�ţ�����ʵ����ָʾֱ�����ѣ�
	{
		SetOpenDoorLight(false);//�������ڿ��Ű�ť��
		SetCloseDoorLight(false);//�������ڹ��Ű�ť��
	}

}

void StateDoorOpen(int *state)
{
	
	bool up;
	if (GetCloseDoorLight())
	{
		SetDoor(GetNearestFloor(), false);//����
		SetCloseDoorLight(false);//���ѹ��Ű�ť��
		*state = DoorClosing;//ת��
		printf("Transition:from MovingDown to DoorClosing.\n");//transition
	}
	else if (IsDoorOpen(GetNearestFloor()))//���Ǵ򿪵�
	{
		SetDoor(GetNearestFloor(), false);//����
		*state = DoorClosing;//ת��
		printf("Transition:from DoorOpen to DoorCloosing.\n");//transition
	}
	else if (GetOpenDoorLight())//�ظ�����ť
	{
		SetOpenDoorLight(false);//���ѿ��Ű�ť
	}
}

void StateDoorClosing(int *state)
{
	bool up;
	if (GetOpenDoorLight())
	{
		SetDoor(GetNearestFloor(), true);//����
		SetOpenDoorLight(false);//���ѿ��Ű�ť
		*state = DoorOpen;//ת��
		printf("Transition:from DoorClosing to DoorOpen.\n");//transition
	}
	else if (GetCloseDoorLight())//�ظ�����ť
	{
		SetCloseDoorLight(false);//���ѹ��Ű�ť
	}
	else if (IsBeamBroken())//���ϰ���
	{
		SetDoor(GetNearestFloor(), true);//����
		*state = DoorOpen;//ת��
		printf("Transition:from DoorClosing to DoorOpen.\n");//transition
	}
	else if (IsDoorClosed(GetNearestFloor()))//���ѹر�
	{
		*state = Idle;//ת��
		printf("Transition:from DoorClosd to Idle.\n");//transition
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
