#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ElevatorLib.h"

/**********************************************
 * Idle状态，电梯停止在某楼层，门是关闭的，处于静止状态，等待相关事件的发生，从而转换到下一个状态。
 **********************************************/
void StateIdle(int *state)                                                       //Idle状态机
{   int floor; bool up;
      floor = IdleWhatFloorToGoTo(&up);
	if (floor > 0 && up)
	 {
		 SetMotorPower(1);
		 *state = MovingUp;
		 printf("从ldle到MovingUp");
	 }                                                                           //S1
    if (floor > 0 && !up)
	{
		 SetMotorPower(-1);
		 *state = MovingDown;
		 printf("从ldle到MovingDown");                                           //S2
	 } 
    if (GetOpenDoorLight()||GetCallLight(floor=GetNearestFloor(),true)||GetCallLight(floor=GetNearestFloor(),false))
	{
		 SetDoor(floor, true);
		 SetOpenDoorLight(false);
		 SetCallLight(floor=GetNearestFloor(), false, false);
		 SetCallLight(floor = GetNearestFloor(), true, false);
		 *state = DoorOpen;
		 printf("从ldle到DoorOpen");
	}                                                                            //S3
	if (GetCloseDoorLight()) 
	{
		SetCloseDoorLight(false); 
		return; 
		printf("从ldle到ldle");
	}                                                                            //S4
}

void StateMovingUp(int *state)                                                   //上升状态机
{
	int floor;
	floor = GoingUpToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(floor, true);
		SetCallLight(floor,true,false);
		SetCallLight(floor, false, false);
		SetPanelFloorLight(floor = GetNearestFloor(), false);
		printf("从MovingUp到DoorOpen");
		GetOpenDoorLight();
		GetCloseDoorLight();
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);                                         //D
		*state = DoorOpen;
	}                                                                             //S5                

}

void StateMovingDown(int *state)                                                 //下降状态机
{
	int floor;
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(floor, true);
		SetCallLight(floor, true, false);
		SetCallLight(floor, false, false);
		SetPanelFloorLight(floor=GetNearestFloor(), false);
		printf("从MovingDown到DoorOpen");
		GetOpenDoorLight();
		GetCloseDoorLight();
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);                                          //D
		*state = DoorOpen;
	}                                                                              //S6
}
void StateDoorOpen(int *state)                                                     //开门状态机
{
	int floor = GetNearestFloor();
	if (GetCloseDoorLight()||IsDoorOpen(floor))
	{
		SetDoor(floor, false);
		SetCloseDoorLight(false);
		SetOpenDoorLight(false);
		*state = DoorClosing; 
		printf("从DoorOpen到DoorClosing");                                         //S7
	}                                                    
	

}

void StateDoorClosing(int *state)
{
	int floor = GetNearestFloor();
	if (GetOpenDoorLight()||IsBeamBroken())
	{
		SetDoor(floor, true);
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);
		*state = DoorOpen;
		printf("从DoorClosing到DoorOpen");
	}                                                                              //S8
	if (IsDoorClosed(floor))
	{
		*state = Idle;
		printf("从DoorClosing到Idle");
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
