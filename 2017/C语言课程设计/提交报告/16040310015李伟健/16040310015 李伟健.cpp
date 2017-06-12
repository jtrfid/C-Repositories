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
	bool up;
	int floor;
	floor = IdleWhatFloorToGoTo(&up);
 if(GetOpenDoorLight()==true)//Event
 {
	 SetDoor(GetNearestFloor(), true);//Transition
	 SetOpenDoorLight(false);
	 *state = DoorOpen;
	 printf("Transition: from Idle to DoorOpen.\n");
 } 
 else if (GetCallLight(GetNearestFloor(), true) || GetCallLight(GetNearestFloor(), false))//Event，门外向下或向上呼叫按钮被按下
 {
	 SetDoor(GetNearestFloor(), true);//Transition
	 SetCallLight(GetNearestFloor(), true, false);
	 SetCallLight(GetNearestFloor(), false, false);
	 *state = DoorOpen;
	 printf("Transition: from Idle to DoorOpen.\n");
 }
 else if ((GetCallLight || GetPanelFloorLight) == true)//Event，门外呼叫按钮或门内楼层按钮被按下
 {
	 
	 if (floor > 0 && up)//判断楼层
	 {
		 SetMotorPower(1);//Transition
		 *state = MovingUp;
		 printf("Transition: from Idle to MovingUp.\n");
	 }
	 else if (floor > 0 && !up)//判断楼层
	 {
		 SetMotorPower(-1);//Transition
		 *state = MovingDown;
		 printf("Transition: from Idle to MovingDown.\n");
	 }
 }
 else if (GetCloseDoorLight()==true)
 {
	 SetCloseDoorLight(false); return;
 }
 }
 
/*MovingUp状态函数*/
void StateMovingUp(int *state)
{
	bool up;
	int floor;
	floor = GoingUpToFloor();
	if ((GetCallLight || GetPanelFloorLight) == true)//Event，门外呼叫按钮或门内楼层按钮被按下
	{
		if (fabs(GetFloor() - floor) < Lib_FloorTolerance)//判断是否到达目标楼层
		{
			SetMotorPower(0);//Transition
			SetDoor(floor, true);//Transition
			SetPanelFloorLight(floor, false);
			*state = DoorOpen;
			if (floor != 3)//判断楼层是否为最高层
			{
				SetCallLight(floor, true, false);
				SetCallLight(floor, false, false);
			}
			else
				SetCallLight(floor, false, false);
			printf("Transition: from MovingUp to DoorOpen .\n");
		}
	}
	else if ((GetOpenDoorLight || GetCloseDoorLight) == true)//Event，门内开门按钮火关门按钮被按下
	{
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);
	}
}

/*MovingDown状态函数*/
void StateMovingDown(int *state)
{
	bool up;
	int floor;
	floor = GoingDownToFloor();
	if((GetCallLight||GetPanelFloorLight)==true)//Event
	{
		if (fabs(GetFloor() - floor) < Lib_FloorTolerance)//判断是否到达目标楼层
		{
			SetMotorPower(0);//Transition
			SetDoor(floor, true);//Transition
			SetPanelFloorLight(floor, false);
			*state = DoorOpen;			
			if (floor != 1)//判断楼层是否为1层
			{
				SetCallLight(floor, false, false);
				SetCallLight(floor, true, false);
			}
			else
                SetCallLight(floor, true, false);	
			printf("Transition: from MovingDown to DoorOpen .\n");
		}
	}
	else if ((GetOpenDoorLight || GetCloseDoorLight) == true)//Event，门内开门按钮火关门按钮被按下
	{
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);
	}
}

/*DoorOpen状态函数*/
void StateDoorOpen(int *state)
{
	bool up;
	if (GetCloseDoorLight() == true)//Event
	{
		SetDoor(GetNearestFloor(), false);//Transition
		SetCloseDoorLight(false);
		*state = DoorClosing;
		printf("Transition: from DoorOpen to DoorClosing .\n");
	}
	else if (IsDoorOpen(GetNearestFloor()))//Event，门打开
	{
		SetDoor(GetNearestFloor(), false);//Transition
		*state = DoorClosing;
		printf("Transition: from DoorOpen to DoorClosing .\n");
	}
	else if (GetOpenDoorLight() == true)
	{
		SetOpenDoorLight(false);
	}
}

/*DoorClosing状态函数*/
void StateDoorClosing(int *state)
{
	bool up;
	if (GetOpenDoorLight() == true)//Event
	{
		SetDoor(GetNearestFloor(), true);//Transition
		SetOpenDoorLight(false);
		*state = DoorOpen;
		printf("Transition: from DoorClosing to DoorOpen .\n");
	}
	else if (GetCloseDoorLight() == true)
	{
		SetCloseDoorLight(false);
	}
	else if (IsBeamBroken() == true)//Event，红外探测到物体
	{
		SetDoor(GetNearestFloor(), true);//Transition
		*state = DoorOpen;
		printf("Transition: from DoorClosing to DoorOpen .\n");
	}
	else if (IsDoorClosed(GetNearestFloor()))//门关闭
		*state = Idle;
	printf("Transition: from DoorClosing to Idle .\n");
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
