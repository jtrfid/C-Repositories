//#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <Windows.h>

/****************************************************************************************
 ******************************** hanoni_show1()
 ****************************************************************************************/

//功能：汉诺塔问题算法的控制台动画演示程序
#define MAX 5    //塔的最大高度（层数），也是圆盘的最大个数与最大大小
//一个堆栈，用于模拟塔柱
struct stack
{
	int space[MAX];//堆栈空间，用于塔柱的各层，存放的数字代表圆盘的大小
	int p;//栈顶指针
}stacks[3];//三个塔柱

char star[MAX+1]; //存放星星的数组（用于表示圆盘）
char space[MAX+1];//存放空格的数组

// 入栈，表示放入一个圆盘：
// stackNum：塔号，number：圆盘大小
void push(int stackNum,int number)
{
	if(stacks[stackNum].p>=MAX)
	{
		printf("错误：无法入栈，已到达栈顶。\n");
		exit(0);
	}
	stacks[stackNum].space[stacks[stackNum].p++]=number;
}
// 出栈，表示取出一个圆盘：
// stackNum：塔号，return：：圆盘大小
int pop(int stackNum)
{
	if(stacks[stackNum].p<=0)
	{
		printf("错误：无法出栈，已到达栈底。\n");
		exit(0);
	}
	return stacks[stackNum].space[--stacks[stackNum].p];
}

//显示当前塔的状态
void show()
{
	int charCount;//统计当前行绘制了多少个字符
	int i;
	//system("cls");//清屏函数
	//最高层开始逐层绘制塔图
	int taP= MAX;
	while(taP--)
	{
		for(i=0;i<3;i++)
		{
			charCount = 0;
			//若此层有圆饼，就绘制
			if(taP<=stacks[i].p-1)
			{
				printf("%s",&star[MAX-stacks[i].space[taP]]);
				charCount+=stacks[i].space[taP];
			}
			//补全空格
			printf("%s|",&space[charCount]);
		}
		printf("\n");
	}
	//sleep(500);
	//getchar();
}

//移动圆饼：柱a->柱b
void move(int a,int b)
{
	push(b,pop(a));
	printf("move %c to %c\n",a+'A',b+'A');
	getchar();
}

//汉诺塔主算法：n：圆饼数，a~c：三个塔柱
// 将n个盘从a移到b，借助c
void Hanoi_1(int n,int a,int b,int c)
{
	// 移动圆饼
	if(n>0)
	{
		Hanoi_1(n-1,a,c,b); // n-1,a->c,借助b
		move(a,b);        // 1个，a->b
		show();           //显示汉诺塔
		Hanoi_1(n-1,c,b,a); // n-1,c->b,借助a
	}
}

//汉诺塔主算法：n：圆饼数，a~c：三个塔柱
// 将n个盘从a移到c，借助b
void Hanoi_2(int n,int a,int b,int c)
{
	// 移动圆饼
	if(n>0)
	{
		Hanoi_2(n-1,a,c,b); // n-1,a->b,借助c
		move(a,c);          // 1个，a->c
		show();             //显示汉诺塔
		Hanoi_2(n-1,b,a,c); // n-1,b->c,借助a
	}
}

//汉诺塔主算法：n：圆饼数，a~c：三个塔柱
// 将n个盘从a移到c，借助b
void Hanoi_3(int n,int a,int b,int c)
{
	if (n==1) {
		printf("1111\n");
		move(a,c);   // 1个，a->c
		show();      //显示汉诺塔
	}
	else {
		Hanoi_3(n-1,a,c,b); // n-1,a->b,借助c
		move(a,c);          // 1个，a->c
		show();             //显示汉诺塔
		Hanoi_3(n-1,b,a,c); // n-1,b->c,借助a
	}
}

//初始化一些变量
void init()
{
	stacks[0].p = stacks[1].p = stacks[2].p =0;
	int i;
	for(i=0;i<MAX;i++)
	{
		star[i]='*';    //创建一个星星数组，便于绘图
		space[i]=' ';   //创建一个空格数组，便于绘图
	}
	space[i]=star[i]='\0';
}
void hanoi_show1()
{
	init();
	//放置圆饼:放入零号塔柱，依次放入大小为3~1的圆饼
	//push(0,5);
	//push(0,4);
	push(0,3);
	push(0,2);
	push(0,1);
	show();         //显示放入圆饼后的汉诺塔
	//Hanoi_1(5,0,1,2); // 将n个盘从a移到b，借助c ok
	//Hanoi_2(5,0,1,2); // 将n个盘从a移到c，借助b ok

	//Hanoi_1(3,0,1,2); // 将n个盘从a移到b，借助c  ok
	//Hanoi_2(3,0,1,2); // 将n个盘从a移到c，借助b  ok

	//Hanoi_1(3,0,2,1);  // = Hanoi_2(3,0,1,2);  ok

	Hanoi_3(3,0,1,2);  // 将n个盘从a移到c，借助b ok
}

/****************************************************************************************
 ******************************** hanoni_show1()   end
 ****************************************************************************************/


/*************
 编写函数，根据整型参数m的值，计算公式：
     t=1-1/2*2-1/3*3-...-1/m*m
 的值。例如：m=5,则输出0.536389
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
 用函数递归方法以字符串形式输出一个整数。
 *****/
void ch7_2(char *s,int n)
{
	printf("=======ch7_2()\n");
	int n1,i=0;
	// s存储n的逆序
	while(n!=0) 
	{ 
		n1=n%10; 
		n=n/10;
		s[i++]='0'+n1;
	} 
	
	// 正序
	int j,tmp;
	for(j=0;j<i/2;j++) {
		tmp=s[j];
		s[j]=s[i-j-1];
		s[i-j-1]=tmp;
	}

	// 字符串末尾补'\0'
	s[i]='\0';
	printf("s=%s\n",s);
}

/*****
 用函数递归方法以字符串形式输出一个整数。
 同p153，例7.20
 *****/
void ch7_3(int n)
{
	printf("=======ch7_3()\n");
	if(n/10) ch7_3(n/10);
	putchar(n%10+'0');
}

/*****
 用函数递归方法以字符串形式输出一个整数。
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

	// 递归
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
