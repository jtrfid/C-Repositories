// ConsoleApplication2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "my.h"

int _tmain(int argc, _TCHAR* argv[])
{
	char ch ='0';
	while (1)
	{
		if (ch != '\n') printf("输入1,2,输入0结束:\n");
		ch = getchar(); // getchar是带缓冲区的函数，输入12回车,'1'
		if (ch == '0') break;
		switch (ch)
		{
		case '1':
			int c, d;
			c = 100, d = 200;
			fun1(c,&d);
			printf("c,d=%d,%d\n", c, d);
			break;
		case '2':
			printf("press 2\n");
			break;
		case '\n':
			//printf("========%d\n",ch);
			break;
		default:
			printf("error!!\n");
		}
	}
	getchar();
	return 0;
}



