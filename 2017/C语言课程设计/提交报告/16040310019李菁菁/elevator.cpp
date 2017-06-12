#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ElevatorLib.h"

/**********************************************
* Idle״̬������ֹͣ��ĳ¥�㣬���ǹرյģ����ھ�ֹ״̬���ȴ�����¼��ķ������Ӷ�ת������һ��״̬��
**********************************************/
/*void StateIdle(int *state)
{
	bool up;
	int floor;
	floor = IdleWhatFloorToGoTo(&up);
	if (floor>0 && up)
	{
		SetMotorPower(1);
		*state = MovingUp;
	}
	if (floor>0 && !up)
	{
		SetMotorPower(-1);
		*state = MovingDown;
	}
	if (GetOpenDoorLight())
	{
		SetDoor(GetNearestFloor(), 1);
		SetOpenDoorLight(0);
		*state = DoorOpen;
	}
	if (GetCallLight(GetNearestFloor(), 1))
	{
		SetDoor(GetNearestFloor(), 1);
		SetCallLight(GetNearestFloor(), 1, 0);
		*state = DoorOpen;
	}
	if (GetCallLight(GetNearestFloor(), 0))
	{
		SetDoor(GetNearestFloor(), 1);
		SetCallLight(GetNearestFloor(), 0, 0);
		*state = DoorOpen;
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(0);
		return;
	}
}

void StateMovingUp(int *state)
{
	int floor;
	floor = GoingUpToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(floor, 1);
		SetPanelFloorLight(floor, 0);
		SetCallLight(floor, 1, 0);
		if (floor == Lib_FloorNum)
		{
			SetCallLight(floor, 0, 0);
		}
		*state = DoorOpen;
	}
	if (GetOpenDoorLight() || GetCloseDoorLight())
	{
		SetOpenDoorLight(0);
		SetCloseDoorLight(0);
	}
}

void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(floor, 1);
		SetCallLight(floor, 0, 0);
		if (floor == 1)
			SetCallLight(floor, 1, 0);
		SetPanelFloorLight(floor, 0);
		*state = DoorOpen;
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
	floor = GetNearestFloor();
	if (GetCloseDoorLight())
	{
		SetDoor(floor, 0);
		SetCloseDoorLight(0);
		*state = DoorClosing;
	}
	if (IsDoorOpen(floor))
	{
		SetDoor(floor, 0);
		*state = DoorClosing;
	}
	if (GetOpenDoorLight())
	{
		SetOpenDoorLight(0);
	}
}

void StateDoorClosing(int *state)
{
	int floor;
	floor = GetNearestFloor();
	if (GetOpenDoorLight())
	{
		SetDoor(floor, 1);
		SetOpenDoorLight(0);
		*state = DoorOpen;
	}
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(0);
	}
	if (IsBeamBroken())
	{
		SetDoor(floor, 1);
		*state = DoorOpen;
	}
	if (IsDoorClosed(floor))
	{
		*state = Idle;
	}
}*/
/*********************************************
 * Idle״̬������ֹͣ��ĳ¥�㣬���ǹرյģ����ھ�ֹ״̬���ȴ�����¼��ķ������Ӷ�ת������һ��״̬��
 **********************************************/
void StateIdle(int *state)
 {
	int floor,nowfloor;
	bool up;
	nowfloor = GetNearestFloor(); 
	floor = IdleWhatFloorToGoTo(&up); 
	printf("%d",up);
	if (floor > 0 && up) {
		SetMotorPower(1);
		*state = MovingUp;
	}//����
	if (floor > 0 && !up) {
		SetMotorPower(-1);
		*state = MovingDown;
	}//  ���E3 E4 E5�¼� �������˶�����

			if (GetOpenDoorLight()){ 
				SetDoor(nowfloor, 1);
			SetOpenDoorLight(0);
			*state = DoorOpen;//������ڿ��ذ�ť �Ƿ����� ��������
			}
			if (GetCallLight(nowfloor, 1) && (up || !GetCallLight(3, 0) && (!up || GetCallLight(1, 1)))){
				SetDoor(nowfloor,1);
				SetCallLight(nowfloor,1,0);
				SetCallLight(nowfloor, 0, 0);
				*state = DoorOpen;
		}//���E4 E5 �ж�Ŀ�� ¥���Ƿ��ǵ�ǰ¥�� �Ǽ����Ž���dooropen
			GetCloseDoorLight();
			if (GetCloseDoorLight()) { SetCloseDoorLight(false); return; }

}
	
