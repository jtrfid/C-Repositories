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
	floor = IdleWhatFloorToGoTo(&up); //ȷ��ǰ��¥�� 
	if (floor > 0 && up){			  //�ж�״̬ 
		SetMotorPower(1);				//�򿪵�� 
		*state = MovingUp;				//������һ״̬ 
	}
	if (floor > 0 && !up){				//״̬ͬ�� 
		SetMotorPower(-1);
		*state = MovingDown;
	}
	if (GetOpenDoorLight()){			//�жϿ��ŵ� 
		SetDoor(floor, true);			//���� 
		SetOpenDoorLight(false);		//���ѿ��ŵ� 
		*state = DoorOpen;				//������һ״̬ 
	}
	if (GetCloseDoorLight()){			//��״̬���ޱ仯 
		SetCloseDoorLight(false);
		return;
	}
	int getfloor;
	getfloor = GetNearestFloor();
	if (GetCallLight(GetNearestFloor(), true) || GetCallLight(GetNearestFloor(), false)){	//�жϵ�����������Ƿ�ͬһ�� 
		SetDoor(getfloor, true);															//���� 
		SetCallLight(getfloor, true, false);												//���� 
		SetCallLight(getfloor, false, false);											
		*state = DoorOpen;																	//������һ״̬ 
	}
}

void StateMovingUp(int *state)
{
	int floor;
	floor = GoingUpToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance){				//�ж��Ƿ񵽴� 
		SetMotorPower(0);											//�رյ�� 
		SetDoor(floor, true);										//���� 
		SetCallLight(floor, true, false);							//������� 
		SetCallLight(floor, false, false);
		SetPanelFloorLight(floor, false);							//�����ڲ�¥��� 
		*state = DoorOpen;											//��һ״̬ 
	}
	if (GetOpenDoorLight() || GetCloseDoorLight()){					//�ƶ�״̬�޷������� 
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);
	}
}

void StateMovingDown(int *state)									//״̬��������ͬ 
{
	int floor;
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance){
		SetMotorPower(0);
		SetDoor(floor, true);
		SetCallLight(floor, true, false);
		SetCallLight(floor, false, false);
		SetPanelFloorLight(floor, false);
		*state = DoorOpen;
	}
	if (GetOpenDoorLight() || GetCloseDoorLight()){
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);
	}
}

void StateDoorOpen(int *state)
{
	int floor = GetNearestFloor();
	if (GetCloseDoorLight()){							//�жϹ��ŵ� 
		SetDoor(floor,false);							//���� 
		SetCloseDoorLight(false);						//���� 
		*state = DoorClosing;
	}
	if (IsDoorOpen(floor)){								//�����н��й��� 
		SetDoor(floor, false);
		*state = DoorClosing;
	}
	if (GetOpenDoorLight()){							//���ŵ���״̬ 
		SetOpenDoorLight(false);
	}
}

void StateDoorClosing(int *state)
{
	int floor = GetNearestFloor();
	if (GetOpenDoorLight()){							//״̬�뿪��ʱ�������� 
		SetDoor(floor, true);
		SetOpenDoorLight(false);
		*state = DoorOpen;
	}
	if (GetCloseDoorLight()){
		SetCloseDoorLight(false);
	}
	if (IsBeamBroken()){								//�ж��Ƿ��������ڵ����ſ� 
		SetDoor(floor ,true);							//���� 
		*state = DoorOpen;								//������һ״̬ 
	}
	if (IsDoorClosed(floor)){
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
