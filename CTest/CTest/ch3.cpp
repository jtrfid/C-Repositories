#include <stdafx.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>

void lf_test()
{
	printf("======lf_test()\n");
	float a;
	double b;
	printf("��һ������a,b\n");
	scanf("%f%f",&a,&b);
	printf("a=%f,b=%lf\n",a,b);
	printf("�ڶ�������a,b\n");
	scanf("%f%lf",&a,&b);
	printf("a=%f,b=%lf\n",a,b);
	/***
	15 20
	a=15.000000,b=0.000000
	15 20
	a=15.000000,b=20.000000
	***/
		
	printf("���Բ���\n");
	char ch;
	ch=getche(); 
	//ch=getch();
	putchar(ch);
	putchar('\n');
	getchar();

}

/**********************************
 * ��֪������a��b��c�������������   *
 **********************************/
void area1()
{
	float a,b,c,s,area;

	printf("������a,b,c\n");
	scanf("%f,%f,%f",&a,&b,&c);  // ��������ö��Ÿ�������3,4,5
	//scanf("%f,%f,%f\n",&a,&b,&c);  // ��������ö��Ÿ��������λس���������,��3,4,5�س� 3,4,5�س�
    //scanf("%f%f%f",&a,&b,&c);    // ��������ÿո�������Ƽ�ʹ��,��3 4 5

	//s =1/2*(a+b+c);  // error, s=0
	//s =1.0/2*(a+b+c);  // ok, ����������Զ��ضϣ�ʵ�ͳ�������Ҫ����'.'
	//s =1/2.0*(a+b+c);  // ok
	//s =1.0/2.0*(a+b+c);  // ok

	// ����֮�ʹ��ڵ����ߣ���С�ڵ�����
	// Calculate the absolute value
	// int abs(int n)
	// double fabs(double x)
	if(a+b<c || fabs(a-b)>c || a+c<b || fabs(a-c)>b || b+c<a || fabs(b-c)>a) {
		printf("���ܹ��������Σ�");
		return;
	}
	s =0.5*(a+b+c);  // ok
	area = sqrt(s*(s-a)*(s-b)*(s-c));
	printf("area = %8.3f\n",area);
}

// ѧϰָ����p33,3.����� (32)
// ���㣺1-1/2+1/3-1/4+...+1/99-1/100
void guide_p33()
{
	int i,n,s=1;   // i:ѭ�������� n�� �������� s��������;
	float sum,k;   // sum����; k: ����ÿһ��

	i=1; sum=0.0;
	printf("��n�����ĺ�,����n\n");
	scanf("%d",&n);
	while(i<=n)
	{ 
		k=(float)s/i;
		//k=s/i;  // ��һ�k=1���ڶ���k=0.5����������������Զ��ضϣ�k=0
		sum=sum+k;
		s=-s;
		i++;
	}

	printf("sum=%f\n",sum);
}

// ѧϰָ��p154, (3)�����б���˵��,(2)i1++i2%i3
void guide_p154()
{
	char c1='a',c2='B',c3='c';
	int i1=10,i2=20,i3=30;
	double d1=0.1,d2=0.2,d3=0.3;
	printf("%d\n",c1+i2*i3/i2%i1); // 97+20*30/20%30=97+30%30=97
	printf("%d\n",i1+++i2%i3);     // 10+20%30=10+20
	printf("%d\n",c1>i1?i1:c2);    // ��һ����i1++,��ˣ�11
	printf("%d\n",!i1&&i2--);      // 0
	printf("%d\n",i2--);           // 20����һ���У�i2--��δִ�У���Ϊa&&b��aΪ0ʱ��������b;������0Ϊ0
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