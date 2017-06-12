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
	floor = IdleWhatFloorToGoTo(&up);//获取要去的楼层
	if (floor > 0 && up){            //合法且向上走
		SetMotorPower(1);            //向上走
		*state = MovingUp;           //更新state
		printf("Transition: from Idle to MovingUp.\n");
	    }
	if (floor > 0 && !up){           //合法且向下走
		SetMotorPower(-1);
		*state = MovingDown;
		printf("Transition: from Idle to MovingDown.\n");
	}
	if (GetOpenDoorLight()){         //检查开门按钮 若被按下
		SetOpenDoorLight(false);     //关闭开门灯 
		floor = GetNearestFloor();   //获取当前楼层
		SetDoor(floor,true);         //开当前楼层门
		*state = DoorOpen;
		printf("Transition: from Idle to DoorOpen.\n");
	    }
	if (GetCallLight(GetNearestFloor(), true) || GetCallLight(GetNearestFloor(),false)){//当前楼层呼叫灯被按下
		floor = GetNearestFloor();
		SetDoor(floor, true);         //开门
		SetCallLight(floor, true, false);//关闭呼叫灯
		SetCallLight(floor, false, false);
		*state = DoorOpen;
		printf("Transition: from Idle to DoorOpen.\n");
	    }
	if (GetCloseDoorLight()) { //关门按钮被按下 关闭关门按钮灯 该动作无效 
		SetCloseDoorLight(false); 
	    }

}

void StateMovingUp(int *state)
{  
	int floor;
	SetOpenDoorLight(false);                    //关闭开关门灯 开关门动作失效
	SetCloseDoorLight(false);
	floor = GoingUpToFloor();                   //获取要去的楼层
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance){//当前楼层和目标楼层间距小于误差
		SetMotorPower(0);                       //停
		SetDoor(floor, true);                   //开门
		*state = DoorOpen;
		printf("Transition: from MovingUp to DoorOpen.\n");
		if (floor == Lib_FloorNum)             //3楼的话 关闭下呼叫灯
	    	SetCallLight(floor, false, false);
		else{                                  //否则上下都关
			SetCallLight(floor, true, false);
			SetCallLight(floor, false, false);
		}
		SetPanelFloorLight(GetNearestFloor(), false);//关闭楼层按钮灯
	}
}

void StateMovingDown(int *state)
{
	int floor;
	SetOpenDoorLight(false);
	SetCloseDoorLight(false);
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance){
		SetMotorPower(0);
		SetDoor(floor, true);
		*state = DoorOpen;
		printf("Transition: from MovingDown to DoorOpen.\n");
		if (floor == 1)
			SetCallLight(floor, true, false);
		else{
			SetCallLight(floor, false, false);
			SetCallLight(floor, true, false);
		}
		SetPanelFloorLight(GetNearestFloor(), false);
	}
}

void StateDoorOpen(int *state)
{
	if (GetCloseDoorLight()){      //检查关门灯 关门
		SetDoor(GetNearestFloor(), false);
		*state = DoorClosing;
		SetCloseDoorLight(false);  //关闭关门灯 
		printf("Transition: from DoorOpen to DoorClosing.\n");
	}
	if (IsDoorOpen(GetNearestFloor())){//门已经开启
		SetDoor(GetNearestFloor(), false);//关门
		*state = DoorClosing;
		printf("Transition: from DoorOpen to DoorClosing.\n");
	}
	if (GetOpenDoorLight()) //开门按钮失效
		SetOpenDoorLight(false);
}

void StateDoorClosing(int *state)
{
	if (GetOpenDoorLight()){      //开门按钮被按下
		SetDoor(GetNearestFloor(), true);//开门
		SetOpenDoorLight(false);   //关闭开门灯
		*state = DoorOpen;
		printf("Transition: from DoorClosing to DoorOpen.\n");
	}
	if (GetCloseDoorLight())    //关门按钮失效
		SetCloseDoorLight(false);
	if (IsBeamBroken()){        //有障碍物 转为开门
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;
		printf("Transition: from DoorClosing to DoorOpen.\n");
	}
	if (IsDoorClosed(GetNearestFloor()))//门已经关闭
		*state = Idle;                  //更新状态
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
