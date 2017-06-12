include "stdafx.h"
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
	floor = IdleWhatFloorToGoTo(&up);//目标楼层
	if (floor > 0 && up)  SetMotorPower(1); //上升
	else if (floor > 0 && !up)  SetMotorPower(-1);//下降
	else if (GetOpenDoorLight() )//检查开门灯状态
	{
		SetDoor(GetNearestFloor(),true);//开门
		SetOpenDoorLight(false);//消费开门按钮
	}
	else if (GetCallLight(GetNearestFloor(),true))//获取当前楼层门外CallLight按钮状态
	{
		SetDoor(GetNearestFloor(),true);//开门
		SetCallLight(GetNearestFloor(),up,false);//消费当前楼层UP按钮
	}
	else if (GetCallLight(GetNearestFloor(),false ))
	{
		SetDoor(GetNearestFloor(), true);
		SetCallLight(GetNearestFloor(), false, false);//消费门外down按钮
	}

	else if(GetCloseDoorLight())
	{
		SetCloseDoorLight(false);//消费关门按钮
		return;
	}
	

}

void StateMovingUp(int *state)
{
	int floor;
    floor = GoingDownToFloor();
	if (fabs(GetFloor()-floor)>Lib_FloorTolerance)
	{
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);
	}
	if (fabs(GetFloor()-floor)<Lib_FloorTolerance)//到达
	{
		SetMotorPower(0);//停止
		SetDoor(floor, true);//开门
		floor = GetNearestFloor();//获取当前楼层;
		if (floor < Lib_FloorNum)//消费UP 按钮
			SetCallLight(floor, true, false);
		else if (floor == Lib_FloorNum)//最高层 消费DOWN按钮
			SetCallLight(floor, false, false);
		SetPanelFloorLight(floor, false);//消费当前楼层按钮
	}
}

void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor)>Lib_FloorTolerance)//正在下降
	{
		SetOpenDoorLight(false);//消费开门按钮
		SetCloseDoorLight(false);//消费关门按钮
    }
	if (fabs(GetFloor() - floor)<Lib_FloorTolerance)
	{
		SetMotorPower(0);//停止
		SetDoor(floor, true);//开门
		floor = GetNearestFloor();
		if (floor > 1) //消费DOWN按钮
			SetCallLight(floor, false, false);
		else if (floor == 1)//1楼消费UP 按钮
			SetCallLight(floor, true, false);
		SetPanelFloorLight(floor,false);//消费当前楼层按钮
     }
	
}

void StateDoorOpen(int *state)
{
	if (GetCloseDoorLight())//按了关门灯；
		SetDoor(GetNearestFloor(), false);
	else if (GetOpenDoorLight())//按了开门
	{
		SetDoor(GetNearestFloor(), true);//开门
		SetOpenDoorLight(false);//消费开门按钮
	}
	else if (IsDoorOpen(GetNearestFloor()))//开门结束后自动关门
		SetDoor(GetNearestFloor(), false);//开门
	
}

void StateDoorClosing(int *state)
{
	if (GetOpenDoorLight())//检查是否按了开门灯
	{
		SetDoor(GetNearestFloor(), true);//正在关门，转而开门
		SetOpenDoorLight(false);//消费开门按钮
	}
	else if (GetCloseDoorLight())// 检查是否按了关门按钮
	{
		SetDoor(GetNearestFloor(), false);//正在开门，物动作
		SetCloseDoorLight(false);//消费关门按钮
	}
	else if (IsBeamBroken())//检查是否有遮挡物
		SetDoor(GetNearestFloor(), true);//停止关门，转而开门
}

/***********************************************
* 状态机，每隔一定时间(如，100ms)被调用一次，采集系统的运行状态
***********************************************/
void main_control(int *state)
{
	if (IsElevatorRunning())  // 仿真正在运行
	{
		switch (*state)
		{
		case Idle:
			// Idle状态，一定时间无动作，自动到一楼
			if (GetNearestFloor() != 1) {
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

