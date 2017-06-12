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
	floor = IdleWhatFloorToGoTo(&up); //确定前往楼层 
	if (floor > 0 && up){			  //判断状态 
		SetMotorPower(1);				//打开电机 
		*state = MovingUp;				//进入下一状态 
	}
	if (floor > 0 && !up){				//状态同上 
		SetMotorPower(-1);
		*state = MovingDown;
	}
	if (GetOpenDoorLight()){			//判断开门灯 
		SetDoor(floor, true);			//开门 
		SetOpenDoorLight(false);		//消费开门灯 
		*state = DoorOpen;				//进入下一状态 
	}
	if (GetCloseDoorLight()){			//无状态，无变化 
		SetCloseDoorLight(false);
		return;
	}
	int getfloor;
	getfloor = GetNearestFloor();
	if (GetCallLight(GetNearestFloor(), true) || GetCallLight(GetNearestFloor(), false)){	//判断电梯与呼唤灯是否同一层 
		SetDoor(getfloor, true);															//开门 
		SetCallLight(getfloor, true, false);												//消费 
		SetCallLight(getfloor, false, false);											
		*state = DoorOpen;																	//进入下一状态 
	}
}

void StateMovingUp(int *state)
{
	int floor;
	floor = GoingUpToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance){				//判断是否到达 
		SetMotorPower(0);											//关闭电机 
		SetDoor(floor, true);										//开门 
		SetCallLight(floor, true, false);							//消费外灯 
		SetCallLight(floor, false, false);
		SetPanelFloorLight(floor, false);							//消费内部楼层灯 
		*state = DoorOpen;											//下一状态 
	}
	if (GetOpenDoorLight() || GetCloseDoorLight()){					//移动状态无法开关门 
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);
	}
}

void StateMovingDown(int *state)									//状态与上升相同 
{
	int floor;
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance){
		SetMotorPower(0);
		SetDoor(floor, true);
		SetCallLight(floor, true, false);
		SetCallLight(floor, false, false);
		SetPanelFloorLight(floor, false);
		*state = DoorOpen;
	}
	if (GetOpenDoorLight() || GetCloseDoorLight()){
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);
	}
}

void StateDoorOpen(int *state)
{
	int floor = GetNearestFloor();
	if (GetCloseDoorLight()){							//判断关门灯 
		SetDoor(floor,false);							//关门 
		SetCloseDoorLight(false);						//消费 
		*state = DoorClosing;
	}
	if (IsDoorOpen(floor)){								//开门中进行关门 
		SetDoor(floor, false);
		*state = DoorClosing;
	}
	if (GetOpenDoorLight()){							//开门灯无状态 
		SetOpenDoorLight(false);
	}
}

void StateDoorClosing(int *state)
{
	int floor = GetNearestFloor();
	if (GetOpenDoorLight()){							//状态与开门时关门类似 
		SetDoor(floor, true);
		SetOpenDoorLight(false);
		*state = DoorOpen;
	}
	if (GetCloseDoorLight()){
		SetCloseDoorLight(false);
	}
	if (IsBeamBroken()){								//判断是否有物体在电梯门口 
		SetDoor(floor ,true);							//开门 
		*state = DoorOpen;								//进入下一状态 
	}
	if (IsDoorClosed(floor)){
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
