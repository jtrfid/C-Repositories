#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ElevatorLib.h"

/**********************************************
 * Idle״̬������ֹͣ��ĳ¥�㣬���ǹرյģ����ھ�ֹ״̬���ȴ�����¼��ķ������Ӷ�ת������һ��״̬��
 **********************************************/
void StateIdle(int *state)
{
	int floor; bool up;
	floor = IdleWhatFloorToGoTo(&up);
	if (floor>0&&up)
	{
		SetMotorPower(1);
		*state = MovingUp;
		printf("transition:from Idle to Movingup.\n");
	}
	else if (floor>0&&!up)
	{
		SetMotorPower(-1);
		*state = MovingDown;
		printf("transition:from Idle to Movingdown.\n");
	}
	else if (GetOpenDoorLight())
	{
		SetDoor(floor,true);
		SetOpenDoorLight(false);
		*state = DoorOpen;
		printf("transition:from Idle to DoorOpen.\n");
	}
	else if (GetCallLight(floor, true))
	{
		SetDoor(floor, true);
		SetCallLight(floor, true, false);
		*state = DoorOpen;
		printf("transition:from Idle to DoorOpen.\n");
	}
	else if (GetCallLight(floor, false))
	{
		SetDoor(floor, true);
		SetCallLight(floor, false, false);
		*state = DoorOpen;
		printf("transition:from Idle to DoorOpen.\n");
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
		return;
	}
}

void StateMovingUp(int *state)
{
	int floor; bool up;
	{
		floor = GoingUpToFloor();
		if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
		{
			SetMotorPower(0);
			SetDoor(floor, true);
			*state = DoorOpen;
			if (GetCallLight(floor, true))
			{
                SetCallLight(floor, true, false);
			}
			else if (GetCallLight(floor, false))
			{
                SetCallLight(floor, false, false);
			}
			if (floor == Lib_FloorNum)
			{
				SetCallLight(floor, false, false);
			}
			printf("transition:from movingup to DoorOpen.\n");
		}
		if (GetOpenDoorLight() ||GetCloseDoorLight())
		{
			SetOpenDoorLight(false);
			SetCloseDoorLight(false);
		}
	}
}

void StateMovingDown(int *state)
{
	int floor; bool up;
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(floor, true);
		*state = DoorOpen;
		if (GetCallLight(floor, true))
		{
            SetCallLight(floor, true, false);
		}
		else if (GetCallLight(floor, false))
		{
            SetCallLight(floor, false, false);
		}
		if (floor == 1)
		{
			SetCallLight(1, true, false);
			SetPanelFloorLight(1, false);
		}
	}
	if (GetOpenDoorLight() ||GetCloseDoorLight())
	{
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);
	}
}

void StateDoorOpen(int *state)
{
	int floor; bool up;
	floor = GetNearestFloor();
	if (GetCloseDoorLight())
	{
		SetDoor(floor, false);
		SetCloseDoorLight(false);
		*state = DoorClosing;
		return;
		printf("transition:from dooropen to doorclosing.\n");
	}
	else if (IsDoorOpen(floor))
	{
		SetDoor(floor, false);
		*state = DoorClosing;
		return;
		printf("transition:from dooropen to Doorclosing.\n");
	}
	else if (GetOpenDoorLight())
	{
		SetOpenDoorLight(false);
		*state = DoorOpen;
	}
}

void StateDoorClosing(int *state)
{
	int floor; bool up;
	floor = GetNearestFloor();
	if (GetOpenDoorLight())
	{
		SetDoor(floor, true);
		SetOpenDoorLight(false);
		*state = DoorOpen;
		printf("transition:from doorclosing to DoorOpen.\n");
	}
	else if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
		*state = DoorOpen;
		printf("transition:from doorclosing to DoorOpen.\n");
	}
	else if (IsBeamBroken())
	{
		SetDoor(floor, true);
		*state = DoorOpen;
		printf("transition:from doorclosing to DoorOpen.\n");
	}
	else if (IsDoorClosed(floor))
	{
		*state = Idle;
	}
}

/***********************************************
 * ״̬����ÿ��һ��ʱ��(�磬100ms)������һ�Σ��ɼ�ϵͳ������״̬
 ***********************************************/
void main_control(int *state)
{  
    if(IsElevatorRunning())  // ������������
    {
		switch(*state)
		{
		case Idle:
			// Idle״̬��һ��ʱ���޶������Զ���һ¥
			if(GetNearestFloor() !=1 ) {
				AutoTo1Floor();
			}
			StateIdle(state);
			break;
		case MovingUp:
			CancelTo1Floor(); // ����״̬��ȡ���Զ���һ¥
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
			printf("û������״̬!!!\n");  
		}
    }
}
