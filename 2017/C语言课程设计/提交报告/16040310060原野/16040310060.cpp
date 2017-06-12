#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ElevatorLib.h"
/**********************************************
* Idle状态，电梯停止在某楼层，门是关闭的，处于静止状态，等待相关事件的发生，从而转换到下一个状态。
**********************************************/
void StateIdle(int *state)//平常状态层数》0并增加时状态为上升 ，当不动时，若按下门内开门，开门后按钮熄灭，状态为开门。
{                                                                     // 若按下门内关门，关门门后按钮熄灭，状态为关门
	int floor; bool up;
	floor = IdleWhatFloorToGoTo(&up);//检查要到的楼层 ，目标楼层 
	if (floor > 0 && up){  //楼层》0并up 
		SetMotorPower(1);//上升 
		*state = MovingUp;//状态为movingup 
	}
	else if (floor > 0 && !up){  //目标楼层 
		SetMotorPower(-1);  //下降 
		*state = Movingopen;//进入moving open状态 
	}
	if (GetOpenDoorLight()){  //检查门内开关按钮 
		SetDoor(floor, true);   //开门 
		SetOpenDoorLight(false);//消费开关按钮
		*state = DoorOpen;  //进入 door open状态 
	}
	if (GetCloseDoorLight()){  //检查门内开关按钮 
		SetCloseDoorLight(false);//消费关门按钮 
		return;
	}
}

void StateMovingUp(int *state)//若当前楼层与目标楼层绝对值极小，停止上升，开门，门外呼叫按钮熄灭 。否则 
{                              //开门按钮，关门按钮都熄灭。 
	int floor; bool up, s;
	floor = GoingUpToFloor();  //目标楼层 
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance){//到达目标楼层 
		SetMotorPower(0);        //停止 
		SetDoor(floor, true);  //开门 
		*state = DoorOpen;    //进入door open状态 
		if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
			SetCallLight(floor, false, false);//消费门外up按钮 
		SetCallLight(floor, true, false);  
		SetPanelFloorLight(floor, false);//消费门内楼层按钮为当前楼层的按钮 
	}
	else {
		GetOpenDoorLight(); SetOpenDoorLight(false);//检查门内开关按钮；消费开门按钮 
		GetCloseDoorLight(); SetCloseDoorLight(false);// 检查门内开关按钮；消费关门按钮 
	}
}

void StateMovingDown(int *state)//若当前楼层与目标楼层绝对值极小，停止下降，开门，门外呼叫按钮熄灭 。否则 
{                                //开门按钮，关门按钮都熄灭。
	int floor; bool up;
	floor = GoingDownToFloor();  //目标楼层 
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance){//到达目标楼层 
		SetMotorPower(0);   //停止 
		SetDoor(floor, true);//开门 
		*state = DoorOpen;  //进入door open状态 
		if (fabs(GetFloor() - 1) < Lib_FloorTolerance)//到达目标楼层 
		SetCallLight(floor, true, false);//消费门外down按钮 
		SetupLight(floor, false, false);//到达1层 ，消费门外up按钮 
		SetPanelFloorLight(floor, false);//消费门内楼层按钮为当前楼层按钮 
		GetNearestFloor();//获取当前楼层 
	}
	else {
		GetOpenDoorLight(); SetOpenDoorLight(false);//检查门内开门按钮，消费门内开门按钮 
		GetCloseDoorLight(); SetCloseDoorLight(false);// 检查门内关门按钮，消费门内关门按钮 
	}
}

void StateDoorOpen(int *state)
{                                
	int floor;
	floor = GetNearestFloor();//获取当前楼层 
	if (GetCloseDoorLight()){  //正在开门，安了关门灯，转而关门。 
		SetCloseDoorLight(false);//消费关门按钮 
		*state = DoorClosing;   //进入door closing状态 
	}
	else if (IsDoorOpen(floor)){    //开门结束 
		SetDoor(floor, false);  //自动进行关门 
		*state = DoorClosing;  //进入door closing状态 
	}
	else if (GetOpenDoorLight())//检查门内开关按钮，正在开门， 按了关门灯 
		SetOpenDoorLight(false);//消费开门按钮 
}

void StateDoorClosing(int *state)
{
	int floor;
	floor = GetNearestFloor();//获取当前楼层 
	if (GetCloseDoorLight()){// 检查门内开关按钮  
		SetDoor(floor, true);//按了开门按钮，转而开门 
		SetOpenDoorLight(false);//消费开门按钮 
		*state = DoorOpen;//进入door open状态 
	}
	else if (GetCloseDoorLight()) SetCloseDoorLight(false);//检查关门按钮，按了关门按钮，无动作，消费关门按钮 
	else if (IsBeamBroken()){    //如果红外线探测到遮挡， 
		SetDoor(floor, true);  //转而开门 
		*state = DoorOpen;    //进入door open状态 
	}
	else if (IsDoorClosed(floor)) *state = Idle;//关门结束，进入idle状态 
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
