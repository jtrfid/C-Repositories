#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ElevatorLib.h"

/**********************************************
* Idle状态，电梯停止在某楼层，门是关闭的，处于静止状态，等待相关事件的发生，从而转换到下一个状态。
**********************************************/
/*void StateIdle(int *state)
{
	bool up;
	int floor;
	floor = IdleWhatFloorToGoTo(&up);
	if (floor>0 && up)
	{
		SetMotorPower(1);
		*state = MovingUp;
	}
	if (floor>0 && !up)
	{
		SetMotorPower(-1);
		*state = MovingDown;
	}
	if (GetOpenDoorLight())
	{
		SetDoor(GetNearestFloor(), 1);
		SetOpenDoorLight(0);
		*state = DoorOpen;
	}
	if (GetCallLight(GetNearestFloor(), 1))
	{
		SetDoor(GetNearestFloor(), 1);
		SetCallLight(GetNearestFloor(), 1, 0);
		*state = DoorOpen;
	}
	if (GetCallLight(GetNearestFloor(), 0))
	{
		SetDoor(GetNearestFloor(), 1);
		SetCallLight(GetNearestFloor(), 0, 0);
		*state = DoorOpen;
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(0);
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
		SetDoor(floor, 1);
		SetPanelFloorLight(floor, 0);
		SetCallLight(floor, 1, 0);
		if (floor == Lib_FloorNum)
		{
			SetCallLight(floor, 0, 0);
		}
		*state = DoorOpen;
	}
	if (GetOpenDoorLight() || GetCloseDoorLight())
	{
		SetOpenDoorLight(0);
		SetCloseDoorLight(0);
	}
}

void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(floor, 1);
		SetCallLight(floor, 0, 0);
		if (floor == 1)
			SetCallLight(floor, 1, 0);
		SetPanelFloorLight(floor, 0);
		*state = DoorOpen;
	}
	if (GetOpenDoorLight() || GetCloseDoorLight())
	{
		SetOpenDoorLight(0);
		SetCloseDoorLight(0);
	}
}

void StateDoorOpen(int *state)
{
	int floor;
	floor = GetNearestFloor();
	if (GetCloseDoorLight())
	{
		SetDoor(floor, 0);
		SetCloseDoorLight(0);
		*state = DoorClosing;
	}
	if (IsDoorOpen(floor))
	{
		SetDoor(floor, 0);
		*state = DoorClosing;
	}
	if (GetOpenDoorLight())
	{
		SetOpenDoorLight(0);
	}
}

void StateDoorClosing(int *state)
{
	int floor;
	floor = GetNearestFloor();
	if (GetOpenDoorLight())
	{
		SetDoor(floor, 1);
		SetOpenDoorLight(0);
		*state = DoorOpen;
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(0);
	}
	if (IsBeamBroken())
	{
		SetDoor(floor, 1);
		*state = DoorOpen;
	}
	if (IsDoorClosed(floor))
	{
		*state = Idle;
	}
}*/
/*********************************************
 * Idle状态，电梯停止在某楼层，门是关闭的，处于静止状态，等待相关事件的发生，从而转换到下一个状态。
 **********************************************/
void StateIdle(int *state)
 {
	int floor,nowfloor;
	bool up;
	nowfloor = GetNearestFloor(); 
	floor = IdleWhatFloorToGoTo(&up); 
	printf("%d",up);
	if (floor > 0 && up) {
		SetMotorPower(1);
		*state = MovingUp;
	}//上移
	if (floor > 0 && !up) {
		SetMotorPower(-1);
		*state = MovingDown;
	}//  检查E3 E4 E5事件 并返回运动方向

			if (GetOpenDoorLight()){ 
				SetDoor(nowfloor, 1);
			SetOpenDoorLight(0);
			*state = DoorOpen;//检查门内开关按钮 是否亮灯 亮灯则开门
			}
			if (GetCallLight(nowfloor, 1) && (up || !GetCallLight(3, 0) && (!up || GetCallLight(1, 1)))){
				SetDoor(nowfloor,1);
				SetCallLight(nowfloor,1,0);
				SetCallLight(nowfloor, 0, 0);
				*state = DoorOpen;
		}//检查E4 E5 判断目标 楼层是否是当前楼层 是即开门进入dooropen
			GetCloseDoorLight();
			if (GetCloseDoorLight()) { SetCloseDoorLight(false); return; }

}
	
void StateMovingUp(int *state)//电梯上移
	{
		int  floor;
		bool up;
		floor = GoingUpToFloor();
		printf("目标%d", floor);
		printf("当前%f \n", GetFloor());

		if ((fabs(GetFloor() - floor) < Lib_FloorTolerance) &&!( (GetCallLight(3, 0)) && !GetCallLight(2, 1) )){//判断是否到达
		 SetMotorPower(0);
			SetDoor(floor, 1); //开门
			if ((GetCallLight(floor, 0) )&& (GetCallLight(floor,1 )))
				SetCallLight(floor, 1, false);
		
		else{ SetCallLight(floor, 1, false);
		SetCallLight(floor, 0, false); }
			SetPanelFloorLight(floor, 0);
			SetPanelFloorLight(floor, 0);//关up灯
			*state = DoorOpen;
		}
		if ((fabs(GetFloor() - 3)) < Lib_FloorTolerance){
			SetMotorPower(0);
			SetDoor(floor, 1);
				SetCallLight(floor, 0, false);
		}
		if (GetOpenDoorLight() ||GetCloseDoorLight()){
			SetOpenDoorLight(0);
			SetCloseDoorLight(0);
		}//开关门失效
	}
		

void StateMovingDown(int *state)
{
	int  floor;
	bool up;
	floor = GoingUpToFloor();
	printf("目标%d", floor);
	printf("当前%f \n", GetFloor());
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance && !(GetCallLight(1, 1) && !GetCallLight(2, 0)) || fabs(GetFloor() - 1) < Lib_FloorTolerance){//判断是否到达
		SetMotorPower(0);
		SetDoor(floor, 1); //开门
		if (fabs(GetFloor() - 1) < Lib_FloorTolerance){//判断是否是最低层
			SetCallLight(1, 1, false); 
			SetPanelFloorLight(1, 0);//关up灯
		}
		else SetCallLight(floor, 0, false);
		SetPanelFloorLight(floor, 0);//关down灯



		*state = DoorOpen;
	}
	if (GetOpenDoorLight() ||
		GetCloseDoorLight()){
		SetOpenDoorLight(0);
		SetCloseDoorLight(0);
	}//开关门失效
}
void StateDoorOpen(int *state)
{
	
	int floor;
	floor = GetNearestFloor();
	if (GetCloseDoorLight() == 1){
		SetDoor(floor,0);
		SetCloseDoorLight(0);
		*state = DoorClosing;
	}
	if (IsDoorOpen(floor)){
		SetDoor(floor, 0);
		*state = DoorClosing;
	}
	if(GetOpenDoorLight())
		SetOpenDoorLight(0);

}

void StateDoorClosing(int *state)
{	int floor;
	floor = GetNearestFloor();
	if (GetOpenDoorLight()){
		SetDoor(floor,1);
		SetOpenDoorLight(0); 
		
	}
	if (GetCloseDoorLight()){
		SetCloseDoorLight(0);
		*state = DoorOpen;
	}
	if (IsBeamBroken()){ SetDoor(floor, 1);
	*state = DoorOpen;
	}
	if (IsDoorClosed(floor))
		*state = Idle;
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
