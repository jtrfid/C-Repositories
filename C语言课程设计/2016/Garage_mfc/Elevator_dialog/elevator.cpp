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
	int floor;
	bool up;
	int CurrentFooor = GetNearestFloor();

	// 监测电梯外上下按钮灯(Call Light)，开关门
	// 本层的电梯外上下按钮灯(Call Light)被按亮，静态变量保证开门一次,电梯走动后,恢复此值
	// 上下按钮各有一次开门机会
	static bool openUpOnes = false;
	static bool openDownOnes = false;
	
	if(!openUpOnes && GetCallLight(CurrentFooor,true)) {  // 向上
		openUpOnes = true;
		SetDoor(CurrentFooor,true);
		*state = DoorOpen;
		return;
	}
	if(!openDownOnes && GetCallLight(CurrentFooor,false)) { // 向下
		openDownOnes = true;
		SetDoor(CurrentFooor,true);
		*state = DoorOpen;
		return;
	}

	// 监测电梯内开关门按钮
	if(GetOpenDoorLight()) { // 开门
		SetDoor(CurrentFooor,true);
		*state = DoorOpen;
		return;
	}
	else if(GetCloseDoorLight()) {  // 关门
		SetDoor(CurrentFooor,false);
		*state = DoorClosing;
		return;
	}

	// 静态检测，下一步将要到那一层（目标层）
	floor = IdleWhatFloorToGoTo(&up);
	if(floor > 0)
	   printf("空闲状态，将要到的楼层:%d,方向:%s\n",floor,up?"向上":"向下");

	if (floor > 0) {
		openUpOnes = false;   // 开关门灯复位
		openDownOnes = false;
		if (up) {
			// 本层的up call light off
			SetCallLight(CurrentFooor,true,false);
			SetMotorPower(1);
			*state = MovingUp;
		}
		else {
			// 本层的down call light off
			SetCallLight(CurrentFooor,false,false);
			SetMotorPower(-1);
			*state = MovingDown;
		}
	} 	
}

void StateMovingUp(int *state)
{
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
		// 电梯内楼层号Floor Light Off
		SetPanelFloorLight(floor,false);
		SetMotorPower(0);
		
		// 开门
		SetDoor(floor,true);
		*state = DoorOpen;
	}

}

void StateMovingDown(int *state)
{
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
		// 电梯内楼层号Floor Light Off
		SetPanelFloorLight(floor,false);
		SetMotorPower(0);

		// 开门
		SetDoor(floor,true);
		*state = DoorOpen;
	}

}

/**********************************************
 * 开门，开门结束后自动进入关门
 **********************************************/
void StateDoorOpen(int *state)
{
	int floor = GetNearestFloor();

	// 如果是门内开门按钮引起的开门，关掉此按钮的灯。避免Idle状态监测到灯还在亮引起重复开门。
	if(GetOpenDoorLight()) SetOpenDoorLight(false);

	// 如果开门结束，进入关门状态
	if(IsDoorOpen(floor))
	{
		// 关门
		SetDoor(floor,false);
		*state = DoorClosing;
	} 
}

/********************************************
 * 正在关门，省略红外探测
 ********************************************/
void StateDoorClosing(int *state)
{
	// 如果是门内关门按钮引起的关门，关掉此按钮的灯。避免Idle状态监测到灯还在亮引起重复关门门。
	if(GetCloseDoorLight()) SetCloseDoorLight(false);

	// 如果关门结束，到空闲状态，判断下一步的走向
	if(IsDoorClosed(GetNearestFloor()))
	{
		*state = Idle;
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
