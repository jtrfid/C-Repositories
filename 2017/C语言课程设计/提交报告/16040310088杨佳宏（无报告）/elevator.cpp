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
	bool s;
	int floor_now = GetNearestFloor();
	floor = IdleWhatFloorToGoTo(&up);//Event

	if (floor>0 && up)
	{
		SetMotorPower(1);
		*state = MovingUp;//Transition
		printf("Transition:from Idles to MovingUp.\n");//s1
		return;

	}
	
	else if (floor >0 && !up)
	{
		SetMotorPower(-1);
		*state = MovingDown;//Transition
		printf("Transition：from Idle to MovingDown.\n");//s2
		return;

	}
	if (GetOpenDoorLight())//按动门内开门按钮
	{
		SetDoor(floor, true);
		*state = DoorOpen;//Transition
		printf("Transition:from Idle to Dooropen.\n");//s3
		return;
	}

	else if (GetCallLight(floor_now, true))//按门外UP按钮
	{
		*state = DoorOpen;
		printf("Transition:from Idle to Dooropen.\n");
		return;
	}


	else if (GetCallLight(floor_now, false))//按门外Down按钮
	{
		*state = DoorOpen;
		printf("Transition:from Idle to Dooropen.\n");
		return;
	}

	else if (GetCloseDoorLight())//Idel状态门内关门按键失效
	{
		SetCloseDoorLight(false);
		*state = Idle;
		printf("Transition:from Idle to Idle.\n");
		return;
	}


}

void StateMovingUp(int *state)
{
	int floor;
	bool up;
	bool s;
	floor = GoingUpToFloor();//Event
	if (GetOpenDoorLight())//运动状态，开门失效，消费开门按钮
	{
		SetOpenDoorLight(false);

	}

	if (GetCloseDoorLight())//运动状态，关门失效，消费关门按钮
	{
		SetCloseDoorLight(false);
	}

	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)//UP s4

	{
		SetMotorPower(0);//停止
		SetDoor(floor, true);
		*state = DoorOpen;
		printf("Transition:from MovingUp to DoorOpen.\n");

		if (GetCallLight(floor, true))//消费门外UP按钮

		{

			SetCallLight(floor, true, false);
			
		}
		return;

	}
	

	
	else if (floor = Lib_FloorNum)//到达最高层消费门外down按钮
	{
		if (GetCallLight(floor, true))
		{
			SetCallLight(floor, false, false);
		}
	}

	floor = GetNearestFloor();	//获取当前楼层
	if (GetPanelFloorLight(floor))
	{
		SetPanelFloorLight(floor, false);//消费门内按钮

	}


	return;
}

		void StateMovingDown(int *state)
{
	int floor;
	bool up;
	bool s;
	floor = GoingDownToFloor();
	if (GetOpenDoorLight())//运动状态，开关门失效，消费开关门
	{
		SetOpenDoorLight(false);
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
	}
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)//s5
	{
		SetMotorPower(0);
		SetDoor(floor, true);
		*state = DoorOpen;
		printf("Transition:from MovingDown to DoorOpen.\n");

		if (GetCallLight(floor, false))//消费门外down按钮
		{
			SetCallLight(floor, false, false);
		}
		return;

	}
	floor = GetNearestFloor();
	if (floor==1 )//到达1层消费门外up按钮 
	{ 
		if (GetCallLight(floor, true))
		{
			SetCallLight(floor, true, false);
		}
	
	}
	floor = GetNearestFloor();	//获取当前楼层
	if (GetPanelFloorLight(floor))
	{
		SetPanelFloorLight(floor, false);//消费门内按钮

	}

	return;
}

void StateDoorOpen(int *state)
{
	int floor;
	bool up; bool s;
	floor = GetNearestFloor();//获取当前楼层
	if (GetOpenDoorLight())//正在开门消费门内开门按钮
	{
		SetOpenDoorLight(false);
	}
	if (GetCallLight(floor, true))//开门时消费门外按钮(up)
	{
		SetDoor(floor, true);
		SetCallLight(floor, true, false);
		*state = DoorOpen;
		return;
	}
	if (GetCallLight(floor, false))//开门时消费门外按钮(down)
	{
		SetDoor(floor, true);
		SetCallLight(floor, false, false);
		*state = DoorOpen;
		return;
	}

	if (GetCloseDoorLight())// Event:按了门内关门按钮 s6
	{
		SetDoor(floor, false);
		//SetCloseDoorLight(false);
		*state = DoorClosing;
		printf("Transition:from DoorOpen to DoorClosing./n");
		return;
	}
	if (IsDoorOpen(floor))//Event:开门结束后自动关门 s6
	{
		SetDoor(floor, false);
		*state = DoorClosing;
		printf("Transition:from DoorOpen to DoorClosing./n");
		return;
	}

}

void StateDoorClosing(int *state)
{
	int floor;
	bool up; bool s;
	floor = GetNearestFloor();//获取当前楼层
	if (GetCloseDoorLight())//Event:正在关门时按关门灯p5
	{
		SetCloseDoorLight(false);//消费关门按钮
	}


	if (GetOpenDoorLight())//Event:正在关门时按门内开门按钮s7
	{
		SetDoor(floor, true);
		*state = DoorOpen;
		printf("Transition:from DoorClosing to DoorOpen.\n");
		return;
	}
	if (IsBeamBroken())//正在关门感应到障碍物
	{
		SetDoor(floor, true);
		*state = DoorOpen;
		printf("Transition:from DoorClosing to DoorOpen.\n");
		return;

	}


	else if (IsDoorClosed(floor))//关门结束
	{
		SetDoor(floor, false);
		*state = Idle;
		printf("Transition:from DoorClosing to Idel.\n");
		return;

	}


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

