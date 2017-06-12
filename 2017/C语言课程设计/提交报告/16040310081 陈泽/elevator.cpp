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
	int floor;
	bool up;
	floor = IdleWhatFloorToGoTo(&up);  // 静态检测(检测将要到的目标楼层)
	if (floor > 0 && up)
	{
	SetMotorPower(1);
	*state = MovingUp;                                // 变迁由静止进入MovingUp状态
	}
	floor = IdleWhatFloorToGoTo(&up);
	if (floor > 0 && !up)
	{
	SetMotorPower(-1); 
	*state = MovingDown;                              // 变迁由静止进入MovingDown状态
	}
	if (GetOpenDoorLight())                               //按门内开门键
	{
	SetDoor(GetNearestFloor(), true);                 //开门
	SetOpenDoorLight(false);                          //关闭开门键
	*state = DoorOpen;                                //变迁由静止进入DoorOpen状态
	}
	if (GetCallLight(GetNearestFloor(), true))            //按门外呼叫按钮
	{
	SetDoor(GetNearestFloor(), true);                 
	SetCallLight(GetNearestFloor(), true, false);    
	*state = DoorOpen;                                //变迁由静止进入DoorOpen状态
	}
	if (GetCallLight(GetNearestFloor(), false))           //按门外呼叫按钮
	{
		SetDoor(GetNearestFloor(), true);                 
		SetCallLight(GetNearestFloor(),false, false);     
		*state = DoorOpen;                                //变迁由静止进入DoorOpen状态  
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);                         //关闭按键
		return;                        
	}
}

void StateMovingUp(int *state)
{
	int floor;
	floor = GoingUpToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
	SetMotorPower(0);                                      //停止运动
	SetDoor(GetNearestFloor(), true);                      //开门
	*state = DoorOpen;                                     //变迁由DoorClosing进入DoorOpen状态
	if (GetNearestFloor() != Lib_FloorNum)
	SetCallLight(GetNearestFloor(), true, false);      //关闭呼叫按钮
	else (SetCallLight(GetNearestFloor(), false, false));  //关闭呼叫按钮
	}
	SetPanelFloorLight(GetNearestFloor(),false);               //关闭呼叫按钮    
	if (GetOpenDoorLight())
		SetOpenDoorLight(false);                              //关闭呼叫按钮
	if (GetCloseDoorLight())
		SetCloseDoorLight(false);                             //关闭呼叫按钮
}

void StateMovingDown(int *state)
{
int floor;
floor = GoingDownToFloor();
if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
SetMotorPower(0);                                         //停止运动
SetDoor(GetNearestFloor(), true);                        //开门
*state = DoorOpen;                                     //变迁进入DoorOpen状态
if (GetNearestFloor() != 1)
	
SetCallLight(GetNearestFloor(), false, false);           //关闭门外呼叫按钮
else( SetCallLight(1, true, false));                        //关闭门外呼叫按钮
	}
SetPanelFloorLight(GetNearestFloor(), false);             //关闭门外呼叫按钮      
if (GetOpenDoorLight())
	SetOpenDoorLight(false);                              //关闭门内呼叫按钮
	SetCloseDoorLight(false);                             //关闭门内呼叫按钮
}

void StateDoorOpen(int *state)
{
int floor;
floor = GetNearestFloor();                               //静态检测(检测将要到的目标楼层)
if (GetCloseDoorLight())
	{
SetDoor(floor,false);                                //关门
SetCloseDoorLight(false);                            
*state = DoorClosing;                                //变迁进入DoorClosing状态
	}
if (IsDoorOpen(floor))
	{
SetDoor(floor,false);                               //自动检测系统，自动关闭
*state = DoorClosing;                               //变迁进入DoorClosing状态
	}
	if (GetOpenDoorLight())
		SetOpenDoorLight(false);                       //关闭门内呼叫按钮
}

void StateDoorClosing(int *state)
{int floor;
	floor = GetNearestFloor();                        //静态检测(检测将要到的目标楼层)
	if (GetOpenDoorLight())
	{
		SetDoor(floor, true);                         //开门
		SetOpenDoorLight(false);                        //关闭门内呼叫按钮
		*state = DoorOpen;                            //变迁进入DoorOpen状态
	}
	if (GetCloseDoorLight())
		SetCloseDoorLight(false);                      //关闭门内呼叫按钮
	if (IsBeamBroken())                                 //检查障碍物，红外探测
	{
		SetDoor(floor, true);                           //门无法关闭
		*state = DoorOpen;                              //变迁进入DoorOpen状态
	}
	if (IsDoorClosed(floor))
		*state = Idle;                                  //变迁进入Idle状态
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
