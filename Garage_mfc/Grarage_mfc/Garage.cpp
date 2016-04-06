#include <stdafx.h>
#include "Garage.h"

/* Our possible garage door states */
#define DoorClosed 1
#define DoorOpening 2
#define DoorOpen 3
#define DoorClosing 4

int a = 10;  // ����ȫ�ֱ�����ȫ�ֱ���ֻ�ܶ���һ��

bool Running = false;
bool ButtonPressed = false;

void print_test()
{	
	printf("\nprintf_test(),a=%d\n",a);
	a = 20;
}


bool IsGarageRunning()
{
	return Running;
}

void GarageStartup()
{
	// �����������,�������ڵ�����״̬
	if(Running) {
		printf("������������!!\n");
		return; 
	}
	Running = true;
	ButtonPressed = false;
	printf("��ʼ����!!\n");
}

void GarageShutdown()
{
	// �����������״̬,�������ڵķ�����״̬
	if(!Running) {
		printf("����û�п�ʼ!!\n");
		return;
	}
	Running = false;
	printf("��������!!\n");
}

bool WasButtonPressed()
{
	return ButtonPressed;
}

void StateDoorClosed(int *state)
{
	if(WasButtonPressed())
	{
		//SetMotorPower(1);
		*state = DoorOpening;
	}
}

/** ÿ200ms������һ�Σ���������Ѿ���ʼ����⳵���ŵ�״̬ */
void main_control()
{
	static int state = DoorClosed;  // ��ʼ�ǹ���״̬

	if (IsGarageRunning())
	{
		printf("���ڵ�״̬��state=%d\n",state);
		switch(state)
		{
		case DoorClosed:
			StateDoorClosed(&state);
			break;
		case DoorOpening:
			//StateDoorOpening(&state);
			break;
		}
	}
}

