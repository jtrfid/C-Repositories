#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ElevatorLib.h"

/**********************************************
 * 空闲状态，判断是否有按键，判断将要进入何种状态
 **********************************************/
void StateIdle(int *state)
{
	int floor;  // 目标楼层
	bool up;    // 上升/下降
	int CurrentFloor = GetNearestFloor();  // 当前楼层

	// 静态检测，下一步将要到那一层（目标层）
	floor = IdleWhatFloorToGoTo(&up);
	if (floor > 0)
		printf("空闲状态，将要到的楼层【目标楼层】:%d,方向:%s\n", floor, up ? "向上" : "向下");

	// 监测电梯外上下按钮灯(Call Light)，开门请求
	if (up && GetCallLight(CurrentFloor, true)) {  // 向上
		// 电梯外Up，Call Light Off
		SetCallLight(CurrentFloor, true, false);
		// 开门
		SetDoor(CurrentFloor, true);
		*state = DoorOpen;
		printf("Transition:  from Idle to DoorOpen.\n");
		return;
	}

	if (!up && GetCallLight(CurrentFloor, false)) { // 向下
		// 电梯外Down，Call Light Off
		SetCallLight(CurrentFloor, false, false);
		// 开门
		SetDoor(CurrentFloor, true);
		*state = DoorOpen;
		printf("Transition:  from Idle to DoorOpen.\n");
		return;
	}

	// 监测电梯内开关门按钮
	if(GetOpenDoorLight()) { // 开门
		SetOpenDoorLight(false); // turn off, 关灯，为了读取一次生效，而后不重复 
		SetDoor(CurrentFloor,true);
		*state = DoorOpen;
		printf("Transition:  from Idle to DoorOpen.\n");
		return;
	}
	// 断言在Idle状态，门一定是关闭的, 因此应该不执行从Idle到DoorClosing的转换
	// 仅读取关门灯，并关闭关门灯，即消费按键行为。
	else if(GetCloseDoorLight()) {  // 关门
		SetCloseDoorLight(false); // turn off, 关灯，为了读取一次生效，而后不重复
		ASSERT(IsDoorClosed(CurrentFloor));  // 断言门一定是关闭的
		return;
		/********
		SetDoor(CurrentFloor,false);
		*state = DoorClosing;
		printf("Transition:  from Idle to DoorClosing.\n");
		return;
		*********/
	}

	if (floor > 0) {
		if (up) {
			// 本层的up call light off
			SetCallLight(CurrentFloor,true,false);
			SetMotorPower(1);
			*state = MovingUp;
			printf("Transition:  from Idle to MovingUp.\n");
		}
		else {
			// 本层的down call light off
			SetCallLight(CurrentFloor,false,false);
			SetMotorPower(-1);
			*state = MovingDown;
			printf("Transition:  from Idle to MovingDown.\n");
		}
	} 	
}

void StateMovingUp(int *state)
{
	// 安全设置，运动状态，开关门失效，消费相应按钮行为
	if (GetOpenDoorLight()) { // 消费开门按钮
		SetOpenDoorLight(false); 
		printf("运动状态，开关门失效!!!\n");
		return;
	}
	if (GetCloseDoorLight()) { // 消费关门按钮
		SetCloseDoorLight(false); 
		printf("运动状态，开关门失效!!!\n");
		return;
	}

	int floor = GoingUpToFloor(); // 获得目标楼层
	
	//double distance = GetFloor();
	//printf("StateMovingUp %d,%f,%f,%f\n",floor,GetPosition(),distance,distance-floor);

	if(fabs(GetFloor() - floor) < Lib_FloorTolerance) {
		CString status;
		status.Format(_T("Up\n[%d]楼"),floor);
		ViewStatus(status);

		printf("Up 到[%d]楼啦！\n",floor);

		// 电梯外UP， Call Light Off
		SetCallLight(floor,true,false);

		// 如果到了最高层，电梯外Down，Call Light Off
		if (floor == Lib_FloorNum) SetCallLight(floor, false, false);

		// 电梯内楼层号Floor Light Off
		SetPanelFloorLight(floor,false);

		// 停止
		SetMotorPower(0);
		
		// 开门
		SetDoor(floor,true);
		*state = DoorOpen;
		printf("Transition:  from MovingUp to DoorOpen.\n");
	}

}

