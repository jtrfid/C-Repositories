#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ElevatorLib.h"

/*
 * Name :        <insert name here>
 * Description : Project 2 - The elevator controller
 */

/*
 * Name :        main()
 * Description : Program entry point.
 */


/**********************************************
 * ����״̬���ж��Ƿ��а�������Ҫ�������״̬
 **********************************************/
void StateIdle(int *state)
{
	int floor;
	bool up;
	int CurrentFooor = GetNearestFloor();

	// Call Light������
	// ����ĵ��������°�ť��(Call Light)����������̬������֤����һ��,�����߶���,�ָ���ֵ
	// ���°�ť����һ�ο��Ż���
	static bool openUpOnes = false;
	static bool openDownOnes = false;
	
	if(!openUpOnes && GetCallLight(CurrentFooor,true)) {  // ����
		openUpOnes = true;
		*state = DoorOpen;
	}
	if(!openDownOnes && GetCallLight(CurrentFooor,false)) { // ����
		openDownOnes = true;
		*state = DoorOpen;
	}

	// ��Ҫ����һ��
	floor = IdleWhatFloorToGoTo(&up);
	if(floor > 0)
	   printf("����״̬����Ҫ����¥��:%d,����:%s\n",floor,up?"����":"����");

	if (floor > 0) {
		openUpOnes = false;   // �����Ÿ�λ
		openDownOnes = false;
		if (up) {
			// �����up call light off
			SetCallLight(CurrentFooor,true,false);
			SetMotorPower(1);
			*state = MovingUp;
		}
		else {
			// �����down call light off
			SetCallLight(CurrentFooor,false,false);
			SetMotorPower(-1);
			*state = MovingDown;
		}
	} 	
}

void StateMovingUp(int *state)
{
	int floor = GoingUpToFloor(); // ����Ƿ���һ¥����Ҫ����¥��

	double distance = GetFloor();
	//printf("StateMovingUp %d,%f,%f,%f\n",floor,GetPosition(),distance,distance-floor);

	if(fabs(GetFloor() - floor) < 0.01) {
		CString status;
		status.Format(_T("Up\n[%d]¥"),floor);
		ViewStatus(status);

		printf("Up ��[%d]¥����\n",floor);
		// ������UP�� Call Light Off
		SetCallLight(floor,true,false);
		// ������¥���Floor Light Off
		SetPanelFloorLight(floor,false);
		SetMotorPower(0);
		
		// ����
		SetDoor(floor,true);
		*state = DoorOpen;
	}

}

void StateMovingDown(int *state)
{
	int floor = GoingDownToFloor(); // ����Ƿ���һ¥����Ҫ����¥��

	double distance = GetFloor();
	//printf("StateMovingDown %d,%f,%f,%f\n",floor,GetPosition(),distance,distance-floor);

	if(fabs(GetFloor() - floor) < 0.01) {
		CString status;
		status.Format(_T("Down\n[%d]¥"),floor);
		ViewStatus(status);

		printf("Down ��[%d]¥����\n",floor);
		// ������Down��Call Light Off
		SetCallLight(floor,false,false);  // �������������Ϊ��up�����˲�ģ�Ӧ����upΪlight off
		// ������¥���Floor Light Off
		SetPanelFloorLight(floor,false);
		SetMotorPower(0);

		// ����
		SetDoor(floor,true);
		*state = DoorOpen;
	}

}

/**********************************************
 * ���ţ�һ��ʱ����Զ��������
 **********************************************/
void StateDoorOpen(int *state)
{
	int floor = GetNearestFloor();
	// ������Ž������������״̬
	if(IsDoorOpen(floor))
	{
		// ����
		SetDoor(floor,false);
		*state = DoorClosing;
	} 
	else // ����
	{
		SetDoor(floor,true);
	}
}

/********************************************
 * ���ڹ��ţ�ʡ�Ժ���̽��
 ********************************************/
void StateDoorClosing(int *state)
{
	// ������Ž�����������״̬���ж���һ��������
	if(IsDoorClosed(GetNearestFloor()))
	{
		*state = Idle;
	} 
}

/***********************************************
 * This loop runs until we shut the elevator system down
 * by closing the window it runs in.
 ***********************************************/
void main_control(int *state)
{  
    if(IsElevatorRunning())
    {
		switch(*state)
		{
		case Idle:
			StateIdle(state);
			break;
		case MovingUp:
			StateMovingUp(state);
			break;
		case MovingDown:
			StateMovingDown(state);
			break;
		case DoorOpen:
			StateDoorOpen(state);
			break;
		case DoorClosing:
			StateDoorClosing(state);
			break;
		default:
			printf("û������״̬!!!\n");  
		}
    }
}
