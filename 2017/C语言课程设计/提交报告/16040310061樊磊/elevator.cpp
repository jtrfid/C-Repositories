#include "stdafx.h"
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
	int floor; bool up;
	floor = IdleWhatFloorToGoTo(&up);
	if (floor > 0 && up)                   //���¥��׼������
	{
		SetMotorPower(1);
		*state = MovingUp;
	}
	if (floor > 0 && !up)
	{                                      //���¥��׼���½�
		SetMotorPower(-1);
		*state = MovingDown;
	}
	if((GetOpenDoorLight())
	{
		SetOpenDoorLight(false);              //���Ѽ���������ڿ��Ű�ť������
		SetDoor(GetNearestFloor(),true);
		*state = DoorOpen;
	}
	if (GetCallLight())
	{
		SetCallLight(false);                 //���Ѽ������������а�ť������
		SetDoor();
		*state = DoorOpen;
	}
	if (GetCloseDoorLight())                  //���Ѽ���������ڹ��Ű�ť�����仯
	{
		SetCloseDoorLight(false);
		return;

	}
}

void StateMovingUp(int *state)
{
	int floor; bool up;
	floor = GoingUpToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)    //����ָ��¥�㣬����
	{
		SetMotorPower(0);
		SetDoor;
		*state = DoorOpen;
	}
	if (GetCallLight)                                        //���������а�ť��������ţ����Ѽ�
	{
		SetDoor;
		SetCallLight(GetNearestFloor(),false,false);
	}
	if (GetOpenDoorLight() || GetCloseDoorLight())              //���й��̣����ڿ��ذ�ť��Ч
	{
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);
	}
	
}

void StateMovingDown(int *state)
{
	int floor; bool up;
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)            //����ָ��¥�㿪��
	{
		SetMotorPower(0);
		SetDoor;
		*state = DoorOpen;
	}
	if (GetCallLight)                                              //���������а�ť�������ر�
	{
		SetCallLight(GetNearestFloor(),false,false);
	}
	if (floor = 1)                                                  //�������������ť���ر�
	{
		SetCallLight(1,true,false);
	}
	if (GetPanelFloorLight);                               //�������¥�㰴ť�����Ѽ�
	{
		SetPanelFloorLight(GetNearestFloor(),false); }
	if (GetOpenDoorLight() || GetCloseDoorLight)               //������ڿ����Ű�ť����Ч
	{
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);
	}
	


}

void StateDoorOpen(int *state)
{
	if (GetCloseDoorLight)                //������ڹ��Ű�ť�����ţ����Ѽ�
	{
		SetDoor(GetNearestFloor(),false);
		SetCloseDoorLight(false);
		*state = DoorClosing;
	}
	if (GetOpenDoorLight)                     //������ڿ��Ű�ť�����ţ����Ѽ�
	{
		SetOpenDoorLight(false);
	}
	if (IsDoorOpen)                      //����ſ������Զ��ر�
	{
		SetDoor(GetNearestFloor(),false);
		*state = DoorClosing;
	}
}

void StateDoorClosing(int *state)
{
	if (GetOpenDoorLight)                //������ڿ��Ű�ť���Ѽ�������
	{
		SetDoor;
		SetOpenDoorLight(false);
		*state = DoorOpen;
	}
	if (GetCloseDoorLight)            //������ڹ��Ű�ť�����Ѽ�����Ч
	{
		SetCloseDoorLight(false);
	}
	if (IsBeamBroken)              //��������ϰ�����ϰ��ﱣ�ֿ���״̬
	{
		SetDoor;
		*state = DoorOpen;
	}
	if (IsDoorClosed)          //�����״̬�رգ����뾲ֹ״̬
	{
		*state = Idle;
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
