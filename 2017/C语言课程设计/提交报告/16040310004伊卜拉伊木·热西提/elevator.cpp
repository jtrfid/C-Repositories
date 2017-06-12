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
	floor = IdleWhatFloorToGoTo(&up);//��ȡҪȥ��¥��
	if (floor > 0 && up){            //�Ϸ���������
		SetMotorPower(1);            //������
		*state = MovingUp;           //����state
		printf("Transition: from Idle to MovingUp.\n");
	    }
	if (floor > 0 && !up){           //�Ϸ���������
		SetMotorPower(-1);
		*state = MovingDown;
		printf("Transition: from Idle to MovingDown.\n");
	}
	if (GetOpenDoorLight()){         //��鿪�Ű�ť ��������
		SetOpenDoorLight(false);     //�رտ��ŵ� 
		floor = GetNearestFloor();   //��ȡ��ǰ¥��
		SetDoor(floor,true);         //����ǰ¥����
		*state = DoorOpen;
		printf("Transition: from Idle to DoorOpen.\n");
	    }
	if (GetCallLight(GetNearestFloor(), true) || GetCallLight(GetNearestFloor(),false)){//��ǰ¥����еƱ�����
		floor = GetNearestFloor();
		SetDoor(floor, true);         //����
		SetCallLight(floor, true, false);//�رպ��е�
		SetCallLight(floor, false, false);
		*state = DoorOpen;
		printf("Transition: from Idle to DoorOpen.\n");
	    }
	if (GetCloseDoorLight()) { //���Ű�ť������ �رչ��Ű�ť�� �ö�����Ч 
		SetCloseDoorLight(false); 
	    }

}

void StateMovingUp(int *state)
{  
	int floor;
	SetOpenDoorLight(false);                    //�رտ����ŵ� �����Ŷ���ʧЧ
	SetCloseDoorLight(false);
	floor = GoingUpToFloor();                   //��ȡҪȥ��¥��
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance){//��ǰ¥���Ŀ��¥����С�����
		SetMotorPower(0);                       //ͣ
		SetDoor(floor, true);                   //����
		*state = DoorOpen;
		printf("Transition: from MovingUp to DoorOpen.\n");
		if (floor == Lib_FloorNum)             //3¥�Ļ� �ر��º��е�
	    	SetCallLight(floor, false, false);
		else{                                  //�������¶���
			SetCallLight(floor, true, false);
			SetCallLight(floor, false, false);
		}
		SetPanelFloorLight(GetNearestFloor(), false);//�ر�¥�㰴ť��
	}
}

void StateMovingDown(int *state)
{
	int floor;
	SetOpenDoorLight(false);
	SetCloseDoorLight(false);
	floor = GoingDownToFloor();
	if (fabs(GetFloor() - floor) < Lib_FloorTolerance){
		SetMotorPower(0);
		SetDoor(floor, true);
		*state = DoorOpen;
		printf("Transition: from MovingDown to DoorOpen.\n");
		if (floor == 1)
			SetCallLight(floor, true, false);
		else{
			SetCallLight(floor, false, false);
			SetCallLight(floor, true, false);
		}
		SetPanelFloorLight(GetNearestFloor(), false);
	}
}

void StateDoorOpen(int *state)
{
	if (GetCloseDoorLight()){      //�����ŵ� ����
		SetDoor(GetNearestFloor(), false);
		*state = DoorClosing;
		SetCloseDoorLight(false);  //�رչ��ŵ� 
		printf("Transition: from DoorOpen to DoorClosing.\n");
	}
	if (IsDoorOpen(GetNearestFloor())){//���Ѿ�����
		SetDoor(GetNearestFloor(), false);//����
		*state = DoorClosing;
		printf("Transition: from DoorOpen to DoorClosing.\n");
	}
	if (GetOpenDoorLight()) //���Ű�ťʧЧ
		SetOpenDoorLight(false);
}

void StateDoorClosing(int *state)
{
	if (GetOpenDoorLight()){      //���Ű�ť������
		SetDoor(GetNearestFloor(), true);//����
		SetOpenDoorLight(false);   //�رտ��ŵ�
		*state = DoorOpen;
		printf("Transition: from DoorClosing to DoorOpen.\n");
	}
	if (GetCloseDoorLight())    //���Ű�ťʧЧ
		SetCloseDoorLight(false);
	if (IsBeamBroken()){        //���ϰ��� תΪ����
		SetDoor(GetNearestFloor(), true);
		*state = DoorOpen;
		printf("Transition: from DoorClosing to DoorOpen.\n");
	}
	if (IsDoorClosed(GetNearestFloor()))//���Ѿ��ر�
		*state = Idle;                  //����״̬
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
