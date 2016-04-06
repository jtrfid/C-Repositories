#include <stdafx.h>
#include "Garage.h"

/* Our possible garage door states */
#define DoorClosed 1
#define DoorOpening 2
#define DoorOpen 3
#define DoorClosing 4

int a = 10;  // 定义全局变量，全局变量只能定义一次

bool Running = false;
bool ButtonPressed = false;

void print_test()
{	
	printf("\nprintf_test(),a=%d\n",a);
	a = 20;
}


bool IsGarageRunning()
{
	return Running;
}

void GarageStartup()
{
	// 如果正在运行,保持现在的运行状态
	if(Running) {
		printf("仿真正在运行!!\n");
		return; 
	}
	Running = true;
	ButtonPressed = false;
	printf("开始仿真!!\n");
}

void GarageShutdown()
{
	// 如果不是运行状态,保持现在的非运行状态
	if(!Running) {
		printf("仿真没有开始!!\n");
		return;
	}
	Running = false;
	printf("结束仿真!!\n");
}

bool WasButtonPressed()
{
	return ButtonPressed;
}

void StateDoorClosed(int *state)
{
	if(WasButtonPressed())
	{
		//SetMotorPower(1);
		*state = DoorOpening;
	}
}

/** 每200ms被调用一次，如果仿真已经开始，检测车库门的状态 */
void main_control()
{
	static int state = DoorClosed;  // 初始是关门状态

	if (IsGarageRunning())
	{
		printf("现在的状态，state=%d\n",state);
		switch(state)
		{
		case DoorClosed:
			StateDoorClosed(&state);
			break;
		case DoorOpening:
			//StateDoorOpening(&state);
			break;
		}
	}
}

