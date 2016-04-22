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
	int CurrentFooor = GetNearestFloor();

	// 监测电梯外上下按钮灯(Call Light)，开关门
	// 本层的电梯外上下按钮灯(Call Light)被按亮，静态变量保证开门一次,电梯走动后,恢复此值
	// 上下按钮各有一次开门机会
	static bool openUpOnes = false;
	static bool openDownOnes = false;
	
	if(!openUpOnes && GetCallLight(CurrentFooor,true)) {  // 向上
		openUpOnes = true;
		*state = DoorOpen;
	}
	if(!openDownOnes && GetCallLight(CurrentFooor,false)) { // 向下
		openDownOnes = true;
		*state = DoorOpen;
	}


	// 将要到那一层
	floor = IdleWhatFloorToGoTo(&up);
	if(floor > 0)
	   printf("空闲状态，将要到的楼层:%d,方向:%s\n",floor,up?"向上":"向下");

	if (floor > 0) {
		openUpOnes = false;   // 开关门复位
		openDownOnes = false;
		if (up) {
			// 本层的up call light off
			SetCallLight(CurrentFooor,true,false);
			SetMotorPower(1);
			*state = MovingUp;
		}
		else {
			// 本层的down call light off
			SetCallLight(CurrentFooor,false,false);
			SetMotorPower(-1);
			*state = MovingDown;
		}
	} 	
}

void StateMovingUp(int *state)
{
	int floor = GoingUpToFloor(); // 检查是否上一楼层是要到的楼层

	double distance = GetFloor();
	//printf("StateMovingUp %d,%f,%f,%f\n",floor,GetPosition(),distance,distance-floor);

	if(fabs(GetFloor() - floor) < 0.01) {
		CString status;
		status.Format(_T("Up\n[%d]楼"),floor);
		ViewStatus(status);

		printf("Up 到[%d]楼啦！\n",floor);
		// 电梯外UP， Call Light Off
		SetCallLight(floor,true,false);
		// 电梯内楼层号Floor Light Off
		SetPanelFloorLight(floor,false);
		SetMotorPower(0);
		
		// 开门
		SetDoor(floor,true);
		*state = DoorOpen;
	}

}

void StateMovingDown(int *state)
{
	int floor = GoingDownToFloor(); // 检查是否下一楼层是要到的楼层

	double distance = GetFloor();
	//printf("StateMovingDown %d,%f,%f,%f\n",floor,GetPosition(),distance,distance-floor);

	if(fabs(GetFloor() - floor) < 0.01) {
		CString status;
		status.Format(_T("Down\n[%d]楼"),floor);
		ViewStatus(status);

		printf("Down 到[%d]楼啦！\n",floor);
		// 电梯外Down，Call Light Off
		SetCallLight(floor,false,false);  // 待处理，如果是因为是up而到此层的，应该置up为light off
		// 电梯内楼层号Floor Light Off
		SetPanelFloorLight(floor,false);
		SetMotorPower(0);

		// 开门
		SetDoor(floor,true);
		*state = DoorOpen;
	}

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
	else // 开门
	{
		SetDoor(floor,true);
	}
}

/********************************************
 * 正在关门，省略红外探测
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
