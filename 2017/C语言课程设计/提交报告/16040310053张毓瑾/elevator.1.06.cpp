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
	if (floor > 0 && up)
	{
		SetMotorPower(1);
		*state = MovingUp;
	}/*��⽫Ҫ��������������״̬*/

	if (floor > 0 && !up)
	{
		SetMotorPower(-1);
		*state = MovingDown;
	}/*��⽫Ҫ�½��������½�״̬*/

	if (GetOpenDoorLight())
	{
		SetDoor(GetNearestFloor(),true);
		SetOpenDoorLight(false);
		*state = DoorOpen;
	}/*������ڿ��ذ�ť�����ţ��������ڿ��Ű�ť*/

	if (GetCallLight(GetNearestFloor(), true) && up)
	{
		SetDoor(GetNearestFloor(), true);
		SetCallLight(GetNearestFloor(),true,false);
		*state = DoorOpen;
	}/*�����а�ť�����ţ����ѿ��Ű�ť�����뿪��״̬*/
	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
		return;
	}/*��ȡ���ŵƣ����ѹ��ŵ�*/

}

void StateMovingUp(int *state)
{
	int floor;
	floor = GoingUpToFloor();
		if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
		{
			SetMotorPower(0);
			SetDoor(floor, true);
			*state = DoorOpen;
			SetCallLight(floor, true, false);
			SetPanelFloorLight(floor,false);
		}/*����Ŀ��¥�㣬ֹͣ�����ţ����Ѻ��а�ť�����ڵ�ǰ¥�㰴ť*/
		if (GetNearestFloor()==3)
		{
			SetCallLight(floor, down, false);
		}/*������߲㣬��������down��ť*/



	if (GetOpenDoorLight() || GetCloseDoorLight())
	{
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);
	}/*�˶�״̬��������ʧЧ*/
}

void StateMovingDown(int *state)
{
	int floor;
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance)
	{
		SetMotorPower(0);
		SetDoor(floor, true);
		*state = DoorOpen;
		SetCallLight(floor,false,false);
		SetPanelFloorLight(floor, false);
	}/*����Ŀ��¥�㣬ֹͣ�����ţ�����������а�ť�����ڵ�ǰ¥�㰴ť*/
	if (GetNearestFloor() == 1)
	{
		SetCallLight(floor, up, false);
	}/*������ײ㣬��������up��ť*/
	if (GetOpenDoorLight() || GetCloseDoorLight())
	{
		SetOpenDoorLight(false);
		SetCloseDoorLight(false);
	}/*�˶�״̬��������ʧЧ*/
}

void StateDoorOpen(int *state)
{
	int floor;
	floor = GetNearestFloor();
	if (GetCloseDoorLight())
	{
		SetDoor(floor,false);
		SetCloseDoorLight(false);
		*state = DoorClosing;
	}/*�����ŵƣ����ڿ��ţ����˹��ŵƣ�ת�����ţ����ѹ��Ű�ť���������״̬*/
	if (IsDoorOpen(floor))
	{
		SetDoor(floor,false);
		*state = DoorClosing;
	}/*���Ž������Զ��������״̬*/

	if (GetOpenDoorLight())
	{
		SetOpenDoorLight(false);
	}/*������⿪�Ű�ť�����ڿ��ţ����˿��ŵƣ��޶���,���ѿ��Ű�ť*/


}

void StateDoorClosing(int *state)
{
	int floor;
	floor = GetNearestFloor();
	if (GetOpenDoorLight())
	{
		SetDoor(floor, false);
		SetOpenDoorLight(false);
		*state = DoorOpen;
	}/*������⿪�Ű�ť�����ڹ��ţ����˿��ŵƣ�ת�����ţ����ѹ��Ű�ť*/

	if (GetCloseDoorLight())
	{
		SetCloseDoorLight(false);
	}/*���������Ű�ť�����ѹ��Ű�ť*/

	if (IsBeamBroken())
	{
		SetDoor(floor,true);
		*state = DoorOpen;
	}/*����̽�⵽�ڵ� ��ת������*/
	if (IsDoorClosed(floor))
	{
		*state = Idle;
	}/*���Ž����󣬽���Idle״̬*/






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
			if(GetNearestFloor() !=1 )
			{
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
