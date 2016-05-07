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
	int floor;  // Ŀ��¥��
	bool up;    // ����/�½�
	int CurrentFooor = GetNearestFloor();  // ��ǰ¥��

	// ��̬��⣬��һ����Ҫ����һ�㣨Ŀ��㣩
	floor = IdleWhatFloorToGoTo(&up);
	if (floor > 0)
		printf("����״̬����Ҫ����¥�㡾Ŀ��¥�㡿:%d,����:%s\n", floor, up ? "����" : "����");

	// �����������°�ť��(Call Light)����������
	if (up && GetCallLight(CurrentFooor, true)) {  // ����
		// ������Up��Call Light Off
		SetCallLight(CurrentFooor, true, false);
		// ����
		SetDoor(CurrentFooor, true);
		*state = DoorOpen;
		printf("Transition:  from Idle to DoorOpen.\n");
		return;
	}

	if (!up && GetCallLight(CurrentFooor, false)) { // ����
		// ������Down��Call Light Off
		SetCallLight(CurrentFooor, false, false);
		// ����
		SetDoor(CurrentFooor, true);
		*state = DoorOpen;
		printf("Transition:  from Idle to DoorOpen.\n");
		return;
	}

	// �������ڿ����Ű�ť
	if(GetOpenDoorLight()) { // ����
		SetOpenDoorLight(false); // turn off, �صƣ�Ϊ�˶�ȡһ����Ч�������ظ� 
		SetDoor(CurrentFooor,true);
		*state = DoorOpen;
		printf("Transition:  from Idle to DoorOpen.\n");
		return;
	}
	else if(GetCloseDoorLight()) {  // ����
		SetCloseDoorLight(false); // turn off, �صƣ�Ϊ�˶�ȡһ����Ч�������ظ� 
		SetDoor(CurrentFooor,false);
		*state = DoorClosing;
		printf("Transition:  from Idle to DoorClosing.\n");
		return;
	}

	if (floor > 0) {
		if (up) {
			// �����up call light off
			SetCallLight(CurrentFooor,true,false);
			SetMotorPower(1);
			*state = MovingUp;
			printf("Transition:  from Idle to MovingUp.\n");
		}
		else {
			// �����down call light off
			SetCallLight(CurrentFooor,false,false);
			SetMotorPower(-1);
			*state = MovingDown;
			printf("Transition:  from Idle to MovingDown.\n");
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

		// ���������߲㣬������Down��Call Light Off
		if (floor == Lib_FloorNum) SetCallLight(floor, false, false);

		// ������¥���Floor Light Off
		SetPanelFloorLight(floor,false);

		// ֹͣ
		SetMotorPower(0);
		
		// ����
		SetDoor(floor,true);
		*state = DoorOpen;
		printf("Transition:  from MovingUp to DoorOpen.\n");
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

		// �������1¥��������Up��Call Light Off
		if (floor == 1) SetCallLight(floor, true, false);

		// ������¥���Floor Light Off
		SetPanelFloorLight(floor,false);

		// ֹͣ
		SetMotorPower(0);

		// ����
		SetDoor(floor,true);
		*state = DoorOpen;
		printf("Transition:  from MovingDown to DoorOpen.\n");
	}

}

/**********************************************
 * ���ţ����Ž������Զ��������
 **********************************************/
void StateDoorOpen(int *state)
{
	int floor = GetNearestFloor();

	// ������ڿ���ʱ�����˹��ŵƣ�ת������
	if (GetCloseDoorLight()) {
		SetCloseDoorLight(false); // turn off, �صƣ�Ϊ�˶�ȡһ����Ч�������ظ� 
		SetDoor(floor, false);
		*state = DoorClosing;
		printf("Transition:  from DoorOpen to DoorClosing.\n");
		return;
	}

	// ������ڿ��ţ����˿��ŵƣ��صƷ���
	if (GetOpenDoorLight()) {
		SetOpenDoorLight(false); // turn off, �صƣ�Ϊ�˶�ȡһ����Ч�������ظ�
		printf("�Ѿ��ڿ��ţ�\n");
		return;
	}

	// ������Ž������������״̬
	if(IsDoorOpen(floor))
	{
		// ����
		SetDoor(floor,false);
		*state = DoorClosing;
		printf("Transition:  from DoorOpen to DoorCloing.\n");
	} 
}

/********************************************
 * ���ڹ��ţ�ʡ�Ժ���̽��
 ********************************************/
void StateDoorClosing(int *state)
{
	int floor = GetNearestFloor();

	// ������ڹ���ʱ�����˿��ŵƣ�ת������
	if (GetOpenDoorLight()) { 
		SetOpenDoorLight(false); // turn off, �صƣ�Ϊ�˶�ȡһ����Ч�������ظ� 
		SetDoor(floor, true);
		*state = DoorOpen;
		printf("Transition:  from DoorClosing to DoorOpen.\n");
		return;
	}

	// ������ڹ��ţ����˹��ŵƣ��صƷ���
	if (GetCloseDoorLight()) {
		SetCloseDoorLight(false); // turn off, �صƣ�Ϊ�˶�ȡһ����Ч�������ظ�
		printf("�Ѿ��ڹ��ţ�\n");
		return;
	}

	// ������Ž�����������״̬���ж���һ��������
	if(IsDoorClosed(floor))
	{
		*state = Idle;
		printf("Transition:  from DoorClosing to Idle.\n");
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
