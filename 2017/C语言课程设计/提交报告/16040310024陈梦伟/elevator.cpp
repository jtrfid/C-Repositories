#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ElevatorLib.h"

/**********************************************
 * Idle״̬������ֹͣ��ĳ¥�㣬���ǹرյģ����ھ�ֹ״̬���ȴ�����¼��ķ������Ӷ�ת������һ��״̬��
 **********************************************/
void StateIdle(int *state)
{ int floor; bool up;
    floor = IdleWhatFloorToGoTo(&up);//Event
	if (floor > 0 && up) 
	{
		SetMotorPower(1);//Transition
		*state = MovingUp;
		printf("Transition:from Idle to MovingUp.\n");

	}
	if (floor>0 && !up)
	{
		SetMotorPower(-1);//Transition
		*state = MovingDown;
		printf("Transition:from Idle to MovingDown.\n");
	}
	if (GetOpenDoorLight())
	{   
		SetDoor(GetNearestFloor(),1 ); //Transition
		SetOpenDoorLight(0);
		*state = DoorOpen;
		printf("Transition:from Idle to DoorOpen.\n");
		
	}
	if (GetCallLight(GetNearestFloor(),1))
	{
		SetDoor(GetNearestFloor(), 1);//Transition
		SetCallLight(GetNearestFloor(), 1, 0);//Transition
		*state = DoorOpen; 
		printf("Transition:from Idle to DoorOpen.\n");
	}
	if (GetCallLight(GetNearestFloor(),0))
	{
		SetDoor(GetNearestFloor(), 1);//Transition
		SetCallLight(GetNearestFloor(), 0, 0);//Transition
		*state = DoorOpen;
		printf("Transition:from Idle to DoorOpen");
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(0); return;
	}
}

void StateMovingUp(int *state)
{
	int floor; 
	floor = GoingUpToFloor();//Event 
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);//Transition
		SetDoor(floor,1);//Transition
		SetPanelFloorLight(floor, 0);//Transition
		SetCallLight(floor, 1, 0);//Transition
		if (floor==Lib_FloorNum)
		{
			SetCallLight(floor, 0, 0);
		}
			*state = DoorOpen;
			printf("Transition:from Idle to DoorOpen");

	}
	if (GetOpenDoorLight() || GetCloseDoorLight())
	{
		SetOpenDoorLight(0);//Transition
		SetCloseDoorLight(0);//Transition
	}
 	
};

void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();//Event
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(floor, 1);
		SetCallLight(floor, 0, 0);//Transition
		if (floor == 1)
			SetCallLight(floor, 1, 0);
		SetPanelFloorLight(floor, 0);
		*state = DoorOpen;
		printf("Transition:from MovingDown to DoorOpen");
	}
	if (GetOpenDoorLight() || GetCloseDoorLight())
	{
		SetOpenDoorLight(0);
		SetCloseDoorLight(0);
	}

}

void StateDoorOpen(int *state)
{
	int floor;
	floor = GetNearestFloor();//Event
	if (GetCloseDoorLight())
	{
		SetDoor(floor, 0);
		SetCloseDoorLight(0);//Transition
		*state = DoorClosing;
		printf("Transition:from DoorOpen to DoorClosing");
		}
	if (IsDoorOpen(floor))
	{
		SetDoor(floor, 0);//Transition
		*state = DoorClosing;
		printf("Transition:from DoorOpen to DoorClosing");
	}
	if (GetOpenDoorLight())
	{
		SetOpenDoorLight(0);//Transition

	}


}
void StateDoorClosing(int *state)
{
	int floor;
	floor = GetNearestFloor();//Event
	if (GetOpenDoorLight())
	{
		SetDoor(floor, 1);
		SetOpenDoorLight(0);//Transition
		*state = DoorOpen;
		printf("Transition:from DoorClosing to DoorOpen");
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(0);//Transition
	}
	if (IsBeamBroken())
	{
		SetDoor(floor, 1);//Transition
		*state = DoorOpen;
		printf("Transition:from DoorClosing to DoorOpen");


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