void StateMovingUp(int *state)//��������
	{
		int  floor;
		bool up;
		floor = GoingUpToFloor();
		printf("Ŀ��%d", floor);
		printf("��ǰ%f \n", GetFloor());

		if ((fabs(GetFloor() - floor) < Lib_FloorTolerance) &&!( (GetCallLight(3, 0)) && !GetCallLight(2, 1) )){//�ж��Ƿ񵽴�
		 SetMotorPower(0);
			SetDoor(floor, 1); //����
			if ((GetCallLight(floor, 0) )&& (GetCallLight(floor,1 )))
				SetCallLight(floor, 1, false);
		
		else{ SetCallLight(floor, 1, false);
		SetCallLight(floor, 0, false); }
			SetPanelFloorLight(floor, 0);
			SetPanelFloorLight(floor, 0);//��up��
			*state = DoorOpen;
		}
		if ((fabs(GetFloor() - 3)) < Lib_FloorTolerance){
			SetMotorPower(0);
			SetDoor(floor, 1);
				SetCallLight(floor, 0, false);
		}
		if (GetOpenDoorLight() ||GetCloseDoorLight()){
			SetOpenDoorLight(0);
			SetCloseDoorLight(0);
		}//������ʧЧ
	}
		

void StateMovingDown(int *state)
{
	int  floor;
	bool up;
	floor = GoingUpToFloor();
	printf("Ŀ��%d", floor);
	printf("��ǰ%f \n", GetFloor());
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance && !(GetCallLight(1, 1) && !GetCallLight(2, 0)) || fabs(GetFloor() - 1) < Lib_FloorTolerance){//�ж��Ƿ񵽴�
		SetMotorPower(0);
		SetDoor(floor, 1); //����
		if (fabs(GetFloor() - 1) < Lib_FloorTolerance){//�ж��Ƿ�����Ͳ�
			SetCallLight(1, 1, false); 
			SetPanelFloorLight(1, 0);//��up��
		}
		else SetCallLight(floor, 0, false);
		SetPanelFloorLight(floor, 0);//��down��



		*state = DoorOpen;
	}
	if (GetOpenDoorLight() ||
		GetCloseDoorLight()){
		SetOpenDoorLight(0);
		SetCloseDoorLight(0);
	}//������ʧЧ
}
void StateDoorOpen(int *state)
{
	
	int floor;
	floor = GetNearestFloor();
	if (GetCloseDoorLight() == 1){
		SetDoor(floor,0);
		SetCloseDoorLight(0);
		*state = DoorClosing;
	}
	if (IsDoorOpen(floor)){
		SetDoor(floor, 0);
		*state = DoorClosing;
	}
	if(GetOpenDoorLight())
		SetOpenDoorLight(0);

}

void StateDoorClosing(int *state)
{	int floor;
	floor = GetNearestFloor();
	if (GetOpenDoorLight()){
		SetDoor(floor,1);
		SetOpenDoorLight(0); 
		
	}
	if (GetCloseDoorLight()){
		SetCloseDoorLight(0);
		*state = DoorOpen;
	}
	if (IsBeamBroken()){ SetDoor(floor, 1);
	*state = DoorOpen;
	}
	if (IsDoorClosed(floor))
		*state = Idle;
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
