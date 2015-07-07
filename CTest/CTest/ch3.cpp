#include <stdafx.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>

void lf_test()
{
	printf("======lf_test()\n");
	float a;
	double b;
	printf("第一次输入a,b\n");
	scanf("%f%f",&a,&b);
	printf("a=%f,b=%lf\n",a,b);
	printf("第二次输入a,b\n");
	scanf("%f%lf",&a,&b);
	printf("a=%f,b=%lf\n",a,b);
	/***
	15 20
	a=15.000000,b=0.000000
	15 20
	a=15.000000,b=20.000000
	***/
		
	printf("回显测试\n");
	char ch;
	ch=getche(); 
	//ch=getch();
	putchar(ch);
	putchar('\n');
	getchar();

}

/**********************************
 * 已知三条边a、b、c，求三角形面积   *
 **********************************/
void area1()
{
	float a,b,c,s,area;

	printf("请输入a,b,c\n");
	scanf("%f,%f,%f",&a,&b,&c);  // 输入必须用逗号隔开。如3,4,5
	//scanf("%f,%f,%f\n",&a,&b,&c);  // 输入必须用逗号隔开。两次回车接收输入,如3,4,5回车 3,4,5回车
    //scanf("%f%f%f",&a,&b,&c);    // 输入必须用空格隔开。推荐使用,如3 4 5

	//s =1/2*(a+b+c);  // error, s=0
	//s =1.0/2*(a+b+c);  // ok, 整数相除，自动截断；实型常量，不要忘了'.'
	//s =1/2.0*(a+b+c);  // ok
	//s =1.0/2.0*(a+b+c);  // ok

	// 两边之和大于第三边，差小于第三边
	// Calculate the absolute value
	// int abs(int n)
	// double fabs(double x)
	if(a+b<c || fabs(a-b)>c || a+c<b || fabs(a-c)>b || b+c<a || fabs(b-c)>a) {
		printf("不能构成三角形！");
		return;
	}
	s =0.5*(a+b+c);  // ok
	area = sqrt(s*(s-a)*(s-b)*(s-c));
	printf("area = %8.3f\n",area);
}

// 学习指导，p33,3.编程题 (32)
// 计算：1-1/2+1/3-1/4+...+1/99-1/100
void guide_p33()
{
	int i,n,s=1;   // i:循环变量； n： 总项数； s：正负号;
	float sum,k;   // sum：和; k: 代表每一项

	i=1; sum=0.0;
	printf("求n个数的和,输入n\n");
	scanf("%d",&n);
	while(i<=n)
	{ 
		k=(float)s/i;
		//k=s/i;  // 第一项，k=1；第二项k=0.5，两个整数相除，自动截断，k=0
		sum=sum+k;
		s=-s;
		i++;
	}

	printf("sum=%f\n",sum);
}

// 学习指导p154, (3)假设有变量说明,(2)i1++i2%i3
void guide_p154()
{
	char c1='a',c2='B',c3='c';
	int i1=10,i2=20,i3=30;
	double d1=0.1,d2=0.2,d3=0.3;
	printf("%d\n",c1+i2*i3/i2%i1); // 97+20*30/20%30=97+30%30=97
	printf("%d\n",i1+++i2%i3);     // 10+20%30=10+20
	printf("%d\n",c1>i1?i1:c2);    // 上一句中i1++,因此，11
	printf("%d\n",!i1&&i2--);      // 0
	printf("%d\n",i2--);           // 20，上一句中，i2--，未执行，因为a&&b，a为0时，不计算b;即，有0为0
	getchar();
}

void ch3()
{
	printf("======ch3()\n");
	lf_test();
    //area1();
	//guide_p33();
	guide_p154();
}