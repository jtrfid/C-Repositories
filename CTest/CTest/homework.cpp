#include "stdafx.h"
#include <stdio.h>
#include <string.h>

/********************************************************
* P126,��6��                                            *
* �������ܣ����ַ���s���ú����                            *
* ����,func1("ab12"),��ӡ���21ba                         *
*********************************************************/
void func1(char *s)
{
	int i,len;
	char tmp;
	len=strlen(s);
	for(i=0;i<len/2;i++)
	{
		tmp=s[i];
		s[i]=s[len-1-i];
		s[len-1-i]=tmp;
	}
}

/********************************************************
* P156,��9��                                            *
* �������ܣ��������Ͳ���m��ֵ�����㹫ʽ��              *
* t=1-1/2*2-1/3*3-��-1/m*m                                 *
* ��ֵ��                                                *
* ����,func2(5),����0.536389                              *
*********************************************************/
double func2(int m)
{
	int i;
	double sum=1.0;
	for(i=2;i<=m;i++) 
	{
		sum -= 1.0/(i*i);
	}
	return sum;
}

/********************************************************
* P192,��3��                                            *
* �������ܣ��ҳ��ַ���s�еĴ�д��ĸ��Сд��ĸ���ո������Լ������ַ����ж��١�
*                                                        *
*********************************************************/
void func3(char *s)
{
	int A=0,a=0,space=0,num=0,other=0;

	while(*s) {
		if (*s==' ') space++;
		else if((*s>='A') && (*s<='Z')) A++;
		else if((*s>='a') && (*s<='z')) a++;
		else if((*s>='0') && (*s<='9')) num++;
		else other++;
		s++;
	}

	printf("A=%d,a=%d,space=%d,num=%d,other=%d\n",A,a,space,num,other);
}

// ����1404061_14040610004_������,ok
void m1()
{
	int upper=0,lower=0,digit=0,space=0,other=0,i=0;
	char *p,s[20];
	printf("input string: ");
	while ((s[i]=getchar())!='\n') i++; // s���һ���ַ���\n
	p=&s[0]; // p=s;���
	while (*p!='\n')
	{if(('A'<=*p)&&(*p<='Z'))
	++upper;
	else if(('a'<=*p)&&(*p<='z'))
		++lower;
	else if(*p==' ')
		++space;
	else if((*p<='9')&&(*p>='0'))
		++digit;
	else
		++other;
	p++;
	}
	printf("upper case:%d   lower case:%d",upper,lower);
	printf("     space:%d   digit:%d       other:%d\n",space,digit,other);

}

//����1404061_14040610015-����,ok
float Result(float a){
	float Sum = 1;  //�����ܺͱ�������ʼ��
	for(float i=2;i<=a;i++){
		Sum = Sum - (1/(i*i));  
	}
	return Sum;  //���ؽ��ֵ
}

//����1404061_14040610023_���ӽ�
float f(int m)
{
	if(m==1)
		return 1;
	//else if(m==2)
	//	return -1/(float)(m*m)+f(m-1);
	else
		return -1/(float)(m*m)+f(m-1);
}

void homework()
{
	char s[]="1234";
	func1(s);
	// func1("1234"); // ����ʹ�����ַ�ʽ����Ϊ�ں����ڲ�Ҫ�޸��βε����ݣ����������൱��ʵ����һ���ַ��������������޸ġ�
	printf("%s\n",s);
	printf("%lf\n",func2(5));
	func3("ABCa12cD 123");

	//m1();
	//printf("sum=%f",Result(5.0));
	printf("sum=%f",f(5));
}