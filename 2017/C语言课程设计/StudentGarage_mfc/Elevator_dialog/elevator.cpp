#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ElevatorLib.h"

/**********************************************
 * 空闲状态，判断是否有按键，判断将要进入何种状态
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
 * 开门，开门结束后自动进入关门
 **********************************************/
void StateDoorOpen(int *state)
{

}

/********************************************
 * 正在关门，省略红外探测
 ********************************************/
void StateDoorClosing(int *state)
{

}

/***********************************************
 * 状态机，每隔一定时间(如，100ms)被调用一次，采集系统的运行状态
 ***********************************************/
void main_control(int *state)
{  
    if(IsElevatorRunning())
    {
		switch(*state)
		{
		case Idle:
			// 一定时间无动作，自动到一楼
			if(GetNearestFloor() !=1 ) {
				AutoTo1Floor();
			}
			StateIdle(state);
			break;
		case MovingUp:
			CancelTo1Floor();
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
			printf("没有这种状态!!!\n");  
		}
    }
}
