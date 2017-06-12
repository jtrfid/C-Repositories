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
	int  floor;
	bool up;
	floor = WhichFloorToGoTo(&up);
	if(floor>=1)
	{
		if(down)
		{
			*state=MovingDown;
			SetCallLight(floor, false, false);
		}
		else
		{
			*state=MovingUp;
			SetCallLight(floor, true, false);
		}
	if (GetOpenDoorlight()= true)
	{
		SetDoor(GetTheFloor(), true);
		SetOpenDoorLight(false);
		*state = DoorOpen;
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
	floor=GoingUpToFloor();	
	if(fabs(GetFloor()-floor)<Lib_FloorTolerance)
	{
		SetDoor(floor,true);
		SetCallLight(floor,true,false);
		SetPaneFloorLight(floor,false);
		*state=DooeOpen;
	}
	else
	{
		SetCallLight(Lib_FloorNum,false);
		SetPanelFLoorLight(Lib_FloorNum, false);
	}
	if (GetOpenDoorLight())
		SetOpenDoorLight(false);
	if (GetCloseLight())
		SetCloseDoorLight(false);
}
void StateMovingDown(int *state)
{
    int floor;
    floor=GoingDownToFloor();
    if(fabs(GetFloor()-floor)<Lib_FloorTolerance) 
    {
        SetDoor(floor,true);
        SetCallLight(floor,false);
        SetPaneFloorLight(floor,false);
    	*state=DooeOpen;	
    }
    if(fabs(GetFloor()-1)<Lib_FloorTolerance) 
    {
    	 SetCallLight(1,true,false);
    	 SetPaneFloorLight(1,false);
	}
	if (GetOpenDoorLight())
		SetOpenDoorLight(false);
	if (GetCloseDoorLight())
		SetCloseDoorLight(false);
}

void StateDoorOpen(int *state)
{
     int floor;
	 floor= GettheFloor();
	 if(GetCloseDoorLight()
	 {
	 	SetDoor(GetNestFloor(),false);
	 	SetCloseDoorLight(false);
	 }
	 if (GetOpenDoorLight())
		 SetOpenDoorLight(false;);
	 if(IsDoorOpen(floor))
	 {
	 	SetDoor(floor,false);
	    *state=DoorClosing;
	 }
}

void StateDoorClosing(int *state)
{
      if(GetOpenDoorLight())
      {
      	SetDoor(GetNestFloor(),true);
      	SetOpenDoorLight(false);
      	*state=DoorOpen;
	  }
	  if(GetCloseDoorLight())
	  {
	  	SetCloseDoorLight(false);
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
