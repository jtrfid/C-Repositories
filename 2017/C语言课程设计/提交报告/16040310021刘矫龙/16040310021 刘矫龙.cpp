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
	floor = IdleWhatFloorToGoTo(&up);to
	if (floor > 0 && up)
	{
		SetMotorPower(1);
		*state = MovingUp;
		printf("Translation:from Idle to MovingUp.\n";
	}
	if (floor>0&&!up)
	{
		SetMotorPower(-1);
		*state = MovingUp;
		printf("/translation:from Idle to MovingDown.\n");
	}

	if (GetOpenDoorLight())
	{
		SetDoor(floor, true);
		*state = DoorOpen;
		SetOpenDoorLight(false);
		SetCallLight(GetNearestFloor(), true, false);
		SetCallLight(GetNearestFloor(), false, false);
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
		return;
	}
}

void StateMovingUp(int *state)
{
	int floor;
	floor = GoingUpToFloor();
	if (fabs(GetFloor() - floor)<Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(floor, true);
		*state = DoorOpen;
		printf("Translation:from MovingUp to DoorOpen.\n");
		SetPanelFloorLight(GetNearestFloor(), false);
		if (floor = Lib_Num)
		{
			SetCallLight(false);
		}
	}
	if (GetOpenDoorLight() || GetCloseDoorLight())
	{
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);
	}
}


void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor)<Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(floor, true);
		*state = DoorOpen;
		printf("Translation:from MovingDown to DoorOpen.\n");
		SetPanelFloorLight(GetNearestFloor(), false);
		if (floor ==1)
		{
			SetCallLight(false);
		}
	}
	if (GetOpenDoorLight() || GetCloseDoorLight())
	{
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);
	}
}

void StateDoorOpen(int *state)
{ 
	if (GetCloseDoorLight())
	{
		SetDoor(GetNearestFloor(), false);
		SetCloseDoorLight(false);
		*state = DoorClosing;
		printf("Translation:from DoorOpen to DoorClosing.\n");

	}
	if (IsDoorOpen(GetNearestFloor()))
	{
		SetDoor(GetNearestFloor, false);
		*state = DoorClosing;
		printf("Translation:from DoorOpen to DoorClosing.\n");

	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
	}

}

void StateDoorClosing(int *state)
{
	if (GetOpenDoorLight())
	{
		SetDoor(GetNearestFloor(), true);
		SetOpenDoorLight(false);
		*state = DooOpen;
		printf("Translation:from DoorClosing to DoorOpen.\n");

	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
	}
	if (IsBeamBroken())
	{
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;
		Printf("translation:from DoorClosing to DoorOpen.\n");
	}
	if (IsDoorClosed(GetNearestFloor()))
	{
		SetDoor(GetNearestFloor, false);
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
