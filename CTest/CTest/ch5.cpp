#include "stdafx.h"
#include <stdio.h>

// p106,����1!+2!+3!+...+10!
void p106_2()
{
	int i,j;
	long int fac;     // �׳�
	long int sum=0; // sum���ܺ�

	int n=10;
	for (i=1;i<=n;i++)
	{
		fac=1;
		for (j=1;j<=i;j++) fac *= j;
		sum += fac;
	}
	printf("sum=%ld\n",sum);
}

void ch5()
{
	// p106,����1!+2!+3!+...+10!
	p106_2();
	getchar();
}
