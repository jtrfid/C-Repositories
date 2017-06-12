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
	int Floor,PresentFloor;
	bool up;
	Floor = IdleWhatFloorToGoTo(&up);
	PresentFloor = GetNearestFloor();
	if (Floor > 0 && up)
	{
		SetMotorPower(1);
		*state = MovingUp;
	}
	if (Floor > 0 && !up)
	{
		SetMotorPower(-1);
		*state = MovingDown;
	}
	if (GetOpenDoorLight())/*检测门内开门按钮*/
	{
			SetDoor(PresentFloor, true);/*开门*/
			SetOpenDoorLight(false);/*消费开门按钮*/
			*state = DoorOpen;
	}
	if (GetCallLight(PresentFloor, up))/*检测门外up按钮*/
	{
		SetDoor(PresentFloor, true);/*开门*/
			SetCallLight(PresentFloor, up, false);/*消费按钮*/
			*state = DoorOpen;
	}
	if (GetCallLight(PresentFloor, !up))/*检测门外down按钮*/
	{
		SetDoor(PresentFloor, true);/*开门*/
			SetCallLight(PresentFloor, !up, false);/*消费按钮*/
			*state = DoorOpen;
	}
	if (GetCloseDoorLight())/*检测门内关门按钮*/
	{
			SetCloseDoorLight(false);
			*state = DoorClosing;
			return;
	}
	
}
/*MovingUp状态*/
void StateMovingUp(int *state)
{

	SetMotorPower(1);     /*向上移动*/
	int Floor;
	Floor = GoingUpToFloor();
	if (fabs(GetFloor() - Floor) < Lib_FloorTolerance)/*到达目标楼层，停止，开门*/
	{
		SetMotorPower(0);
		SetDoor(Floor, true);
		*state = DoorOpen;
		SetCallLight(Floor, true, false);/*消费门外up按钮*/
		SetPanelFloorLight(Floor, false);/*消费门内楼层按钮灯*/
	}
	if (fabs(GetFloor() - 3) < Lib_FloorTolerance)/*到达3楼*/
	{
		SetMotorPower(0);
		SetDoor(Floor, true);
		*state = DoorOpen;
		SetCallLight(3, false, false);/*消费门外down按钮*/
		SetPanelFloorLight(3, false);/*消费门内按钮*/
	}
	if (GetOpenDoorLight())     /*灭掉开关门灯*/
		SetOpenDoorLight(false);
	if (GetCloseDoorLight())
		SetCloseDoorLight(false);
}
void StateMovingDown(int *state)
{
	SetMotorPower(-1);     /*向下移动*/
	int Floor;
	Floor = GoingDownToFloor();
	if (fabs(GetFloor() - Floor) < Lib_FloorTolerance)/*到达目标楼层，停止，开门*/
	{
		SetMotorPower(0);
		SetDoor(Floor, true);
		*state = DoorOpen;
		SetCallLight(Floor, true, false);/*消费门外down按钮*/
		SetPanelFloorLight(Floor, false);/*消费门内按钮*/
	}
	if (fabs(GetFloor() - 1) < Lib_FloorTolerance)/*到达1层*/
	{
		SetMotorPower(0);
		SetDoor(Floor, true);
		*state = DoorOpen;
		SetCallLight(1, false, false);/*消费门外up按钮*/
		SetPanelFloorLight(1, false);/*消费门内当前楼层的按钮*/
	}

	if (GetOpenDoorLight())     /*灭掉开关门灯*/
		SetOpenDoorLight(false);
	if (GetCloseDoorLight())
		SetCloseDoorLight(false);

	
}
/*DoorOpen状态*/
void StateDoorOpen(int *state)
{
	int PresentFloor;
	PresentFloor = GetNearestFloor();
	if (GetCloseDoorLight())/*检测门内关门按钮*/
	{
		SetDoor(PresentFloor, false);/*关门*/
		SetCloseDoorLight(false);/*消费关门按钮*/
		*state = DoorClosing;/*转入关门状态*/
	}
	if (IsDoorOpen(PresentFloor))/*检测门的状态，自动关门*/
	{
		SetDoor(PresentFloor, false);
		*state = DoorClosing;
	}
	if (GetOpenDoorLight())
		SetOpenDoorLight(false);
}
/*DoorClosing状态*/
void StateDoorClosing(int *state)
{
	int PresentFloor;
	PresentFloor = GetNearestFloor();
 if (GetOpenDoorLight())    /*检查门内开关按钮*/
	{
		SetDoor(PresentFloor, true);
		SetCloseDoorLight(false);   /*消费开门灯*/
		*state = DoorOpen;    /*开门*/
	}
	if (GetCloseDoorLight())    /*检查关门灯，正在关门，无动作，消费关门灯*/
		SetCloseDoorLight(false);
	if (IsBeamBroken())   /*检测是否有障碍物*/
	{
		SetDoor(PresentFloor, true);/*有障碍物，开门*/
		*state = DoorOpen;
	}
	if (IsDoorClosed(PresentFloor))
		*state = Idle;
}
/***********************************************
* 主控循环： 状态机，每隔一定时间(如，100ms)被调用一次，采集系统的运行状态
***********************************************/
void main_control(int *state)
{
	if (IsElevatorRunning())  // 仿真正在运行
	{
		switch (*state)
		{
		case Idle: 
			if (GetNearestFloor() != 1) 
			{
				AutoTo1Floor();// Idle状态，一定时间无动作，自动到一楼
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