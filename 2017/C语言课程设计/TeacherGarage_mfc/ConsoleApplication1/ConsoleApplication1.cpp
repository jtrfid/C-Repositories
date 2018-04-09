// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "garage.h"

int _tmain(int argc, _TCHAR* argv[])
{
	char ch = '0';
	int state;
	while(1)
	{
		if (ch != '\n') printf("请按键1,2,3,4，0结束。\n");
		ch = getchar(); // 输入字符，回车，下一次从输入缓冲区获取的是上一次的回车键'\n'。
		if (ch == '0') break;
		switch (ch)
		{
		  case '\n':
			  //printf("消费回车键！\n");
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
	printf("Hello world！");
	getchar();
	return 0;
}
