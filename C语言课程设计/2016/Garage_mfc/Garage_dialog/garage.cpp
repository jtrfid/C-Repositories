#include "stdafx.h"  // mfc必须的

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "GarageLib.h"

void StateDoorClosed(int *state)
{
	if(WasButtonPressed())  // Event
	{
		SetMotorPower(1);  // Transition
		*state = DoorOpening;
	}
}

void StateDoorOpening(int *state)
{
	if (GetDoorPosition() >= Lib_DoorHeight - Lib_DoorTolerance)  // Event
	{
		SetMotorPower(0);  // Transition
		*state = DoorOpen;
	}
	else if(WasButtonPressed()) // Event
	{
		SetMotorPower(-1);  // Transition
		*state = DoorClosing;
	}
}

void StateDoorClosing(int *state)
{
	if(IsBeamBroken()) // Event
	{
		SetMotorPower(1); // Transition
		*state = DoorOpening;
		return;
	}

	if (GetDoorPosition() <= Lib_DoorTolerance)  // Event
	{
		SetMotorPower(0);  // Transition
		*state = DoorClosed;
	}
	else if(WasButtonPressed() || IsBeamBroken())  // Event
	{
		SetMotorPower(1);  // Transition
		*state = DoorOpening;
	}
}

void StateDoorOpen(int *state)
{
	if(WasButtonPressed())  // Event
	{
		SetMotorPower(-1); // Transition
		*state = DoorClosing;
	}
}

// 状态机，每隔一定时间(如，100ms)被调用一次，采集系统的运行状态
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

