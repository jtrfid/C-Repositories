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
 * 空闲状态，判断是否有按键，将要进入何种状态
 **********************************************/
void StateIdle(int *state)
{
	int floor;
	bool up;
	
	floor = IdleWhatFloorToGoTo(&up);
	if(floor > 0)
	   printf("到楼层:%d,方向:%s\n",floor,up?"向上":"向下");

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
	int floor = GoingUpToFloor(); // 检查是否上一楼层是要到的楼层
	double distance = GetFloor();

	printf("%d,%f,%f,%f\n",floor,GetPosition(),distance,distance-floor);
	if(fabs(GetFloor() - floor) < 0.01) {
		CString status;
		status.Format(_T("到[%d]楼啦！\n"),floor);
		ViewStatus(status);

		printf("到[%d]楼啦！\n",floor);
		// 电梯外UP， Call Light Off
		SetCallLight(floor,true,false);
		// 电梯内楼层号Floor Light Off
		SetPanelFloorLight(floor,false);
		SetMotorPower(0);
		
		// 开门
		SetDoor(floor,true);
		*state = DoorOpen;
	}

	/**
	if(Lib_CurrentCarPosition >= Lib_MaxCarPosition - Lib_FloorTolerance) {
		ASSERT(FALSE); // 不可能到此

		printf("到3楼啦，往下走吧！\n");
		SetMotorPower(0);
		*state = Idle;
	}
	**/
}

void StateMovingDown(int *state)
{
	int floor = GoingUpToFloor(); // 检查是否下一楼层是要到的楼层

	if(fabs(GetFloor() - floor) < 0.01) {
		CString status;
		status.Format(_T("到[%d]楼啦！\n"),floor);
		ViewStatus(status);

		printf("到[%d]楼啦！\n",floor);
		// 电梯外Down，Call Light Off
		SetCallLight(floor,false,false);
		// 电梯内楼层号Floor Light Off
		SetPanelFloorLight(floor,false);
		SetMotorPower(0);

		// 开门
		SetDoor(floor,true);
		*state = DoorOpen;
	}

	/***
	if(Lib_CurrentCarPosition <= Lib_FloorTolerance) {
		ASSERT(FALSE); // 不可能到此
		SetMotorPower(0);
		*state = Idle;
	}
	***/
}

/**********************************************
 * 开门，一定时间后自动进入关门
 **********************************************/
void StateDoorOpen(int *state)
{
	int floor = GetNearestFloor();
	// 如果开门结束，进入关门状态
	if(IsDoorOpen(floor))
	{
		// 关门
		SetDoor(floor,false);
		*state = DoorClosing;
	} 
}

/********************************************
 * 红外探测
 ********************************************/
void StateDoorClosing(int *state)
{
	// 如果关门结束，到空闲状态，判断下一步的走向
	if(IsDoorClosed(GetNearestFloor()))
	{
		*state = Idle;
	} 
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
			printf("没有这种状态!!!\n");  
		}
    }
}
