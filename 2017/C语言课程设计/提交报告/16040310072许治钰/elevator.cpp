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
	floor = IdleWhatFloorToGoTo(&up);//��̬��⣬���Ŀ��¥��
	if (floor > 0 && up)//���Ŀ��¥���Ƿ���ڵ�������¥��
	{
		SetMotorPower(1);//���������������������
		*state = MovingUp;
		printf("Transition: from Idle to MovingUp.\n");
	}
	else if (floor > 0 && !up)//���Ŀ��¥���Ƿ���ڵ�������¥��
	{
		SetMotorPower(-1);//���������������������
		*state = MovingDown;
		printf("Transition: from Idle to MovingDown.\n");
	}
	else if (GetOpenDoorLight())//������ڿ��Ű�ť�Ƶ�״̬
	{
		SetOpenDoorLight(false);//�رտ��Ű�ť��
		SetDoor(floor, true); //�򿪵�����
		*state = DoorOpen;
		printf("Transition: from Idle to DoorOpen.\n");
	}
	else if (GetCloseDoorLight())//������ڹ��Ű�ť�Ƶ�״̬
	{
		SetCloseDoorLight(false);//�رչ��Ű�ť��
		return;
	}
	floor=GetNearestFloor();//��ȡ��������¥����
	if (GetCallLight(floor, false))//����������°�ť�Ƶ�״̬
	{
		SetCallLight(floor, false, false); //�ر��������°�ť��
		SetDoor(floor, true);//�򿪵�����
		*state = DoorOpen;
		printf("Transition: from Idle to DoorOpen.\n");
	}
	if (GetCallLight(floor, true))//����������ϰ�ť�Ƶ�״̬
	{
		SetCallLight(floor, up, false);//�ر��������ϰ�ť��
		SetDoor(floor, true);//�򿪵�����
		*state = DoorOpen;
		printf("Transition: from Idle to DoorOpen.\n");
	}

}
/**********************************************
* MovingUp״̬�����������ƶ����ƶ������е������޷����ء�
**********************************************/

void StateMovingUp(int *state)
{
	int floor;
	floor = GoingUpToFloor(); //��̬��⣬���Ŀ��¥��
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)//����Ƿ񵽴�Ŀ��¥��
	{
		if (GetCallLight(floor, true))//����������ϰ�ť�Ƶ�״̬
		{
			SetCallLight(floor, true, false);//�ر��������ϰ�ť��
			SetPanelFloorLight(floor, false);//�ر����ڵ�������¥��İ�ť��
			SetMotorPower(0);//���ֹͣ
			SetDoor(floor, true);//�򿪵�����
			*state = DoorOpen;
			printf("Transition: from MovingUp to DoorOpen.\n");
			return;
		}
		if (GetCallLight(floor, false))//����������°�ť�Ƶ�״̬
		{
			SetCallLight(floor, false, false); //�ر��������°�ť��
			SetPanelFloorLight(floor, false);//�ر����ڵ�������¥��İ�ť��
			SetMotorPower(0);//���ֹͣ
			SetDoor(floor, true);//�򿪵�����
			*state = DoorOpen;
			printf("Transition: from MovingUp to DoorOpen.\n");
			
		}
		SetPanelFloorLight(floor, false);//�ر����ڵ�������¥��İ�ť��
		SetMotorPower(0);//���ֹͣ
		SetDoor(floor, true);//�򿪵�����
		*state = DoorOpen;
		printf("Transition: from MovingUp to DoorOpen.\n");
	}
	else if (GetOpenDoorLight())//������ڿ��Ű�ť�Ƶ�״̬
	{
		SetOpenDoorLight(false);//�رտ��Ű�ť��
		return;
	}
	else if (GetCloseDoorLight())//������ڹ��Ű�ť�Ƶ�״̬
	{
		SetCloseDoorLight(false);//�رչ��Ű�ť��
		return;
	}
}
/**********************************************
* MovingDown״̬�����������ƶ����ƶ������е������޷����ء�
**********************************************/

