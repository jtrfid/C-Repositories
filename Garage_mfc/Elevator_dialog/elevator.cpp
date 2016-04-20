#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ElevatorLib.h"

/*
 * Name :        <insert name here>
 * Description : Project 2 - The elevator controller
 */

/*
 * Name :        main()
 * Description : Program entry point.
 */


/**********************************************
 * ����״̬���ж��Ƿ��а�������Ҫ�������״̬
 **********************************************/
void StateIdle(int *state)
{
	int floor;
	bool up;
	
	floor = IdleWhatFloorToGoTo(&up);
	if(floor > 0)
	   printf("��¥��:%d,����:%s\n",floor,up?"����":"����");

	if (floor > 0) {
		if (up) {
			SetMotorPower(1);
			*state = MovingUp;
		}
		else {
			SetMotorPower(-1);
			*state = MovingDown;
		}
	} 
	
}

void StateMovingUp(int *state)
{
	int floor = GoingUpToFloor(); // ����Ƿ���һ¥����Ҫ����¥��
	double distance = GetFloor();

	printf("%d,%f,%f,%f\n",floor,GetPosition(),distance,distance-floor);
	if(fabs(GetFloor() - floor) < 0.01) {
		printf("��[%d]¥����\n",floor);
		// ������UP�� Call Light Off
		SetCallLight(floor,true,false);
		// ������¥���Floor Light Off
		SetPanelFloorLight(floor,false);
		SetMotorPower(0);
		*state = Idle;
	}

	/**
	if(Lib_CurrentCarPosition >= Lib_MaxCarPosition - Lib_FloorTolerance) {
		ASSERT(FALSE); // �����ܵ���

		printf("��3¥���������߰ɣ�\n");
		SetMotorPower(0);
		*state = Idle;
	}
	**/
}

void StateMovingDown(int *state)
{
	int floor = GoingUpToFloor(); // ����Ƿ���һ¥����Ҫ����¥��

	if(fabs(GetFloor() - floor) < 0.01) {
		CString status;
		status.Format(_T("��[%d]¥����\n"),floor);
		ViewStatus(status);

		printf("��[%d]¥����\n",floor);
		// ������Down��Call Light Off
		SetCallLight(floor,false,false);
		// ������¥���Floor Light Off
		SetPanelFloorLight(floor,false);
		SetMotorPower(0);
		*state = Idle;
	}

	/***
	if(Lib_CurrentCarPosition <= Lib_FloorTolerance) {
		ASSERT(FALSE); // �����ܵ���
		SetMotorPower(0);
		*state = Idle;
	}
	***/
}

/**********************************************
 * ���ţ�һ��ʱ����Զ��������
 **********************************************/
void StateDoorOpen(int *state)
{
	// ������ʱ�����Զ�����
	*state = DoorClosing;
}

/********************************************
 * ����̽��
 ********************************************/
void StateDoorClosing(int *state)
{
	*state = Idle;
}

/***********************************************
 * This loop runs until we shut the elevator system down
 * by closing the window it runs in.
 ***********************************************/
void main_control(int *state)
{  
    if(IsElevatorRunning())
    {
		switch(*state)
		{
		case Idle:
			StateIdle(state);
			break;
		case MovingUp:
			StateMovingUp(state);
			break;
		case MovingDown:
			StateMovingDown(state);
			break;
		case DoorOpen:
			StateDoorOpen(state);
			break;
		case DoorClosing:
			StateDoorClosing(state);
			break;
		default:
			printf("û������״̬!!!\n");  
		}
    }
}
