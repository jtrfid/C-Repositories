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
	int floor, bool up;
	floor = IdleWhatFloorToGoTo(&up);
	if (floor > 0 && up)
	{
		SetMotorPower(1);
		*state = MovingUp;
	}//上升，进入Movingup状态
	if (floor > 0 && !up)
	{
		SetMotorPower(-1);
		*state = MovingDown;
	}//上升，进入Movingdown状态
	if (GetOpenDoorLight())
	{
		SetDoor(GetNearestFloor(), true);
		SetOpenDoorLight(true);
		*state = DoorOpen;
	}//门内开门按钮，开门，消费开门按钮，进入Dooropen状态
	if (GetCallLight(GetNearestFloor(), true))
	{
		SetDoor(GetNearestFloor(), true);
		SetCallLight(GetNearestFloor(), true, false);
		SetCallLight(GetNearestFloor(), false, false);
		*state = DoorOpen;
	}//门内呼叫按钮，开门，消费门外呼叫按钮，进入Dooropen状态
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
		return;
	}//此时门应该是关闭的，因此仅读取关门灯，并关闭关门灯，即消费按键行为，防止下一周期重复处理该按钮的行为
}

void StateMovingUp(int *state)
{
	int floor;
	floor = GoingUpToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;
		SetCallLight(GetNearestFloor(), false, false);//到达目标楼层，停止，开门，进入Dooropen状态，消费门外down按钮
		if (floor = Lib_FloorNum)
		{
			SetCallLight(GetNearestFloor(), false, false);//到达最高层消费门外down按钮
		}
		SetPanelFloorLight(GetNearestFloor(), false);//消费门内楼层为当前楼层的按钮
		if (GetOpenDoorLight() || GetCloseDoorLight())
		{
			SetOpenDoorLight(false);
			SetCloseDoorLight(false);
		}//运动状态，开关门失效，消费开/关按钮
	} 
}

void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;
		SetCallLight(GetNearestFloor(), false, false);////到达目标楼层，停止，开门，进入Dooropen状态，消费门外down按钮
		if (floor == 1)
		{
		   SetCallLight(GetNearestFloor(),true,false);
		}//到了1层消费门外up按钮
		SetPanelFloorLight(GetNearestFloor(), false);//消费门内楼层为当前楼层的按钮
	}
	if (GetOpenDoorLight() || GetCloseDoorLight())
	{
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);
	}//运动状态，开关门失效，消费开/关门按钮
}

void StateDoorOpen(int *state)
{
	if (GetCloseDoorLight())
	{
		SetDoor(GetNearestFloor(), false);
		SetCloseDoorLight(false);
		*state = DoorClosing;//正在开门。按了关门，关门，进入Doorclosing状态
	}
	if (IsDoorOpen(GetNearestFloor()))
	{
		SetDoor(GetNearestFloor(), true);
		*state = DoorClosing;
	}//开门结束后自行关门，进入Doorclosing状态
	if (GetOpenDoorLight())
	{
		SetOpenDoorLight(false);
	}//正在开门按了开门，无动作，消费开门按钮
}

void StateDoorClosing(int *state)
{
	if (GetOpenDoorLight())
	{
		SetDoor(GetNearestFloor(), true);
		SetOpenDoorLight(false);
		*state = DoorOpen;//正在关门按了开门灯，开门，消费开门按钮，进入Dooropen状态
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
		*state = DoorClosing;//正在关门按了关门灯，无动作，消费关门按钮
	}
	if (IsBeamBroken())
	{
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;//红外探测到遮挡，开门，进入Dooropen状态
	}
	if (IsDoorClosed(GetNearestFloor()))
	{
		*state = Idle;//关门结束后进入Idle状态
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
