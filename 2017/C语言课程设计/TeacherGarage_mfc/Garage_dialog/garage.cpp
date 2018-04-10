#include "stdafx.h"  // mfc必须的

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "GarageLib.h"

/* DoorClosed状态函数 */
void StateDoorClosed(int *state)
{
	if(WasButtonPressed())  // Event
	{
		SetMotorPower(1);  // Transition
		*state = DoorOpening;
		printf("Transition: from DoorClosed to DoorOpening.\n");
	}
}

/* DoorOpening状态函数 */
void StateDoorOpening(int *state)
{
	if (GetDoorPosition() >= Lib_DoorHeight - Lib_DoorTolerance)  // Event
	{
		SetMotorPower(0);  // Transition
		*state = DoorOpen;
		printf("Transition: from DoorOpening to DoorOpen.\n");
	}
	else if(WasButtonPressed()) // Event
	{
		SetMotorPower(-1);  // Transition
		*state = DoorClosing;
		printf("Transition: from DoorOpening to DoorClosing.\n");
	}
}

/* DoorClosing状态函数 */
void StateDoorClosing(int *state)
{
	if (GetDoorPosition() <= Lib_DoorTolerance)  // Event
	{
		SetMotorPower(0);  // Transition
		*state = DoorClosed;
		printf("Transition: from DoorClosing to DoorClosed.\n");
	}
	else if(WasButtonPressed() || IsBeamBroken())  // Event，按键或门的下部红外探测到物体
	{
		SetMotorPower(1);  // Transition
		*state = DoorOpening;
		printf("Transition: from DoorClosing to DoorOpening.\n");
	}
}

/* DoorOpen状态函数 */
void StateDoorOpen(int *state)
{
	if(WasButtonPressed())  // Event
	{
		SetMotorPower(-1); // Transition
		*state = DoorClosing;
		printf("Transition: from DoorOpen to DoorClosing.\n");
	}
}

// 主控循环： 状态机，每隔一定时间(如，100ms)被调用一次，采集系统的运行状态
void main_control(int *state)
{
    if (IsGarageRunning())
    {
		switch(*state)
		{
		case DoorClosed:
			StateDoorClosed(state);
			break;
		case DoorOpening:
			StateDoorOpening(state);
			break;
		case DoorClosing:
			StateDoorClosing(state);
			break;
		case DoorOpen:
			StateDoorOpen(state);
			break;
		}
	}

}