void StateMovingDown(int *state)
{
	// 安全设置，运动状态，开关门失效，消费相应按钮行为
	if (GetOpenDoorLight()) { // 消费开门按钮
		SetOpenDoorLight(false);
		printf("运动状态，开关门失效!!!\n");
		return;
	}
	if (GetCloseDoorLight()) { // 消费关门按钮
		SetCloseDoorLight(false);
		printf("运动状态，开关门失效!!!\n");
		return;
	}

	int floor = GoingDownToFloor(); // 获得目标楼层

	//double distance = GetFloor();
	//printf("StateMovingDown %d,%f,%f,%f\n",floor,GetPosition(),distance,distance-floor);

	if(fabs(GetFloor() - floor) < Lib_FloorTolerance) {
		CString status;
		status.Format(_T("Down\n[%d]楼"),floor);
		ViewStatus(status);

		printf("Down 到[%d]楼啦！\n",floor);
		// 电梯外Down，Call Light Off
		SetCallLight(floor,false,false);  

		// 如果到了1楼，电梯外Up，Call Light Off
		if (floor == 1) SetCallLight(floor, true, false);

		// 电梯内楼层号Floor Light Off
		SetPanelFloorLight(floor,false);

		// 停止
		SetMotorPower(0);

		// 开门
		SetDoor(floor,true);
		*state = DoorOpen;
		printf("Transition:  from MovingDown to DoorOpen.\n");
	}

}

/**********************************************
 * 开门，开门结束后自动进入关门
 **********************************************/
void StateDoorOpen(int *state)
{
	int floor = GetNearestFloor();

	// 如果正在开门时，按了关门灯，转而关门
	if (GetCloseDoorLight()) {
		SetCloseDoorLight(false); // turn off, 关灯，为了读取一次生效，而后不重复 
		SetDoor(floor, false);
		*state = DoorClosing;
		printf("Transition:  from DoorOpen to DoorClosing.\n");
		return;
	}

	// 如果正在开门，按了开门灯，关灯返回
	if (GetOpenDoorLight()) {
		SetOpenDoorLight(false); // turn off, 关灯，为了读取一次生效，而后不重复
		printf("已经在开门！\n");
		return;
	}

	// 如果开门结束，进入关门状态
	if(IsDoorOpen(floor))
	{
		// 关门
		SetDoor(floor,false);
		*state = DoorClosing;
		printf("Transition:  from DoorOpen to DoorCloing.\n");
	} 
}

/********************************************
 * 正在关门，省略红外探测
 ********************************************/
void StateDoorClosing(int *state)
{
	int floor = GetNearestFloor();

	// 如果正在关门时，按了开门灯，转而开门
	if (GetOpenDoorLight()) { 
		SetOpenDoorLight(false); // turn off, 关灯，为了读取一次生效，而后不重复 
		SetDoor(floor, true);
		*state = DoorOpen;
		printf("Transition:  from DoorClosing to DoorOpen.\n");
		return;
	}

	// 如果正在关门，按了关门灯，关灯返回
	if (GetCloseDoorLight()) {
		SetCloseDoorLight(false); // turn off, 关灯，为了读取一次生效，而后不重复
		printf("已经在关门！\n");
		return;
	}

	// 如果关门结束，到空闲状态，判断下一步的走向
	if(IsDoorClosed(floor))
	{
		*state = Idle;
		printf("Transition:  from DoorClosing to Idle.\n");
	} 
}

/***********************************************
 * 状态机，每隔一定时间(如，100ms)被调用一次，采集系统的运行状态
 ***********************************************/
void main_control(int *state)
{  
    if(IsElevatorRunning())
    {
		switch(*state)
		{
		case Idle:
			// 一定时间无动作，自动到一楼
			if(GetNearestFloor() !=1 ) {
				AutoTo1Floor();
			}
			StateIdle(state);
			break;
		case MovingUp:
			CancelTo1Floor();
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
