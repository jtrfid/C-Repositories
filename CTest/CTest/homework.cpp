#include "stdafx.h"
#include <stdio.h>
#include <string.h>

/********************************************************
* P126,第6题                                            *
* 函数功能，将字符串s逆置后输出                            *
* 例如,func1("ab12"),打印输出21ba                         *
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
* P156,第9题                                            *
* 函数功能，根据整型参数m的值，计算公式：              *
* t=1-1/2*2-1/3*3-…-1/m*m                                 *
* 的值。                                                *
* 例如,func2(5),返回0.536389                              *
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
* P192,第3题                                            *
* 函数功能，找出字符串s中的大写字母、小写字母、空格、数字以及其它字符各有多少。
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

// 来自1404061_14040610004_曾晶晶,ok
void m1()
{
	int upper=0,lower=0,digit=0,space=0,other=0,i=0;
	char *p,s[20];
	printf("input string: ");
	while ((s[i]=getchar())!='\n') i++; // s最后一个字符是\n
	p=&s[0]; // p=s;亦可
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

//来自1404061_14040610015-陈振,ok
float Result(float a){
	float Sum = 1;  //定义总和变量并初始化
	for(float i=2;i<=a;i++){
		Sum = Sum - (1/(i*i));  
	}
	return Sum;  //返回结果值
}

//来自1404061_14040610023_王子健
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
	// func1("1234"); // 不能使用这种方式，因为在函数内部要修改形参的内容，这样调用相当于实参是一个字符串常量，不能修改。
	printf("%s\n",s);
	printf("%lf\n",func2(5));
	func3("ABCa12cD 123");

	//m1();
	//printf("sum=%f",Result(5.0));
	printf("sum=%f",f(5));
}