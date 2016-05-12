#include "stdafx.h"  // mfc�����

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "GarageLib.h"

/* DoorClosed״̬���� */
void StateDoorClosed(int *state)
{
	if(WasButtonPressed())  // Event
	{
		SetMotorPower(1);  // Transition
		*state = DoorOpening;
		printf("Transition: from DoorClosed to DoorOpen.\n");
	}
}

/* DoorOpening״̬���� */
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

/* DoorClosing״̬���� */
void StateDoorClosing(int *state)
{
	if (GetDoorPosition() <= Lib_DoorTolerance)  // Event
	{
		SetMotorPower(0);  // Transition
		*state = DoorClosed;
		printf("Transition: from DoorClosing to DoorClosed.\n");
	}
	else if(WasButtonPressed() || IsBeamBroken())  // Event���������ŵ��²�����̽�⵽����
	{
		SetMotorPower(1);  // Transition
		*state = DoorOpening;
		printf("Transition: from DoorClosing to DoorOpening.\n");
	}
}

/* DoorOpen״̬���� */
void StateDoorOpen(int *state)
{
	if(WasButtonPressed())  // Event
	{
		SetMotorPower(-1); // Transition
		*state = DoorClosing;
		printf("Transition: from DoorOpen to DoorClosing.\n");
	}
}

// ����ѭ���� ״̬����ÿ��һ��ʱ��(�磬100ms)������һ�Σ��ɼ�ϵͳ������״̬
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

