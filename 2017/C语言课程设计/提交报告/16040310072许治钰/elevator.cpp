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
	floor = IdleWhatFloorToGoTo(&up);//静态检测，检测目标楼层
	if (floor > 0 && up)//检测目标楼层是否高于电梯所在楼层
	{
		SetMotorPower(1);//电机启动，电梯向上运行
		*state = MovingUp;
		printf("Transition: from Idle to MovingUp.\n");
	}
	else if (floor > 0 && !up)//检测目标楼层是否低于电梯所在楼层
	{
		SetMotorPower(-1);//电机启动，电梯向下运行
		*state = MovingDown;
		printf("Transition: from Idle to MovingDown.\n");
	}
	else if (GetOpenDoorLight())//检测门内开门按钮灯的状态
	{
		SetOpenDoorLight(false);//关闭开门按钮灯
		SetDoor(floor, true); //打开电梯门
		*state = DoorOpen;
		printf("Transition: from Idle to DoorOpen.\n");
	}
	else if (GetCloseDoorLight())//检测门内关门按钮灯的状态
	{
		SetCloseDoorLight(false);//关闭关门按钮灯
		return;
	}
	floor=GetNearestFloor();//获取电梯所在楼层数
	if (GetCallLight(floor, false))//检测门外向下按钮灯的状态
	{
		SetCallLight(floor, false, false); //关闭门外向下按钮灯
		SetDoor(floor, true);//打开电梯门
		*state = DoorOpen;
		printf("Transition: from Idle to DoorOpen.\n");
	}
	if (GetCallLight(floor, true))//检测门外向上按钮灯的状态
	{
		SetCallLight(floor, up, false);//关闭门外向上按钮灯
		SetDoor(floor, true);//打开电梯门
		*state = DoorOpen;
		printf("Transition: from Idle to DoorOpen.\n");
	}

}
/**********************************************
* MovingUp状态：电梯向上移动，移动过程中电梯门无法开关。
**********************************************/

void StateMovingUp(int *state)
{
	int floor;
	floor = GoingUpToFloor(); //动态监测，检测目标楼层
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)//检测是否到达目标楼层
	{
		if (GetCallLight(floor, true))//检测门外向上按钮灯的状态
		{
			SetCallLight(floor, true, false);//关闭门外向上按钮灯
			SetPanelFloorLight(floor, false);//关闭门内电梯所在楼层的按钮灯
			SetMotorPower(0);//电机停止
			SetDoor(floor, true);//打开电梯门
			*state = DoorOpen;
			printf("Transition: from MovingUp to DoorOpen.\n");
			return;
		}
		if (GetCallLight(floor, false))//检测门外向下按钮灯的状态
		{
			SetCallLight(floor, false, false); //关闭门外向下按钮灯
			SetPanelFloorLight(floor, false);//关闭门内电梯所在楼层的按钮灯
			SetMotorPower(0);//电机停止
			SetDoor(floor, true);//打开电梯门
			*state = DoorOpen;
			printf("Transition: from MovingUp to DoorOpen.\n");
			
		}
		SetPanelFloorLight(floor, false);//关闭门内电梯所在楼层的按钮灯
		SetMotorPower(0);//电机停止
		SetDoor(floor, true);//打开电梯门
		*state = DoorOpen;
		printf("Transition: from MovingUp to DoorOpen.\n");
	}
	else if (GetOpenDoorLight())//检测门内开门按钮灯的状态
	{
		SetOpenDoorLight(false);//关闭开门按钮灯
		return;
	}
	else if (GetCloseDoorLight())//检测门内关门按钮灯的状态
	{
		SetCloseDoorLight(false);//关闭关门按钮灯
		return;
	}
}
/**********************************************
* MovingDown状态：电梯向下移动，移动过程中电梯门无法开关。
**********************************************/

void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();//动态监测，检测目标楼层
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)//检测是否到达目标楼层
	{
		if (GetCallLight(floor, true))//检测门外向上按钮灯的状态
		{
			SetCallLight(floor, true, false);//关闭门外向上按钮灯
			SetMotorPower(0);//电机停止
			SetDoor(floor, true);//打开电梯门
			SetPanelFloorLight(floor, false);//关闭门内电梯所在楼层的按钮灯
			*state = DoorOpen;
			printf("Transition: from MovingDown to DoorOpen.\n");
			return;
		}
		if (GetCallLight(floor, false))//检测门外向下按钮灯的状态
		{
			SetCallLight(floor, false, false);//关闭门外向下按钮灯
			SetMotorPower(0);//电机停止
			SetDoor(floor, true);//打开电梯门
			SetPanelFloorLight(floor, false);//关闭门内电梯所在楼层的按钮灯
			*state = DoorOpen;
			printf("Transition: from MovingDown to DoorOpen.\n");
		}
		SetMotorPower(0);//电机停止
		SetDoor(floor, true);//打开电梯门
		SetPanelFloorLight(floor, false);//关闭门内电梯所在楼层的按钮灯
		*state = DoorOpen;
		printf("Transition: from MovingDown to DoorOpen.\n");
	}
	else if (GetOpenDoorLight())//检测门内开门按钮灯的状态
	{
		SetOpenDoorLight(false);//关闭开门按钮灯
		return;
	}
	else if (GetCloseDoorLight())//检测门内关门按钮灯的状态
	{
		SetCloseDoorLight(false);//关闭关门按钮灯
		return;
	}
}
/**********************************************
* DoorOpen状态: 打开电梯门，若电梯内开门按钮灯亮则关闭开门按钮灯。
**********************************************/

void StateDoorOpen(int *state)
{
	int floor;
	floor = GetNearestFloor();//检测当前电梯所在楼层
	if (GetCloseDoorLight())//检测门内关门按钮灯的状态
	{
		SetDoor(floor, false);//关闭电梯门
		SetCloseDoorLight(false);//关闭关门按钮灯
		*state = DoorClosing;
		printf("Transition: from DoorOpen to DoorClosing.\n");
	}
	else if (IsDoorOpen(floor))//检测电梯门是否是打开的
	{
		SetDoor(floor, false);//关闭电梯门
		*state = DoorClosing;
		printf("Transition: from DoorOpen to DoorClosing.\n");
	}
	else if (GetOpenDoorLight())//检测门内开门按钮灯的状态
	{
		SetOpenDoorLight(false);//关闭开门按钮灯
		return;
	}
}
/**********************************************
* DoorClosing状态: 关闭电梯门，若电梯内关门按钮灯亮则关闭关门按钮灯。
**********************************************/
void StateDoorClosing(int *state)
{
	int floor;
	floor = GetNearestFloor();//检测当前电梯所在楼层
	if (GetOpenDoorLight())//检测门内开门按钮灯的状态
	{
		SetDoor(floor, true);//打开电梯门
		SetOpenDoorLight(false);//关闭开门按钮灯
		*state = DoorOpen;
		printf("Transition: from DoorClosing to DoorOpen.\n");
	}
	else if (GetCloseDoorLight())//检测门内关门按钮灯的状态
	{
		SetCloseDoorLight(false);//关闭关门按钮灯
		return;
	}
	else if (IsBeamBroken())//红外探测仪是否探测到遮挡
	{
		SetDoor(floor, true);//打开电梯门
		SetOpenDoorLight(false);//关闭开门按钮灯
		*state = DoorOpen;
		printf("Transition: from DoorClosing to DoorOpen.\n");
	}
	else if (IsDoorClosed(floor))//检测电梯门是否是关闭的
	{
		*state = Idle;
		printf("Transition: from DoorClosing to Idle.\n");
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
