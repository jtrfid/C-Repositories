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
	floor = IdleWhatFloorToGoTo(&up);  //检查是否有门内的上面楼层数的按钮被点亮 或者是否有上面楼层的门外按钮被呼叫
	if (floor > 0 && up)
	{
		SetMotorPower(1);             //上升 打开电机 电梯向上运动 进入向上的状态
		*state = MovingUp;
		return;

	}
	else if (floor>0&&!up)           //如果检查的结果与上面的相反 都是向下楼层的门内以及门外的按钮被点亮
	{
		SetMotorPower(-1);          //下降 打开电机 电梯向下运动 进入向下的状态
		*state = MovingDown;
		return;
	}
	else if (GetOpenDoorLight())   //检查是否有门内的开门按钮被点亮
	{
		SetOpenDoorLight(0);
		SetDoor(GetNearestFloor(), 1);
		*state = DoorOpen;
		return;
	}
	else if (GetCallLight(floor, up))    //检查是否有门外的按钮被点亮 如果有 打开门 消费掉门外的灯
	{
		SetDoor(floor, 1);
		SetCallLight(floor, up, 0);
		*state = DoorOpen;
		return;
	}
	else if (GetCallLight(1, 1))
	{
		SetDoor(1, 1);
		SetCallLight(1, up, 0);
		*state = DoorOpen;
		return;
	}
	else if (GetCloseDoorLight())      //检查门内的关门按钮是否被按 如果被按动 等待空闲状态下无响应 直接将其消费
	{
		SetCloseDoorLight(0);
		return;
	}
}
void StateMovingUp(int *state)
{

	int floor = GoingUpToFloor();

	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)    //判断电梯是否到达指定楼层
	{
		bool up = GetCallLight(GetNearestFloor(), 1);//判断向上的门外按钮是否被点亮
		bool down = GetCallLight(GetNearestFloor(), 0);//判断向下的门外按钮是否被点亮
		SetMotorPower(0);                              //关掉电机使电梯停止
		if (up == 1)         //如果向上的灯亮 将其消费
		{
			SetCallLight(GetNearestFloor(), 1, 0);
		}
		if (down == 1)      //如果向下的灯亮 将其消费
		{
			SetCallLight(GetNearestFloor(), 0, 0);
		}
		SetPanelFloorLight(floor, 0);
		SetDoor(floor,1);

	  *state = DoorOpen;

	}
	if(GetOpenDoorLight() || GetCloseDoorLight())        //按开门或者关门键是无效的在电梯上升或下降的过程中
	{
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);
	}

}
void StateMovingDown(int *state)
{
	int floor = GoingDownToFloor();
    if(fabs(GetFloor() - floor) < Lib_FloorTolerance)//判断电梯是否到达目标楼层
	{
		bool up = GetCallLight(GetNearestFloor(), 1);//门外的向上的灯是否被点亮
		bool down = GetCallLight(GetNearestFloor(), 0);//门外向下的灯是否被点亮
		SetMotorPower(0);
		SetDoor(GetNearestFloor(), 1);
		if (up == 1)                                   //如果向上的灯被点亮 就将灯消费掉
		{
			SetCallLight(GetNearestFloor(), 1, 0);
		}
		if (down == 1)                                //如果向下的灯被点亮 就将灯消费掉
		{
			SetCallLight(GetNearestFloor(), 0, 0);
		}
		SetPanelFloorLight(GetNearestFloor(), 0);
		*state = DoorOpen;
	}

	else if (GetOpenDoorLight() || GetCloseDoorLight())//电梯在运行的过程中按门外的灯是无效的
	{
		SetOpenDoorLight(0);
			SetCloseDoorLight(0);
	}


}
void StateDoorOpen(int *state)
{
	if (GetCloseDoorLight())               //检测门内的关门按钮是否被点亮
	{
		SetDoor(GetNearestFloor(), 0);     //转而关门
		SetCloseDoorLight(0);              //消费门内的关门灯
		*state = DoorClosing;
     }
	else if (IsDoorOpen(GetNearestFloor()))  //开门结束后自动进行关门
	{
		SetDoor(GetNearestFloor(),0);
		*state = DoorClosing;
	}
	else if (GetOpenDoorLight())            //检测是否有开门灯被点亮 若点亮 将其消费
		SetOpenDoorLight(0);

}

void StateDoorClosing(int *state)
{
	if (GetOpenDoorLight())                 //检查是否门内的开门按钮被点亮
	{
		SetDoor(GetNearestFloor(), 0);       //转而开门
		SetOpenDoorLight(0);                 //消费按钮
		*state = DoorOpen;
	}
	else if (GetCloseDoorLight())             //检查门内的关门按钮是否被点亮   如果被点亮直接将其消费
	{
		SetCloseDoorLight(0);
	}
 	else if (IsBeamBroken())                 //检查是否有遮蔽物 如果红外检测到了遮蔽物
	{
		SetDoor(GetNearestFloor(), 1);        //将门打开
		*state = DoorOpen;
	}
	else if (IsDoorClosed(GetNearestFloor()))  //关门结束后 进入待机状态
	{
		//SetOpenDoorLight(0);
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
