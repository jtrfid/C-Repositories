#include <stdafx.h>
#include "Garage.h"

/* Our possible garage door states */
#define DoorClosed 1
#define DoorOpening 2
#define DoorOpen 3
#define DoorClosing 4

int a = 10;  // ����ȫ�ֱ�����ȫ�ֱ���ֻ�ܶ���һ��

bool Running = false;
bool ButtonPress = false;

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
	Running = true;
	printf("��ʼ����....\n");
	main_control();
}

void GarageShutdown()
{
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

void main_control()
{
	int state = DoorClosed;

	while(IsGarageRunning())
	{
		printf("��������...\n");
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

