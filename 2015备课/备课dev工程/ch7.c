//#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <Windows.h>

/****************************************************************************************
 ******************************** hanoni_show1()
 ****************************************************************************************/

//���ܣ���ŵ�������㷨�Ŀ���̨������ʾ����
#define MAX 5    //�������߶ȣ���������Ҳ��Բ�̵�������������С
//һ����ջ������ģ������
struct stack
{
	int space[MAX];//��ջ�ռ䣬���������ĸ��㣬��ŵ����ִ���Բ�̵Ĵ�С
	int p;//ջ��ָ��
}stacks[3];//��������

char star[MAX+1]; //������ǵ����飨���ڱ�ʾԲ�̣�
char space[MAX+1];//��ſո������

// ��ջ����ʾ����һ��Բ�̣�
// stackNum�����ţ�number��Բ�̴�С
void push(int stackNum,int number)
{
	if(stacks[stackNum].p>=MAX)
	{
		printf("�����޷���ջ���ѵ���ջ����\n");
		exit(0);
	}
	stacks[stackNum].space[stacks[stackNum].p++]=number;
}
// ��ջ����ʾȡ��һ��Բ�̣�
// stackNum�����ţ�return����Բ�̴�С
int pop(int stackNum)
{
	if(stacks[stackNum].p<=0)
	{
		printf("�����޷���ջ���ѵ���ջ�ס�\n");
		exit(0);
	}
	return stacks[stackNum].space[--stacks[stackNum].p];
}

//��ʾ��ǰ����״̬
void show()
{
	int charCount;//ͳ�Ƶ�ǰ�л����˶��ٸ��ַ�
	int i;
	//system("cls");//��������
	//��߲㿪ʼ��������ͼ
	int taP= MAX;
	while(taP--)
	{
		for(i=0;i<3;i++)
		{
			charCount = 0;
			//���˲���Բ�����ͻ���
			if(taP<=stacks[i].p-1)
			{
				printf("%s",&star[MAX-stacks[i].space[taP]]);
				charCount+=stacks[i].space[taP];
			}
			//��ȫ�ո�
			printf("%s|",&space[charCount]);
		}
		printf("\n");
	}
	//sleep(500);
	//getchar();
}

//�ƶ�Բ������a->��b
void move(int a,int b)
{
	push(b,pop(a));
	printf("move %c to %c\n",a+'A',b+'A');
	getchar();
}

//��ŵ�����㷨��n��Բ������a~c����������
// ��n���̴�a�Ƶ�b������c
void Hanoi_1(int n,int a,int b,int c)
{
	// �ƶ�Բ��
	if(n>0)
	{
		Hanoi_1(n-1,a,c,b); // n-1,a->c,����b
		move(a,b);        // 1����a->b
		show();           //��ʾ��ŵ��
		Hanoi_1(n-1,c,b,a); // n-1,c->b,����a
	}
}

//��ŵ�����㷨��n��Բ������a~c����������
// ��n���̴�a�Ƶ�c������b
void Hanoi_2(int n,int a,int b,int c)
{
	// �ƶ�Բ��
	if(n>0)
	{
		Hanoi_2(n-1,a,c,b); // n-1,a->b,����c
		move(a,c);          // 1����a->c
		show();             //��ʾ��ŵ��
		Hanoi_2(n-1,b,a,c); // n-1,b->c,����a
	}
}

//��ŵ�����㷨��n��Բ������a~c����������
// ��n���̴�a�Ƶ�c������b
void Hanoi_3(int n,int a,int b,int c)
{
	if (n==1) {
		printf("1111\n");
		move(a,c);   // 1����a->c
		show();      //��ʾ��ŵ��
	}
	else {
		Hanoi_3(n-1,a,c,b); // n-1,a->b,����c
		move(a,c);          // 1����a->c
		show();             //��ʾ��ŵ��
		Hanoi_3(n-1,b,a,c); // n-1,b->c,����a
	}
}

//��ʼ��һЩ����
void init()
{
	stacks[0].p = stacks[1].p = stacks[2].p =0;
	int i;
	for(i=0;i<MAX;i++)
	{
		star[i]='*';    //����һ���������飬���ڻ�ͼ
		space[i]=' ';   //����һ���ո����飬���ڻ�ͼ
	}
	space[i]=star[i]='\0';
}
void hanoi_show1()
{
	init();
	//����Բ��:����������������η����СΪ3~1��Բ��
	//push(0,5);
	//push(0,4);
	push(0,3);
	push(0,2);
	push(0,1);
	show();         //��ʾ����Բ����ĺ�ŵ��
	//Hanoi_1(5,0,1,2); // ��n���̴�a�Ƶ�b������c ok
	//Hanoi_2(5,0,1,2); // ��n���̴�a�Ƶ�c������b ok

	//Hanoi_1(3,0,1,2); // ��n���̴�a�Ƶ�b������c  ok
	//Hanoi_2(3,0,1,2); // ��n���̴�a�Ƶ�c������b  ok

	//Hanoi_1(3,0,2,1);  // = Hanoi_2(3,0,1,2);  ok

	Hanoi_3(3,0,1,2);  // ��n���̴�a�Ƶ�c������b ok
}

/****************************************************************************************
 ******************************** hanoni_show1()   end
 ****************************************************************************************/


/*************
 ��д�������������Ͳ���m��ֵ�����㹫ʽ��
     t=1-1/2*2-1/3*3-...-1/m*m
 ��ֵ�����磺m=5,�����0.536389
 *************/
double ch7_1(int m)
{
	printf("=======ch7_1()\n");
	double t=1.0;
	int i;
	
	for(i=2;i<=m;i++) 
	{
		t-=1.0/(i*i);
	}
	return (t);
}

/*****
 �ú����ݹ鷽�����ַ�����ʽ���һ��������
 *****/
void ch7_2(char *s,int n)
{
	printf("=======ch7_2()\n");
	int n1,i=0;
	// s�洢n������
	while(n!=0) 
	{ 
		n1=n%10; 
		n=n/10;
		s[i++]='0'+n1;
	} 
	
	// ����
	int j,tmp;
	for(j=0;j<i/2;j++) {
		tmp=s[j];
		s[j]=s[i-j-1];
		s[i-j-1]=tmp;
	}

	// �ַ���ĩβ��'\0'
	s[i]='\0';
	printf("s=%s\n",s);
}

/*****
 �ú����ݹ鷽�����ַ�����ʽ���һ��������
 ͬp153����7.20
 *****/
void ch7_3(int n)
{
	printf("=======ch7_3()\n");
	if(n/10) ch7_3(n/10);
	putchar(n%10+'0');
}

/*****
 �ú����ݹ鷽�����ַ�����ʽ���һ��������
 *****/
void ch7_4(char *s,int n)
{
	printf("=======ch7_4()\n");
	if(n/10) ch7_4(s,n/10);
	int len=strlen(s);
	s[len]=n%10+'0';
	s[len+1]='\0';
}

void ff()
{  
   printf("========ff()\n");
   int a,b=3;
   static int c,d=5;
   a=3;  c=5;
   a++;  b++;  c++;  d++;
   printf("%d\t%d\t%d\t%d\n",a,b,c,d);
} 

void ch7()
{
	ff();
	ff();
    printf("t=%lf\n",ch7_1(5));

	// �ݹ�
	char s[10];
	ch7_2(s,12345);
	printf("s=%s\n",s);
	ch7_3(12345);
	putchar('\n');
	s[0]='\0';
	ch7_4(s,12345);
	printf("s=%s\n",s);
	//////////////////

	//hanoi_show1();
}
