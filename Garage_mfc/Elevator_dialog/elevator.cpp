#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>

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
	SetMotorPower(1);
	*state = MovingUp;
}

void StateMovingUp(int *state)
{
	if(Lib_CurrentCarPosition >= Lib_MaxCarPosition - Lib_FloorTolerance) {
		SetMotorPower(0);
		*state = Idle;
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
