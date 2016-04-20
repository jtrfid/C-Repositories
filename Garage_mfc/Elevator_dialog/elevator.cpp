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
	
	floor = WhatFloorToGoTo(&up);
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
	static int floor = -1;
	if (floor == -1) floor = GoingUpToFloor();
	double distance = GetFloor();

	printf("%d,%f,%f,%f\n",floor,GetPosition(),distance,distance-floor);
	if(GetFloor() > floor) {
		printf("��[%d]¥����\n",floor);
		// ������Call Light Off
		SetCallLight(floor,true,false);
		// ������¥���Floor Light Off
		SetPanelFloorLight(floor,false);
		SetMotorPower(0);
		*state = Idle;
	}

	if(Lib_CurrentCarPosition >= Lib_MaxCarPosition - Lib_FloorTolerance) {
		printf("��3¥���������߰ɣ�\n");
		SetMotorPower(-1);
		*state = MovingDown;
	}
}

void StateMovingDown(int *state)
{
	if(Lib_CurrentCarPosition <= Lib_FloorTolerance) {
		SetMotorPower(0);
		*state = Idle;
	}
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
