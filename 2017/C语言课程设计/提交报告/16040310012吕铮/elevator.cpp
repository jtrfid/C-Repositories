#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ElevatorLib.h"

/**********************************************
 * Idle状态，电梯停止在某楼层，门是关闭的，处于静止状态，等待相关事件的发生，从而转换到下一个状态。
 **********************************************/
bool up;
void StateIdle(int *state)
{
	int floor; bool up;
	floor = IdleWhatFloorToGoTo(&up);  // Event
	if (floor > 0 && up)
	{
		SetMotorPower(1);
		*state = MovingUp;                                // 进入MovingUp状态
	}
	floor = IdleWhatFloorToGoTo(&up);
	if (floor > 0 && !up)
	{
		SetMotorPower(-1); 
		*state = MovingDown;                              // 进入MovingDown状态
	}
	if (GetOpenDoorLight())                               //门内
	{
		SetDoor(GetNearestFloor(), true);                 //开门
		SetOpenDoorLight(false);                          //消费开门键
		*state = DoorOpen;                                //进入DoorOpen状态
	}
	if (GetCallLight(GetNearestFloor(), true))            //门外
	{
		SetDoor(GetNearestFloor(), true);                 //开门
		SetCallLight(GetNearestFloor(), true, false);     //关灯
		*state = DoorOpen;                                //进入DoorOpen状态
	}
	if (GetCallLight(GetNearestFloor(), false))           //门外
	{
		SetDoor(GetNearestFloor(), true);                 //开门
		SetCallLight(GetNearestFloor(),false, false);     //关灯
		*state = DoorOpen;                                //进入DoorOpen状态  
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);                         //消费按键
		return;                        
	}
}

void StateMovingUp(int *state)
{
	int floor;
	floor = GoingUpToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);                                      //停止
		SetDoor(GetNearestFloor(), true);                      //开门
		*state = DoorOpen;                                     //进入DoorOpen状态
		if (GetNearestFloor() != Lib_FloorNum)
			SetCallLight(GetNearestFloor(), true, false);      //消费门外up键
		else (SetCallLight(GetNearestFloor(), false, false));  //消费顶层down键
	}
	SetPanelFloorLight(GetNearestFloor(),false);               //消费门内该楼层按键        
	if (GetOpenDoorLight())
		SetOpenDoorLight(false);                              //消费门内开门灯
	if (GetCloseDoorLight())
		SetCloseDoorLight(false);                             //消费门内关门灯
}

void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);                                     //停止
		SetDoor(GetNearestFloor(), true);                     //开门
		*state = DoorOpen;                                    //进入DoorOpen状态
		if (GetNearestFloor() != 1)
			SetCallLight(GetNearestFloor(), false, false);    //消费门外up键
		else SetCallLight(1, true, false);                                 //消费1层up键
	}
	SetPanelFloorLight(GetNearestFloor(), false);             //消费门内该楼层按键        
	if (GetOpenDoorLight())
		SetOpenDoorLight(false);                              //消费门内开门灯
	if (GetCloseDoorLight())
		SetCloseDoorLight(false);                             //消费门内关门灯
}

void StateDoorOpen(int *state)
{
	int floor;
	floor = GetNearestFloor();                               //获取当前楼层
	if (GetCloseDoorLight())
	{
		SetDoor(floor,false);                                //关门
		SetCloseDoorLight(false);                            //消费门内关门灯
		*state = DoorClosing;                                //进入DoorClosing状态
	}
	if (IsDoorOpen(floor))
	{
		SetDoor(floor,false);                               //自动关门
		*state = DoorClosing;                               //进入DoorClosing状态
	}
	if (GetOpenDoorLight())
		SetOpenDoorLight(false);                            //消费门内开门灯
}

void StateDoorClosing(int *state)
{
	int floor;
	floor = GetNearestFloor();                               //获取当前楼层
	if (GetOpenDoorLight())
	{
		SetDoor(floor, true);                                //开门
		SetOpenDoorLight(false);                             //消费门内开门灯
		*state = DoorOpen;                                   //进入DoorOpen状态
	}
	if (GetCloseDoorLight())
		SetCloseDoorLight(false);                            //消费门内关门灯
	if (IsBeamBroken())                                      //红外探测
	{
		SetDoor(floor, true);                                //自动开门
		*state = DoorOpen;                                   //进入DoorOpen状态
	}
	if (IsDoorClosed(floor))
		*state = Idle;                                       //进入Idle状态
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