void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();//��̬��⣬���Ŀ��¥��
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)//����Ƿ񵽴�Ŀ��¥��
	{
		if (GetCallLight(floor, true))//����������ϰ�ť�Ƶ�״̬
		{
			SetCallLight(floor, true, false);//�ر��������ϰ�ť��
			SetMotorPower(0);//���ֹͣ
			SetDoor(floor, true);//�򿪵�����
			SetPanelFloorLight(floor, false);//�ر����ڵ�������¥��İ�ť��
			*state = DoorOpen;
			printf("Transition: from MovingDown to DoorOpen.\n");
			return;
		}
		if (GetCallLight(floor, false))//����������°�ť�Ƶ�״̬
		{
			SetCallLight(floor, false, false);//�ر��������°�ť��
			SetMotorPower(0);//���ֹͣ
			SetDoor(floor, true);//�򿪵�����
			SetPanelFloorLight(floor, false);//�ر����ڵ�������¥��İ�ť��
			*state = DoorOpen;
			printf("Transition: from MovingDown to DoorOpen.\n");
		}
		SetMotorPower(0);//���ֹͣ
		SetDoor(floor, true);//�򿪵�����
		SetPanelFloorLight(floor, false);//�ر����ڵ�������¥��İ�ť��
		*state = DoorOpen;
		printf("Transition: from MovingDown to DoorOpen.\n");
	}
	else if (GetOpenDoorLight())//������ڿ��Ű�ť�Ƶ�״̬
	{
		SetOpenDoorLight(false);//�رտ��Ű�ť��
		return;
	}
	else if (GetCloseDoorLight())//������ڹ��Ű�ť�Ƶ�״̬
	{
		SetCloseDoorLight(false);//�رչ��Ű�ť��
		return;
	}
}
/**********************************************
* DoorOpen״̬: �򿪵����ţ��������ڿ��Ű�ť������رտ��Ű�ť�ơ�
**********************************************/

void StateDoorOpen(int *state)
{
	int floor;
	floor = GetNearestFloor();//��⵱ǰ��������¥��
	if (GetCloseDoorLight())//������ڹ��Ű�ť�Ƶ�״̬
	{
		SetDoor(floor, false);//�رյ�����
		SetCloseDoorLight(false);//�رչ��Ű�ť��
		*state = DoorClosing;
		printf("Transition: from DoorOpen to DoorClosing.\n");
	}
	else if (IsDoorOpen(floor))//���������Ƿ��Ǵ򿪵�
	{
		SetDoor(floor, false);//�رյ�����
		*state = DoorClosing;
		printf("Transition: from DoorOpen to DoorClosing.\n");
	}
	else if (GetOpenDoorLight())//������ڿ��Ű�ť�Ƶ�״̬
	{
		SetOpenDoorLight(false);//�رտ��Ű�ť��
		return;
	}
}
/**********************************************
* DoorClosing״̬: �رյ����ţ��������ڹ��Ű�ť������رչ��Ű�ť�ơ�
**********************************************/
void StateDoorClosing(int *state)
{
	int floor;
	floor = GetNearestFloor();//��⵱ǰ��������¥��
	if (GetOpenDoorLight())//������ڿ��Ű�ť�Ƶ�״̬
	{
		SetDoor(floor, true);//�򿪵�����
		SetOpenDoorLight(false);//�رտ��Ű�ť��
		*state = DoorOpen;
		printf("Transition: from DoorClosing to DoorOpen.\n");
	}
	else if (GetCloseDoorLight())//������ڹ��Ű�ť�Ƶ�״̬
	{
		SetCloseDoorLight(false);//�رչ��Ű�ť��
		return;
	}
	else if (IsBeamBroken())//����̽�����Ƿ�̽�⵽�ڵ�
	{
		SetDoor(floor, true);//�򿪵�����
		SetOpenDoorLight(false);//�رտ��Ű�ť��
		*state = DoorOpen;
		printf("Transition: from DoorClosing to DoorOpen.\n");
	}
	else if (IsDoorClosed(floor))//���������Ƿ��ǹرյ�
	{
		*state = Idle;
		printf("Transition: from DoorClosing to Idle.\n");
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
