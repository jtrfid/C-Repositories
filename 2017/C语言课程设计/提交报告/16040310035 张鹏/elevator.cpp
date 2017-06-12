#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ElevatorLib.h"

/**********************************************
 * Idle状态，电梯停止在某楼层，门是关闭的，处于静止状态，等待相关事件的发生，从而转换到下一个状态。
 **********************************************/
void StateIdle(int *state)
{   int floor; bool up;
      floor = IdleWhatFloorToGoTo(&up);
	  
	if (floor > 0 && up)
	 {
	SetMotorPower(1);
	 *state = MovingUp;
	 printf("从静止状态上升");
	 }                                              
    if (floor > 0 && !up)
	{
    SetMotorPower(-1);
	*state = MovingDown;
	printf("从静止状态下降");                 
	 } 
	if (GetOpenDoorLight())
	{
	SetDoor(floor, true);
	SetOpenDoorLight(false);
	*state = DoorOpen;
	printf("从门内按钮开门");
	}
    else if (GetCallLight(GetNearestFloor(), true) || GetCallLight(GetNearestFloor(), false))
	{
	SetDoor(floor, true);
	SetCallLight( GetNearestFloor(), false, false);
	SetCallLight( GetNearestFloor(), true, false);
	*state = DoorOpen;
	printf("从门外呼叫开门");
	}                                         
	if (GetCloseDoorLight()) 
	{
	SetCloseDoorLight(false); 
	return; 
	}                                            
}

void StateMovingUp(int *state)
{
	int floor;
	floor = GoingUpToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
	SetMotorPower(0);
	SetOpenDoorLight(false);
	SetCloseDoorLight(false);
	SetCallLight(GetNearestFloor(), true, false);
	SetCallLight(GetNearestFloor(), false, false);
	SetPanelFloorLight(floor, false);
	SetMotorPower(0);
	SetDoor(floor, true);
	GetOpenDoorLight();
	GetCloseDoorLight();
	*state = DoorOpen;
	printf("从上升状态开门");
	}                                            

}

void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
	SetOpenDoorLight(false);
	SetCloseDoorLight(false);
	SetCallLight(GetNearestFloor(), true, false);
	SetCallLight(GetNearestFloor(), false, false);
	SetPanelFloorLight(GetNearestFloor(), false);
	SetMotorPower(0);
	SetDoor(floor, true);
	GetOpenDoorLight();
	GetCloseDoorLight();
    *state = DoorOpen;
	printf("从下降状态开门");
	}                                          
}
void StateDoorOpen(int *state)
{
int floor = GetNearestFloor();
if (GetCloseDoorLight()||IsDoorOpen(floor))
{
   SetCloseDoorLight(false);
   SetOpenDoorLight(false);
   SetDoor(floor, false);
   *state = DoorClosing;  
   printf("从开门到关门");
}                                                    
	

}

void StateDoorClosing(int *state)
{
	int floor = GetNearestFloor();
	if (GetOpenDoorLight())
	{
	SetOpenDoorLight(false);
	SetCloseDoorLight(false);
	SetDoor(floor, true);
	*state = DoorOpen;
	printf("从关门到开门");
	}                                         
	if (IsBeamBroken())
	{
	SetDoor(floor, true);
	*state = DoorOpen;
	printf("检测到异物开门");
	}
	if (IsDoorClosed(floor))
	{
	*state = Idle;
	}
	
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
