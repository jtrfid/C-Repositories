#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
int s,m=8,*q;
void kdl(int *ql,int *front,int *rear)
{
	int i;
	ql=(int *)malloc(m*sizeof(int));
	*front=*rear=m-1;
	s=0;
	for(i=0;i<m;i++)
		*(ql+i)=0;
	q=ql;
}

void add(int q[],int *rear,int *front,int x)
{
	if((s==1)&&(*rear==*front))
	{
		printf("Queue-OVERFLOW  \n");
		return;
	}
	*rear=*rear+1;
	if(*rear==m)
		*rear=0;
	q[*rear]=x;
	s=1;
	return;
}

void del(int q[], int *rear, int *front,int *y)
{
	if(s==0)
	{
		printf("Queue-UNDERFLOW  \n");
		return;
	}
	*front=*front+1;
	if(*front==m)
		*front=0;
	*y=q[*front];
	if(*front==*rear)
		s=0;
	return;
}

void linktest()
{
	int *front,*rear,*y,i;
	kdl(q,front,rear);
	printf("\n\n");
	for(i=0;i<m;i++)
		printf("%5d",*(q+i));
	printf("\n\n");
	add(q,rear,front,0);
	for(i=0;i<6;i++)
		add(q,rear,front,i+65);
	printf("\n\n");
	for(i=0;i<7;i++)
	{
		del(q,rear,front,y);
		printf("%5c",*y);
		getchar();
	}
	printf("\n\n");
	for(i=0;i<2;i++)
	{
		add(q,rear,front,i+88);
		printf("\n\n");
		for(i=0;i<2;i++)
		{
			del(q,rear,front,y);
			printf("%5c",*y);
			getchar();
		}
		printf("\n\n");
		for(i=0;i<5;i++)
			add(q,rear,front,i+75);
		printf("\n\n");
		for(i=0;i<5;i++)
		{
			del(q,rear,front,y);
			printf("%5c",*y);
			getchar();
		}
	}

}
