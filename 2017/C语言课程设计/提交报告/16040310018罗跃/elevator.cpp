#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ElevatorLib.h"

/**********************************************
 * Idle状态，电梯停止在某楼层，门是关闭的，处于静止状态，等待相关事件的发生，从而转换到下一个状态。
 **********************************************/
void StateIdle(int *state)
{
	int floor;
	bool up;
	floor = IdleWhatFloorToGoTo(&up);   
	if (floor > 0 && up)  //Event
	{
		SetMotorPower(1);  //Transition
		*state = MovingUp;  //进入MovingDown状态     
	}
	floor = IdleWhatFloorToGoTo(&up);
	if (floor>0 && !up)  //Event
	{
		SetMotorPower(-1);  //Transition
		*state = MovingDown;  //进入MovingDown状态            
	}
	if (GetOpenDoorLight())  //Event
	{
		SetDoor(GetNearestFloor(), true);  //消费开门按键
		SetOpenDoorLight(false);
		*state = DoorOpen;  //进入DoorOpen状态
	}
	if (GetCallLight(GetNearestFloor(), true))  //Event
	{
		SetDoor(GetNearestFloor(), true);  //消费开门按键
		SetCallLight(GetNearestFloor(), true, false);  //消费门外up/down按键
		*state = DoorOpen;  //进入DoorOpen状态
	}
	if (GetCallLight(GetNearestFloor(), false))  //Event
	{
		SetDoor(GetNearestFloor(), true);   //消费开门按键
		SetCallLight(GetNearestFloor(), false, false);
		*state = DoorOpen;  //进入DoorOpen状态
	}
	if (GetCloseDoorLight())  //Event 消费关门灯按键
	{
		SetCloseDoorLight(false);
		return;
	}
}

/* MovingUp状态函数 */
void StateMovingUp(int *state)
{
	int floor;
	floor = GoingUpToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)  //Event 判断是否到达目标楼层
	{
		SetMotorPower(0);  //Transition  停止 
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;  
		if (GetNearestFloor() != Lib_FloorNum)  //Event  获取当前楼层
			SetCallLight(GetNearestFloor(), true, false);  //消费门外up按钮
		else (SetCallLight(GetNearestFloor(), false, false));   //消费门外down按钮
	}
	SetPanelFloorLight(GetNearestFloor(), false);  //消费门内楼层按钮
	if (GetOpenDoorLight())  //Event
		SetOpenDoorLight(false);   //消费开门按键
	if (GetCloseDoorLight())  //Event   
		SetCloseDoorLight(false);  //消费关门按键
}

/* IMovingDown状态函数 */
void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)  //Event
	{
		SetMotorPower(0);  //Transition
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;  //进入DoorOpen状态
	} 
	if (GetCallLight(GetNearestFloor(), true))  //Event  
		SetCallLight(GetNearestFloor(), true, false);  //消费down按钮
	else if (Lib_FloorNum)  //Event
	{
		SetCallLight(GetNearestFloor(), false, false);
	}
	SetPanelFloorLight(GetNearestFloor(), false);  //消费门内楼层按钮
	SetCallLight;
	if (Lib_FloorNum)   //Event
		SetCallLight;
	SetPanelFloorLight;
}

/* DoorOpen状态函数 */
void StateDoorOpen(int *state)
{
	int floor;
	floor = GetNearestFloor();
	if (GetCloseDoorLight())  //Event
	{
		SetDoor(floor, false);
		SetCloseDoorLight(false);
		*state = DoorClosing;   //进入DoorClosing状态
	}
	if (IsDoorOpen(floor))  //Event
	{
		SetDoor(floor, false);  //自动进行关门
		*state = DoorClosing;  //进入DoorClosing状态
	}
	if (GetCloseDoorLight())  //Event
		SetOpenDoorLight(false);  //消费开门按钮
}

/*DoorClosing状态函数 */
void StateDoorClosing(int *state)
{

	int floor;
	floor = GetNearestFloor();
	if (GetOpenDoorLight())  //Event
	{
		SetDoor(floor, true);
		SetOpenDoorLight(false);
		*state = DoorOpen;  //进入DoorOpen状态
	}
	if (GetCloseDoorLight())  //Event
		SetCloseDoorLight(false);  //消费关门按钮
	if (IsBeamBroken())  //Event 是否红外探测到遮挡物
	{
		SetDoor(floor, true);
		*state = DoorOpen;  //进入DoorOpen状态
	}
	if (IsDoorClosed(floor))  //Event
		*state = Idle;  //进入Idle状态
}

/***********************************************
 * 状态机，每隔一定时间(如，100ms)被调用一次，采集系统的运行状态
 ***********************************************/
void main_control(int *state)
{  
    if(IsElevatorRunning())  // 仿真正在运行
    {
		switch(*state)
		{
		case Idle:
			// Idle状态，一定时间无动作，自动到一楼
			if(GetNearestFloor() !=1 ) {
				AutoTo1Floor();
			}
			StateIdle(state);
			break;
		case MovingUp:
			CancelTo1Floor(); // 其它状态，取消自动到一楼
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
