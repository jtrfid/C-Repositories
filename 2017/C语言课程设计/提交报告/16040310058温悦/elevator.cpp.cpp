#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ElevatorLib.h"

/**********************************************
 * Idle状态，电梯停止在某楼层，门是关闭的，处于静止状态，等待相关事件的发生，从而转换到下一个状态。
 **********************************************/
/*Idle状态函数*/
void StateIdle(int *state)
{
	int floor; bool up;
	floor = IdleWhatFloorToGoTo(&up);
	if (floor > 0 && up)//Event 检查门内楼层按钮和门外呼叫按钮
	{
		SetMotorPower(1);//Transition
		*state = MovingUp;
		printf("transition:from Idle to MovingUp.\n");
		SetCallLight(floor, true, false);//关闭门外呼叫up灯
		SetCallLight(floor, false, false);//关闭门外呼叫down灯

	}
	if (floor > 0 && !up)
	{
		SetMotorPower(-1);//Transition
		*state = MovingDown;
		printf("transition:from Idle to MovingDown.\n");
		SetCallLight(floor, true, false);//关闭门外呼叫up灯
		SetCallLight(floor, false, false);//关闭门外呼叫down灯
	}
	if (GetOpenDoorLight())//Event 检查门内开门按钮
	{
		SetDoor(floor,true);//Transition
		*state = DoorOpen;
		printf("transition:from Idle to DoorOpen.\n");
		SetOpenDoorLight(false);//消费开门按钮
	}
	floor = GetNearestFloor();
	if ((up&&GetCallLight(floor,true))||(!up&&GetCallLight(floor,false)))//Event 检查门外up/down按钮
	{
		SetDoor(floor,true);//Transition
		*state = DoorOpen;
		printf("transition:from Idle to DoorOpen.\n");
		SetCallLight(floor, true, false);//消费门外up按钮
		SetCallLight(floor, false, false);//消费门外down按钮
	}
	if (GetCloseDoorLight())//Event
	{
		SetCloseDoorLight(false);//Transition消费关门按钮
		return;
	}
}
void StateMovingUp(int *state)
{
	int floor;
	floor = GoingUpToFloor();//目标楼层
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)//Event 到达目标楼层
	{
		SetMotorPower(0);
		SetDoor(floor,true);//Transition
		*state = DoorOpen;
		printf("transition:from Moving Up to DoorOpen.\n");
		SetCallLight(GetNearestFloor(), true, false);//消费门外up按钮
		if (GetNearestFloor() == Lib_FloorNum)
			SetCallLight(GetNearestFloor(), false, false);//消费门外down按钮
		SetPanelFloorLight(floor, false);//消费梯内楼层按钮
	}
	/*安全设置，无动作，消费开/关门按钮*/
	if(GetOpenDoorLight())
	SetOpenDoorLight(false);
	if(GetCloseDoorLight())
	SetCloseDoorLight(false);
}
void StateMovingDown(int *state)
{
	int floor;
	floor = GoingUpToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)//Event 到达目标楼层
	{
		SetMotorPower(0);//停止
		SetDoor(floor,true);//Transition
		*state = DoorOpen;
		printf("transition:from Moving Down to DoorOpen.\n");
		SetCallLight(GetNearestFloor(), false, false);//消费门外down按钮
		if (GetNearestFloor()==1)
			SetCallLight(GetNearestFloor(), true, false);//消费门外up按钮
		SetPanelFloorLight(floor,false);//消费当前楼层按钮
	}
	/*安全设置无动作，消费开/关门按钮*/
	if (GetOpenDoorLight())
		SetOpenDoorLight(false);
	if (GetCloseDoorLight())
		SetCloseDoorLight(false);
}
void StateDoorOpen(int *state)
{
	int floor;
	floor = GetNearestFloor();
	if (GetCloseDoorLight())//Event
	{
		SetDoor(floor,false);//Transition
		*state = DoorClosing;
		printf("transition:from DoorOpen to DoorClosing.\n");
		SetCloseDoorLight(false);//消费关门按钮
	}
	if (IsDoorOpen(floor))//Event
	{
		SetDoor(floor, false);//Transition开门结束后自动关门
		*state = DoorClosing;
		printf("transition:from DoorOpen to DoorClosing.\n");
	}
	if (GetOpenDoorLight())
	{
		SetOpenDoorLight(false);//正在开门，按了开门灯，无动作消费开门按钮
	}
}
void StateDoorClosing(int *state)
{
	int floor;
	floor = GetNearestFloor();
	if (GetOpenDoorLight())//Event
	{
		SetDoor(floor,true);//Transition正在关门，按了开门灯，转而开门
		*state = DoorOpen;
		printf("transition:from Doorclosing to Dooropen.\n");
		SetOpenDoorLight(false);//消费开门按钮
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);//正在关门，按了关门灯，无动作，消费关门按钮
	}
	if (IsBeamBroken())//Event探测到障碍
	{
		SetDoor(floor,true);//Transition开门
		*state = DoorOpen;
		printf("transition:from Doorclosing to Dooropen.\n");
	}
	if (IsDoorClosed(floor))//Event关门结束
	{
		*state = Idle;
		printf("transition:from Doorclosing to Idle.\n");
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
