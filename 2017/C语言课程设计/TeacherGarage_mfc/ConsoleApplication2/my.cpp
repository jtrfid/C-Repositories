#include "stdafx.h"

void fun1(int a, int *state)
{
	a = 10;
	*state = 20;
	printf("fun1()\n");
}