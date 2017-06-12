#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ElevatorLib.h"

/**********************************************
 * Idle状态，电梯停止在某楼层，门是关闭的，处于静止状态，等待相关事件的发生，从而转换到下一个状态。
 **********************************************/
bool up;
void StateIdle(int *state)             //空置状态函数
{
	int floor;
    bool up;
	floor = IdleWhatFloorToGoTo(&up);  // 判断目标楼层以及接下来的操作步骤

     if (floor > 0 && up)
	   {
		SetMotorPower(1);
		*state = MovingUp;                                // MovingUp状态
	   }
	    floor = IdleWhatFloorToGoTo(&up);
      if (floor > 0 && !up)
	   {
		SetMotorPower(-1);
	 	*state = MovingDown;                              // MovingDown状态
	   }
	  if (GetOpenDoorLight())                               //空置状态下开门
	   {
		 SetDoor(GetNearestFloor(), 1);
		 SetOpenDoorLight(0);
		 *state = DoorOpen;                                //DoorOpen状态
	   }
	  if (GetCallLight(GetNearestFloor(), 1))            //门外
	   {
		SetDoor(GetNearestFloor(), 1);                 //开门
		SetCallLight(GetNearestFloor(), 1, 0);     //关灯
		*state = DoorOpen;
	   }
	  if (GetCallLight(GetNearestFloor(), 0))           //门外灯亮
       {
		SetDoor(GetNearestFloor(), 1);                 //开门
		SetCallLight(GetNearestFloor(),0, 0);     //门外灯灭
		*state = DoorOpen;
	   }
	  if (GetCloseDoorLight())
	   {
		SetCloseDoorLight(0);                         //消费该功能按键，防止死循环
		return;
	   }
}

void StateMovingUp(int *state)     //电梯上升状态函数
{
	int floor;
	floor = GoingUpToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);                                      //电机停止运转，电梯停下等候下一步指令
		SetDoor(GetNearestFloor(), true);                      //开门
		*state = DoorOpen;                                     //进入DoorOpen状态
		if (GetNearestFloor() != Lib_FloorNum)
			SetCallLight(GetNearestFloor(), true, false);      //消费门外上楼键
		else (SetCallLight(GetNearestFloor(), false, false));  //消费顶层下楼键
	}
	  SetPanelFloorLight(GetNearestFloor(),false);               //消费门内制定楼层按键
	  if (GetOpenDoorLight())
		   SetOpenDoorLight(false);
	  if (GetCloseDoorLight())
		   SetCloseDoorLight(false);
}

void StateMovingDown(int *state)        //电梯下降状态函数
{
	int floor;
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);                                     //电机停止运转，电梯停下等候下一步指令
		SetDoor(GetNearestFloor(), true);                     //开门
		*state = DoorOpen;                                    //进入DoorOpen状态
		if (GetNearestFloor() != 1)
			SetCallLight(GetNearestFloor(), false, false);
		else SetCallLight(1, true, false);                                 //消费1层up键
	}
	SetPanelFloorLight(GetNearestFloor(), false);             //消费门内该楼层按键
	if (GetOpenDoorLight())
		SetOpenDoorLight(false);                              //门内灯灭
	if (GetCloseDoorLight())
		SetCloseDoorLight(false);
}

void StateDoorOpen(int *state)             //开门状态函数
{
	int floor;
	floor = GetNearestFloor();                               //主控程序获取当前楼层
	if (GetCloseDoorLight())
	 {
		SetDoor(floor,false);                                //关门
		SetCloseDoorLight(false);
		*state = DoorClosing;                                //进入DoorClosing状态
	 }
	if (IsDoorOpen(floor))
	 {
	 	SetDoor(floor,false);                               //关门
		*state = DoorClosing;                               //进入DoorClosing状态

	 }
	if (GetOpenDoorLight())
		SetOpenDoorLight(false);                            //消费门内开门灯
}

void StateDoorClosing(int *state)         //关门状态函数
{
	int floor;
	floor = GetNearestFloor();                               //获取当前楼层
	if (GetOpenDoorLight())//电机停止运转，电梯停下等候下一步指令
	{
		SetDoor(floor, true);                                //开门
		SetOpenDoorLight(false);                             //消费门内开门灯
		*state = DoorOpen;                                   //进入DoorOpen状态
	}
	if (GetCloseDoorLight())
		SetCloseDoorLight(false);                            //消费门内关门灯
	if (IsBeamBroken())                                      //红外探测函数
	{
		SetDoor(floor, true);
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
