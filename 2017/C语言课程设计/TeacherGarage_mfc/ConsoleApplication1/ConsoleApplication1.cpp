// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "garage.h"

// ģ�⳵����4��״̬�����ĵ���
int _tmain(int argc, _TCHAR* argv[])
{
	char ch = '0';
	int state;
	while(1)
	{
		if (ch != '\n') printf("�밴��1,2,3,4��0������\n");
		ch = getchar(); // �����ַ����س�����һ�δ����뻺������ȡ������һ�εĻس���'\n'��
		if (ch == '0') break;
		switch (ch)
		{
		  case '\n':
			  //printf("���ѻس�����\n");
			  break;
		  case DoorClosed: 
			  StateDoorClosed(&state);
			  break;
		  case DoorOpening:
			  StateDoorOpening(&state);
			  break;
		  case DoorOpen:
			  StateDoorOpen(&state);
			  break;
		  case DoorClosing:
			  StateDoorClosing(&state);
			  break;
		  default:
			  printf("error!\n");
		}
	}
	printf("Hello world��");
	getchar();
	return 0;
}
