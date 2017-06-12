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
	floor = IdleWhatFloorToGoTo(&up);  /*确定电梯到达的目标楼层*/ 
	/*当楼层正确且上升按钮灯亮时，电梯上升，状态转移到上升状态*/ 
	if (floor > 0 && up) 			 
	{
		SetMotorPower(1);
		*state = MovingUp;
	}
	/*当楼层正确且下降按钮灯亮时，电梯下降，状态转移到下降状态*/ 
	else if (floor > 0 && !up) 		
	{
		SetMotorPower(-1);
		*state = MovingDown;
	}
	/*当电梯内部开门灯亮时，将该楼层的电梯门打开，状态转移到门开状态，消费开门灯*/ 
	if (GetOpenDoorLight()) 		
	{
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;
		 SetOpenDoorLight(false); return;
	}
	/*当本楼层电梯外上下电梯灯亮的时候，打开本楼层的门，状态转移到门开状态，消费上下电梯按钮灯*/ 
	else if (GetCallLight(GetNearestFloor(), true))
	{
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;
		SetCallLight(GetNearestFloor(), true, false);

	}
	else if (GetCallLight(GetNearestFloor(), false))
	{
		SetDoor(GetNearestFloor(), true);
		SetCallLight(GetNearestFloor(), false, false);
		*state = DoorOpen;

	}
	/*当电梯内部关门灯亮的时候，不进行其他操作，消费关门灯*/ 
	if (GetCloseDoorLight()) 
	{ SetCloseDoorLight(false); return; }

}

void StateMovingUp(int *state)
{
	int floor;
	floor = GoingUpToFloor();		/*检测上升中要到达的最近楼层*/ 
	
	/*若当前楼层与要到达楼层的差值小于允许误差时，电梯停止运动，电梯门打开*/ 
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(floor,true);
	/*若当前楼层为三层，消费电梯外下降按钮，否则消费电梯外上升按钮，状态切换到门开状态*/ 
	 if (GetNearestFloor() == 3)
	{
		SetCallLight(floor, false, false);
		*state = DoorOpen;
	}
	else
		SetCallLight(floor, true, false);
		*state = DoorOpen;
	}
	/*在上升过程中，如果开门关门灯被按下，立即消费掉，不执行其他操作*/ 
	if (GetOpenDoorLight())
	{
		SetOpenDoorLight(false);
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
	}
	
}

void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();		/*检测上升中要到达的最近楼层*/ 
	/*若当前楼层与要到达楼层的差值小于允许误差时，电梯停止运动，电梯门打开*/ 
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(floor, true);
		/*若当前楼层为一层，消费电梯外上升按钮，否则消费电梯外下降按钮，状态切换到门开状态*/ 
		if (GetNearestFloor() == 1)
		{
			SetCallLight(floor, true, false);
		}
		else
			SetCallLight(floor, false, false);
		*state = DoorOpen;
	}
	/*在下降过程中，如果开门关门灯被按下，立即消费掉，不执行其他操作*/ 
	if (GetOpenDoorLight())
	{
		SetOpenDoorLight(false);
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
	}
}

inline void StateDoorOpen(int *state)
{
	/*若开门状态下关门灯打开，则立即关门，消费关门灯，状态转移到正在关门*/ 
	if (GetCloseDoorLight())
	{
		SetDoor(GetNearestFloor(),false);
		SetCloseDoorLight(false);
		*state = DoorClosing;
	}
	/*若开门状态下开门灯打开，消费开门灯*/ 
	if (GetOpenDoorLight())
	{
		SetOpenDoorLight(false);
	}
	/*如果门开着，关掉该层的门，状态转移到正在关门状态*/ 
	if (IsDoorOpen(GetNearestFloor()))
	{
		SetDoor(GetNearestFloor(), false);
		*state = DoorClosing;
	}
	/*消费门内楼层灯按钮*/ 
	if (GetPanelFloorLight(GetNearestFloor()))
	{
		SetPanelFloorLight(GetNearestFloor(), false);
	}
}

void StateDoorClosing(int *state)
{
	/*开门灯按钮按下，立即开门，消费开门灯按钮，状态转移到门开状态*/ 
	if (GetOpenDoorLight())
	{
		SetDoor(GetNearestFloor(),true);

		SetOpenDoorLight(false);
		*state = DoorOpen;
	}
	/*关门灯按钮按下，消费关门灯按钮*/ 
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
	}
	/*关门过程中如果有障碍物，则门打开，状态转移到门开状态*/ 
	if (IsBeamBroken())
	{
		SetDoor(GetNearestFloor(),true);
		*state = DoorOpen;
	}
	/*门关闭后转移到Idle状态*/ 
	if (IsDoorClosed(GetNearestFloor()))
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
