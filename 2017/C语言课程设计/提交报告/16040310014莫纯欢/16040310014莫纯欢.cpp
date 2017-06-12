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
	floor=IdleWhatFloorToGoTo(&up);
	if (floor > 0 && up)
	{
		SetMotorPower(1);//启动电机（上升）
		*state =MovingUp;//转变
		printf("Transition:from Idel to MovingUp.\n");//transition
	}
	else if (floor > 0 && !up)
	{
		SetMotorPower(-1);//启动电机（下降）
		*state = MovingDown;//转变
		printf("Transition；from Idel to MovingDown.\n");//transition
	}
	else if (GetOpenDoorLight())
	{
		SetDoor(GetNearestFloor(), true);//开门
		SetOpenDoorLight(false);//消费门内按钮灯
		*state = DoorOpen;//转变
		printf("Transition:from Idel to DoorOpen.\n");//transition
	}
	else if (GetCallLight(floor, up))
	{
		SetDoor(floor, true);//开门
		SetCallLight(GetNearestFloor(), up, false);//消费门外按钮灯
		*state = DoorOpen;//转变
		printf("Transition: from Idle to DoorOpen.\n");//transition
	}
	
	else if (GetCloseDoorLight()) 
	{ 
		SetCloseDoorLight(false); //消费按钮灯
	    return; //返回
	}



}

void StateMovingUp(int *state)
{
	int floor; 
	bool up;
	floor = GoingDownToFloor();
	up = GetCallLight(floor, true);
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);//停止电机
		SetDoor(GetNearestFloor(), true);//开门
		*state = DoorOpen;//转变
		printf("Transition:from MovingUp to DooorOpen.\n");//transition
		SetCallLight(GetNearestFloor(), true, false);//消费门外up按钮灯
		if (GetNearestFloor() == Lib_FloorNum)//到达三楼
		{
			SetCallLight(GetNearestFloor(), false, false);//消费门外down按钮灯
		}
		SetPanelFloorLight(GetNearestFloor(), false);//消费当前楼层按钮灯
	}
	else if (GetOpenDoorLight || GetCloseDoorLight)
	{
		SetOpenDoorLight(false);//消费门内开门按钮灯
		SetCloseDoorLight(false);//消费门内关门按钮灯
	}
}

void StateMovingDown(int *state)
{
	int floor; bool up;
	floor = GoingDownToFloor();
	up = GetCallLight(floor, true);
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);//停止电机
		SetDoor(GetNearestFloor(), true);//开门
		*state = DoorOpen;//转变
		printf("Transition :from MovingDown to DoorOpen./n");//transition
		SetCallLight(GetNearestFloor(), false, false);//消费门外down按钮灯
		if (GetNearestFloor() == 1)//到了一楼
		{
			SetCallLight(GetNearestFloor(), true, false);//消费门外up按钮灯
		}
		SetPanelFloorLight(GetNearestFloor(), false);//消费当前楼层按钮灯
	}
	else if (GetOpenDoorLight() || GetCloseDoorLight())//（防意外开门，无现实意义指示直接消费）
	{
		SetOpenDoorLight(false);//消费门内开门按钮灯
		SetCloseDoorLight(false);//消费门内关门按钮灯
	}

}

void StateDoorOpen(int *state)
{
	
	bool up;
	if (GetCloseDoorLight())
	{
		SetDoor(GetNearestFloor(), false);//关门
		SetCloseDoorLight(false);//消费关门按钮灯
		*state = DoorClosing;//转变
		printf("Transition:from MovingDown to DoorClosing.\n");//transition
	}
	else if (IsDoorOpen(GetNearestFloor()))//门是打开的
	{
		SetDoor(GetNearestFloor(), false);//关门
		*state = DoorClosing;//转变
		printf("Transition:from DoorOpen to DoorCloosing.\n");//transition
	}
	else if (GetOpenDoorLight())//重复按按钮
	{
		SetOpenDoorLight(false);//消费开门按钮
	}
}

void StateDoorClosing(int *state)
{
	bool up;
	if (GetOpenDoorLight())
	{
		SetDoor(GetNearestFloor(), true);//开门
		SetOpenDoorLight(false);//消费开门按钮
		*state = DoorOpen;//转变
		printf("Transition:from DoorClosing to DoorOpen.\n");//transition
	}
	else if (GetCloseDoorLight())//重复按按钮
	{
		SetCloseDoorLight(false);//消费关门按钮
	}
	else if (IsBeamBroken())//有障碍物
	{
		SetDoor(GetNearestFloor(), true);//开门
		*state = DoorOpen;//转变
		printf("Transition:from DoorClosing to DoorOpen.\n");//transition
	}
	else if (IsDoorClosed(GetNearestFloor()))//门已关闭
	{
		*state = Idle;//转变
		printf("Transition:from DoorClosd to Idle.\n");//transition
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
