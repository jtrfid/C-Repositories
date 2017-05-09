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
	int CurrentFloor = GetNearestFloor();  // ��ǰ¥��

	// ��̬��⣬��һ����Ҫ����һ�㣨Ŀ��㣩
	floor = IdleWhatFloorToGoTo(&up);  // Event

	// Idle --> MovingUp/MovingDown
	if (floor > 0) {
		printf("����״̬����Ҫ����¥�㡾Ŀ��¥�㡿:%d,����:%s\n", floor, up ? "����" : "����");
		if (up) {
			SetMotorPower(1);   // Transition
			*state = MovingUp;
			printf("Transition:  from Idle to MovingUp.\n");
		}
		else {
			SetMotorPower(-1); // Transition
			*state = MovingDown;
			printf("Transition:  from Idle to MovingDown.\n");
		}
	}

	// Idle --> DoorOpen
	// �����������°�ť��(Call Light)����������
	if (GetCallLight(CurrentFloor, true) || GetCallLight(CurrentFloor, false) ) {  // Event
		// ���ѵ�����Up/Down��ť�ƣ�Call Light Off
		SetCallLight(CurrentFloor, true, false);
		SetCallLight(CurrentFloor, false, false);
		// ����
		SetDoor(CurrentFloor, true);  // Transition
		*state = DoorOpen;
		printf("Transition:  from Idle to DoorOpen.\n");
		return;
	}

	// �������ڿ����Ű�ť
	if(GetOpenDoorLight()) { // ����    Event
		SetOpenDoorLight(false); // turn off, �صƣ�Ϊ�˶�ȡһ����Ч�������ظ������Ѱ�ť��
		SetDoor(CurrentFloor,true);  // Transition
		*state = DoorOpen;
		printf("Transition:  from Idle to DoorOpen.\n");
		return;
	}

	// Idle --> Idle
	// ������Idle״̬����һ���ǹرյ�, ���Ӧ�ò�ִ�д�Idle��DoorClosing��ת��
	// ����ȡ���ŵƣ����رչ��ŵƣ������Ѱ�����Ϊ��
	else if(GetCloseDoorLight()) {  // ����
		SetCloseDoorLight(false); // turn off, �صƣ�Ϊ�˶�ȡһ����Ч�������ظ������Ѱ�ť��
		ASSERT(IsDoorClosed(CurrentFloor));  // ������һ���ǹرյ�
		return;
		/********
		SetDoor(CurrentFloor,false);
		*state = DoorClosing;
		printf("Transition:  from Idle to DoorClosing.\n");
		return;
		*********/
	}
}

void StateMovingUp(int *state)
{
	// ��ȫ���ã��˶�״̬��������ʧЧ��������Ӧ��ť��Ϊ
	if (GetOpenDoorLight()) { // ���ѿ��Ű�ť
		SetOpenDoorLight(false); 
		printf("�˶�״̬��������ʧЧ!!!\n");
		return;
	}
	if (GetCloseDoorLight()) { // ���ѹ��Ű�ť
		SetCloseDoorLight(false); 
		printf("�˶�״̬��������ʧЧ!!!\n");
		return;
	}

	int floor = GoingUpToFloor(); // ���Ŀ��¥��
	
	//double distance = GetFloor();
	//printf("StateMovingUp %d,%f,%f,%f\n",floor,GetPosition(),distance,distance-floor);

	if(fabs(GetFloor() - floor) < Lib_FloorTolerance) {  // Event
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
		SetMotorPower(0);   // Transition
		
		// ����
		SetDoor(floor,true);
		*state = DoorOpen;
		printf("Transition:  from MovingUp to DoorOpen.\n");
	}

}

void StateMovingDown(int *state)
{
	// ��ȫ���ã��˶�״̬��������ʧЧ��������Ӧ��ť��Ϊ
	if (GetOpenDoorLight()) { // ���ѿ��Ű�ť
		SetOpenDoorLight(false);
		printf("�˶�״̬��������ʧЧ!!!\n");
		return;
	}
	if (GetCloseDoorLight()) { // ���ѹ��Ű�ť
		SetCloseDoorLight(false);
		printf("�˶�״̬��������ʧЧ!!!\n");
		return;
	}

	int floor = GoingDownToFloor(); // ���Ŀ��¥��

	//double distance = GetFloor();
	//printf("StateMovingDown %d,%f,%f,%f\n",floor,GetPosition(),distance,distance-floor);

	if(fabs(GetFloor() - floor) < Lib_FloorTolerance) {  // Event
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
		SetMotorPower(0);  // Transition

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
	int floor = GetNearestFloor();  // ��ǰ¥��

	// ������ڿ���ʱ�����˹��ŵƣ�ת������
	if (GetCloseDoorLight()) {  // Event
		SetCloseDoorLight(false); // turn off, �صƣ�Ϊ�˶�ȡһ����Ч�������ظ� 
		SetDoor(floor, false);  // Trasition
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

	// ������Ž������Զ����й��ţ��������״̬
	if(IsDoorOpen(floor))  // Event, ����һ��ʱ��󣬿��Ž�����ת���Զ��������״̬
	{
		// ����
		SetDoor(floor,false); // Transition
		*state = DoorClosing;
		printf("Transition:  from DoorOpen to DoorCloing.\n");
	} 
}

/********************************************
 * ���ڹ��ţ�ʡ�Ժ���̽��
 ********************************************/
void StateDoorClosing(int *state)
{
	int floor = GetNearestFloor();  // ��ǰ¥��

	// ������ڹ���ʱ�����˿��ŵƣ�ת������
	if (GetOpenDoorLight()) {   // Event
		SetOpenDoorLight(false); // turn off, �صƣ�Ϊ�˶�ȡһ����Ч�������ظ� 
		SetDoor(floor, true);  // Trasition
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
	if(IsDoorClosed(floor))  // Event
	{
		*state = Idle;  // Transition
		printf("Transition:  from DoorClosing to Idle.\n");
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
