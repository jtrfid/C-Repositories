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

	int floor; bool up;
	floor = IdleWhatFloorToGoTo(&up);
	if (floor > 0 && up)
	{
		SetMotorPower(1);                                                                  //电梯上升
		*state = MovingUp;                                                          
	}
	else if (floor > 0 && !up)
	{
		SetMotorPower(-1);                                                              //电梯下降
		*state = MovingDown;                                                    
	}
	else if (GetCallLight(floor, true))
	{
		SetDoor(floor, true);                                                         //开门
		SetCallLight(floor, true, false);                                            //关闭开门灯
		*state = DoorOpen;
	}
	else if (GetCallLight(floor, false))
	{
		SetDoor(floor, true);                                                         //开门
		SetCallLight(floor, false, false);                                             //关灯
		*state = DoorOpen;
	}
	else if (GetOpenDoorLight())
	{
		SetDoor(GetNearestFloor(),true);
		SetOpenDoorLight(false);                                                     //消费开门按钮
		*state = DoorOpen;                                                    
	}
	else (GetCloseDoorLight)
	{
		SetCloseDoorLight(false);
		return;                                                                           //消费按键
	}
}

void StateMovingUp(int *state)
{
		int floor;
		floor = GoingUpToFloor();
		if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
		{
			SetMotorPower(0);             
			SetDoor(GetNearestFloor(), true);                                           //开门
			*state = DoorOpen;                                                  
		}
		else if (GetCallLight(GetNearestFloor(),true))                                            //消费up键
			SetCallLight(GetNearestFloor(),true,false);      
		else if (Lib_FloorNum)
		{
			SetCallLight(GetNearestFloor(), false,false);                                      //消费3层down键
		}
		SetPanelFloorLight(GetNearestFloor(), false);                                      //消费门内楼层按钮
		else if (GetOpenDoorLight())
			    SetOpenDoorLight(false);                                
		else (GetCloseDoorLight())
			SetCloseDoorLight(false);                                                              //消费门内开门灯以及关门灯
}

void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();                                                                 //定义floor为当前层
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);                                                                         //电梯停止
		SetDoor(GetNearestFloor(), true);                                                            //开门
		*state = DoorOpen;                                                                        //转到dooropen状态
	}
	else if (GetCallLight(GetNearestFloor(), true))
		SetCallLight(GetNearestFloor(), true, false);                                           //消费门外up键
	else (Lib_FloorNum)
	{
		SetCallLight(GetNearestFloor(), false, false);
	}
	SetPanelFloorLight(GetNearestFloor(), false);                                                      //消费门内楼层按钮
}

void StateDoorOpen(int *state)
{
	if (GetCloseDoorLight())
	{
		SetDoor(GetNearestFloor(), false);                                                                     //关门
		SetCloseDoorLight(false);                                                                     //消费门内关门灯

		*state = DoorClosing;            
	}
	else if (IsDoorOpen(GetNearestFloor()))
	{
		SetDoor(GetNearestFloor(),false);                                                                   //自行关门
		*state = DoorClosing;               
	}
	else (GetOpenDoorLight())
		SetOpenDoorLight(false);                                                                           //消费门内开门灯
}

void StateDoorClosing(int *state)
{
	if (GetOpenDoorLight())
	{
		SetDoor(GetNearestFloor(),true);                                                                     //开门
		SetOpenDoorLight(false);                                                                           //消费内门开门等
		*state = DoorOpen;                           
	}
	else if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);                                                                            //消费内门关门灯
	}
	else if (IsBeamBroken())
	{
		SetDoor(GetNearestFloor(), true);                                                               //当红外线检测到有物体未通过，停止关门并开门
		*state = DoorOpen;                            
	}
	else (IsDoorClosed(GetNearestFloor()))
		*state=Idle;                           
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
