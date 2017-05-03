#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ElevatorLib.h"

/**********************************************
 * ����״̬���ж��Ƿ��а������жϽ�Ҫ�������״̬
 **********************************************/
void StateIdle(int *state)
{

}

void StateMovingUp(int *state)
{
	

}

void StateMovingDown(int *state)
{

}

/**********************************************
 * ���ţ����Ž������Զ��������
 **********************************************/
void StateDoorOpen(int *state)
{

}

/********************************************
 * ���ڹ��ţ�ʡ�Ժ���̽��
 ********************************************/
void StateDoorClosing(int *state)
{

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
