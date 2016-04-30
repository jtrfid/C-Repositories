#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ElevatorLib.h"

/**********************************************
 * ����״̬���ж��Ƿ��а������жϽ�Ҫ�������״̬
 **********************************************/
void StateIdle(int *state)
{
	int floor;
	bool up;
	int CurrentFooor = GetNearestFloor();

	// �����������°�ť��(Call Light)��������
	// ����ĵ��������°�ť��(Call Light)����������̬������֤����һ��,�����߶���,�ָ���ֵ
	// ���°�ť����һ�ο��Ż���
	static bool openUpOnes = false;
	static bool openDownOnes = false;
	
	if(!openUpOnes && GetCallLight(CurrentFooor,true)) {  // ����
		openUpOnes = true;
		SetDoor(CurrentFooor,true);
		*state = DoorOpen;
		return;
	}
	if(!openDownOnes && GetCallLight(CurrentFooor,false)) { // ����
		openDownOnes = true;
		SetDoor(CurrentFooor,true);
		*state = DoorOpen;
		return;
	}

	// �������ڿ����Ű�ť
	if(GetOpenDoorLight()) { // ����
		SetDoor(CurrentFooor,true);
		*state = DoorOpen;
		return;
	}
	else if(GetCloseDoorLight()) {  // ����
		SetDoor(CurrentFooor,false);
		*state = DoorClosing;
		return;
	}

	// ��̬��⣬��һ����Ҫ����һ�㣨Ŀ��㣩
	floor = IdleWhatFloorToGoTo(&up);
	if(floor > 0)
	   printf("����״̬����Ҫ����¥��:%d,����:%s\n",floor,up?"����":"����");

	if (floor > 0) {
		openUpOnes = false;   // �����ŵƸ�λ
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
	int floor = GoingUpToFloor(); // ���Ŀ��¥��

	//double distance = GetFloor();
	//printf("StateMovingUp %d,%f,%f,%f\n",floor,GetPosition(),distance,distance-floor);

	if(fabs(GetFloor() - floor) < Lib_FloorTolerance) {
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
	int floor = GoingDownToFloor(); // ���Ŀ��¥��

	//double distance = GetFloor();
	//printf("StateMovingDown %d,%f,%f,%f\n",floor,GetPosition(),distance,distance-floor);

	if(fabs(GetFloor() - floor) < Lib_FloorTolerance) {
		CString status;
		status.Format(_T("Down\n[%d]¥"),floor);
		ViewStatus(status);

		printf("Down ��[%d]¥����\n",floor);
		// ������Down��Call Light Off
		SetCallLight(floor,false,false);  
		// ������¥���Floor Light Off
		SetPanelFloorLight(floor,false);
		SetMotorPower(0);

		// ����
		SetDoor(floor,true);
		*state = DoorOpen;
	}

}

/**********************************************
 * ���ţ����Ž������Զ��������
 **********************************************/
void StateDoorOpen(int *state)
{
	int floor = GetNearestFloor();

	// ��������ڿ��Ű�ť����Ŀ��ţ��ص��˰�ť�ĵơ�����Idle״̬��⵽�ƻ����������ظ����š�
	if(GetOpenDoorLight()) SetOpenDoorLight(false);

	// ������Ž������������״̬
	if(IsDoorOpen(floor))
	{
		// ����
		SetDoor(floor,false);
		*state = DoorClosing;
	} 
}

/********************************************
 * ���ڹ��ţ�ʡ�Ժ���̽��
 ********************************************/
void StateDoorClosing(int *state)
{
	// ��������ڹ��Ű�ť����Ĺ��ţ��ص��˰�ť�ĵơ�����Idle״̬��⵽�ƻ����������ظ������š�
	if(GetCloseDoorLight()) SetCloseDoorLight(false);

	// ������Ž�����������״̬���ж���һ��������
	if(IsDoorClosed(GetNearestFloor()))
	{
		*state = Idle;
	} 
}

/***********************************************
 * ״̬����ÿ��һ��ʱ��(�磬100ms)������һ�Σ��ɼ�ϵͳ������״̬
 ***********************************************/
void main_control(int *state)
{  
    if(IsElevatorRunning())
    {
		switch(*state)
		{
		case Idle:
			// һ��ʱ���޶������Զ���һ¥
			if(GetNearestFloor() !=1 ) {
				AutoTo1Floor();
			}
			StateIdle(state);
			break;
		case MovingUp:
			CancelTo1Floor();
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
