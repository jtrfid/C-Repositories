#include "stdafx.h"  // mfc±ØÐëµÄ

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "GarageLib.h"

void StateDoorClosed(int *state)
{
	if(WasButtonPressed())
	{
		SetMotorPower(1);
		*state = DoorOpening;
	}
}

void StateDoorOpening(int *state)
{
	if (GetDoorPosition() >= Lib_DoorHeight - Lib_DoorTolerance)
	{
		SetMotorPower(0);
		*state = DoorOpen;
	}
	else if(WasButtonPressed())
	{
		SetMotorPower(-1);
		*state = DoorClosing;
	}
}

void StateDoorClosing(int *state)
{
	if(IsBeamBroken())
	{
		SetMotorPower(1);
		*state = DoorOpening;
		return;
	}

	if (GetDoorPosition() <= Lib_DoorTolerance)
	{
		SetMotorPower(0);
		*state = DoorClosed;
	}
	else if(WasButtonPressed() || IsBeamBroken())
	{
		SetMotorPower(1);
		*state = DoorOpening;
	}
}

void StateDoorOpen(int *state)
{
	if(WasButtonPressed())
	{
		SetMotorPower(-1);
		*state = DoorClosing;
	}
}

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

