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
	floor = IdleWhatFloorToGoTo(&up);// 静态监测,电梯处于空闲状态, 确定下一步的运动方向和所到楼层(目标楼层)
	if (floor > 0 && up)// 门外up呼叫按钮
	{
		SetMotorPower(1); // 电机运转，上升
		*state = MovingUp;//状态变为上升
		printf("Transition: from Idle to MovingUp.\n");
	}

	if (floor > 0 && !up) // 门外down呼叫按钮
	{
		SetMotorPower(-1);// 电机运转，下降
		*state = MovingDown;//状态变为下降
		printf("Transition: from Idle to MovingDown.\n");
	}
	if (GetOpenDoorLight())//门内开门按钮呼叫
	{
		SetDoor(GetNearestFloor(), true); // 获取最近楼层或所在楼层，开门
		SetOpenDoorLight(false);//消费开门按钮灯
		*state = DoorOpen;//状态变为开门
		printf("Transition: from Idle to DoorOpen.\n");
	}
	if (GetCallLight(GetNearestFloor(), true))//门外上升按钮呼叫
	{
		SetCallLight(GetNearestFloor(), true, false);//消费所在楼层上升按钮灯
		SetDoor(GetNearestFloor(), true); // 获取最近楼层或所在楼层，开门

		*state = DoorOpen;//状态转变为，开门

		printf("Transition: from Idle to DoorOpen.\n");

	}
	if (GetCallLight(GetNearestFloor(), false))//门外下降按钮呼叫
	{
		SetCallLight(GetNearestFloor(), false, false); //消费所在楼层上升按钮灯
		SetDoor(GetNearestFloor(), true); //获取最近楼层或所在楼层，开门

		*state = DoorOpen;//状态转变为，开门

		printf("Transition: from Idle to DoorOpen.\n");
	}
	if (GetCloseDoorLight())//门内关门按钮呼叫

	{
		SetCloseDoorLight(false);//消费关门按钮灯
		return;
	}
}

void StateMovingUp(int *state)
{
	int floor;
	floor = GoingUpToFloor();//获取上升目的楼层

	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)//  到达目标楼层
	{
		SetMotorPower(0);//电动机停止
		SetDoor(GetNearestFloor(), true); // 开门
		SetPanelFloorLight(GetNearestFloor(), false);//消费门内楼层为当前楼层的按钮
		SetCallLight(GetNearestFloor(), true, false);//消费门外上升按钮
		SetCallLight(GetNearestFloor(), false, false);//消费门外下降按钮

		*state = DoorOpen;//状态转变为开门

		printf("Transition: from MovingUp to DoorOpen.\n");
	}
	if (GetOpenDoorLight() )// 获取门内开门按钮
	{
		SetOpenDoorLight(false);//消费开门按钮
		
	}
	if (GetCloseDoorLight())// 获取门内开门按钮
	{
		
		SetCloseDoorLight(false);//消费关门按钮
	}
}
void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();//获取下降目的楼层

	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)// 到达目的楼层 
	{
		SetMotorPower(0);

		SetDoor(GetNearestFloor(), true); // 开门
		SetPanelFloorLight(GetNearestFloor(), false);//消费门内楼层为当前楼层的按钮
		*state = DoorOpen;
		SetCallLight(GetNearestFloor(), true, false);//消费门外上升按钮
		SetCallLight(GetNearestFloor(), false, false);//消费门外下降按钮
		printf("Transition: from MovingUp to DoorOpen.\n");
	}
	if (GetOpenDoorLight())// 门内开门按钮呼叫
	{
		SetOpenDoorLight(false);//消费开门按钮

	}
	if (GetCloseDoorLight())// 门内关门按钮呼叫 
	{

		SetCloseDoorLight(false);//消费门内关门按钮
	}
}

void StateDoorOpen(int *state)
{

	if (GetCloseDoorLight())// 门内关门按钮呼叫
	{
		SetDoor(GetNearestFloor(), false); // 关门
		SetCloseDoorLight(false);//消费关门按钮
		*state = DoorClosing;//状态变为正在门关闭
		printf("Transition: from DoorOpen toDoorClosing.\n");
	}
	if (IsDoorOpen(GetNearestFloor()))// 开门结束后
	{
		SetDoor(GetNearestFloor(), false); // 关门
		*state = DoorClosing;//状态变为正在关门
		printf("Transition: from DoorOpen toDoorClosing.\n");
	}
	if (GetOpenDoorLight())// 门内开门按钮呼叫
		SetOpenDoorLight(false);//消费开门按钮
}
void StateDoorClosing(int *state)
{

	if (GetOpenDoorLight())//门内开门按钮呼叫
	{
		SetDoor(GetNearestFloor(), true); // 开门
		*state = DoorOpen;//状态变为开门
		printf("Transition: from DoorClose toDoorOpen.\n");
	}
	if (GetCloseDoorLight())// 门内关门按钮呼叫
	{
		SetCloseDoorLight(false);//消费关门按钮
	}
	if (IsBeamBroken())// 红外感觉到门中有遮挡物
	{
		SetDoor(GetNearestFloor(), true); // 开门
		*state = DoorOpen;//状态变为开门
		printf("Transition: from DoorOpen toDoorOpen.\n");
	}
	if (IsDoorClosed(GetNearestFloor()))// 关门结束 
		*state = Idle;//状态变为闲置状态
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
