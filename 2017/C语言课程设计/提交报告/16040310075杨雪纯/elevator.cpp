#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ElevatorLib.h"

/**********************************************
 * Idle状态，电梯停止在某楼层，门是关闭的，处于静止状态，等待相关事件的发生，从而转换到下一个状态。
 **********************************************/
///////////16040310075   杨雪纯
void StateIdle(int *state)
{
	if (PanelFloorLight()||GetCallLight())
	{
		int floor; bool up;
		floor=IdleWhatFloorToGoTo(&up);
		if (floor>0&&up)
		{
			SetMotorPower(1);//上升
			*state = MovingUp;
			printf("Transition:form Idle to MovingUp");
		}
		else
		{
			SetMotorPower(-1);
			*state = MovingDown;
			printf("Transition:from Idle to MovingDown");

		}
		if (GetOpenDoorLight())//门内按开门
		{
			SetDoor(floor,1);//开门
			SetOpenDoorLight(floor,0);//消费开门键
			*state = DoorOpen;
			printf("Transition:from Idle to DoorOpen");
		}
		if (GetCallLight())//门外按开门
		{
			SetDoor(floor,1);//开门
			SetCallLight(floor,1,0);//消费up按钮
			SetCallLight(floor,0,0);//消费down按钮
            *state = DoorOpen;
			printf("Transition:from Idle to DoorOpen");
		}
		if (GetCloseDoorLight())
		{
			SetCloseDoorLight(false);//消费关门按钮
			return;
		}
	}
}

void StateMovingUp(int *state)
{
	if (PanelFloorLight()||GetCallLight())
	{
		int floor;
		floor = GoingUpToFloor();
		if(fabs(GetFloor()-floor)<Lib_FloorTolerance)//到达目标楼层
		{
			SetMotorPower(0);//停止
			SetDoor(floor,1);//开门
			*state = DoorOpen;
			printf("Transition:from MovingUp to DoorOpen");
			SetCallLight(floor,1,0);//消费向上按钮
			if (Lib_FloorNum)
			{
				SetCallLight(floor,0,0);//消费向下按钮
				SetPanelFloorLight(floor,0);
				GetNearestFloor();
			}
        }

	}
	if (GetOpenDoorLight() || GetCloseDoorLight())//上升状态门内开关时
	{
		SetOpenDoorLight(0);
		SetCloseDoorLight(0);//消费开和关
	}

}
void StateMovingDown(int *state)
{
	if (PanelFloorLight()||GetCallLight())
	{
		int floor;
		floor = GoingDownToFloor();
		if (fabs(GetFloor() - floor) < Lib_FloorTolerance)//到目标楼层
		{
			SetMotorPower(0);
			SetDoor(floor,1);
			*state = DoorOpen;
			printf("Transition:from MovingDown to DoorOpen");
			SetCallLight(floor,0,0);//消费门外下按钮
        }
		if (Lib_FloorTolerance)//到1层
			SetUpLight(floor,0);//消费门外上按钮
		SetPanelFloorLight(floor);
		GetNearestFloor(floor);
	}
	if (GetOpenDoorLight()||GetCloseDoorLight())//下降过程按开关门
	{
		SetOpenDoorLight(0);
		SetCloseDoorLight(0);
	}

}

void StateDoorOpen(int *state)//开门状态
{
	int floor;
	floor=GetNearestFloor();
	if (GetCloseDoorLight())//手动关门
	{
		SetDoor(floor,0);//关门
		SetCloseDoorLight(0);
		*state = DoorCloseing;
		printf("Transition:from DoorOpen to DoorCloseing");
    }
	if (IsDoorOpen())//开门结束
	{
		SetDoor(floor,0);//关门
		*state = DoorCloseing;
		printf("Transition:from DoorOpen to DoorCloseing");
	}
	if (GetOpenDoorLight())
		SetOpenDoorLight(0);
}

void StateDoorClosing(int *state)
{
	int floor;
	floor=GetNearestFloor();
	if (GetOpenDoorLight())//关门状态下按了开门键
	{
		SetDoor(floor,1);//开门
		SetOpenDoorLight(0);
		*state = DoorOpen;
		printf("Transition:from DoorCloseing to DoorOpen");
	}
	if (GetCloseDoorLight())//关门状态按关门
		SetCloseDoorLight(0);
	if (IsBeamBroken())//障碍物
	{
		SetDoor(floor,1);//开门
		*state = DoorOpen;
		printf("Transition:from DoorCloseing to DoorOpen");
	}
	if (IsDoorClosed())//门关完了
	{
		*state = Idel;
		printf("Transition:from DoorCloseing to Idel");

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
