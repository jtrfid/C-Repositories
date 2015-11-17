#include "stdafx.h"
#include <stdio.h>
#include <string.h>

struct data
{   int a, b, c; };

void value_trans()
{   
	printf("===========value_trans() 结构体变量作函数参数，多值传递。\n");
	void value_func(struct data  parm);
	struct data arg;
	arg.a=27;   arg.b=3;    arg.c=arg.a+arg.b;
	printf("arg.a=%d arg.b=%d arg.c=%d\n",arg.a,arg.b,arg.c); // arg.a=27 arg.b=3 arg.c=30
	value_func(arg);
	printf("arg.a=%d arg.b=%d arg.c=%d\n",arg.a,arg.b,arg.c); // arg.a=27 arg.b=3 arg.c=30
}

void value_func(struct data  parm)
{   
	printf("parm.a=%d parm.b=%d parm.c=%d\n",parm.a,parm.b,parm.c);  // parm.a=27 parm.b=3 parm.c=30
	parm.a=18;     parm.b=5;    parm.c=parm.a*parm.b;
	printf("parm.a=%d parm.b=%d parm.c=%d\n",parm.a,parm.b,parm.c);  // parm.a=18 parm.b=5 parm.c=90
}

void address_trans()
{   
	printf("===========address_trans() 指向结构体变量或数组的指针作参数----地址传递。\n");
	void address_func(struct data  *parm);
	struct data arg;
	arg.a=27;   arg.b=3;    arg.c=arg.a+arg.b;
	printf("arg.a=%d arg.b=%d arg.c=%d\n",arg.a,arg.b,arg.c); // arg.a=27 arg.b=3 arg.c=30
	address_func(&arg);
	printf("arg.a=%d arg.b=%d arg.c=%d\n",arg.a,arg.b,arg.c); // arg.a=18 arg.b=5 arg.c=90
}

void address_func(struct data  *parm)
{   
	printf("parm->a=%d parm->b=%d parm->c=%d\n",parm->a,parm->b,parm->c);  // parm->a=27 parm->b=3 parm->c=30
	parm->a=18;     parm->b=5;    parm->c=parm->a*parm->b;
	printf("parm->a=%d parm->b=%d parm->c=%d\n",parm->a,parm->b,parm->c);  // parm->a=18 parm->b=5 parm->c=90
}


///////////////////// 【含字符串】
struct data_str
{   int a, b, c; char str[20];};

void value_trans_str()
{   
	printf("===========value_trans_str() 【含字符串】结构体变量作函数参数，多值传递。结构体中的字符串是地址传递。\n");
	void value_func_str(struct data_str  parm);
	struct data_str arg;
	arg.a=27;   arg.b=3;    arg.c=arg.a+arg.b; strcpy(arg.str,"ABC");
	printf("arg.a=%d arg.b=%d arg.c=%d arg.str=%s\n",arg.a,arg.b,arg.c,arg.str); // arg.a=27 arg.b=3 arg.c=30 arg.str=ABC
	value_func_str(arg);
	printf("arg.a=%d arg.b=%d arg.c=%d arg.str%s\n",arg.a,arg.b,arg.c,arg.str); // arg.a=27 arg.b=3 arg.c=30 arg.strABC
}

void value_func_str(struct data_str  parm)
{   
	printf("parm.a=%d parm.b=%d parm.c=%d parm.str=%s\n",parm.a,parm.b,parm.c,parm.str);  // parm.a=27 parm.b=3 parm.c=30 parm.str=ABC
	parm.a=18;     parm.b=5;    parm.c=parm.a*parm.b; strcpy(parm.str,"funcABC");
	printf("parm.a=%d parm.b=%d parm.c=%d parm.str=%s\n",parm.a,parm.b,parm.c,parm.str);  // parm.a=18 parm.b=5 parm.c=90 parm.str=funcABC
}

void address_trans_str()
{   
	printf("===========address_trans_str() 【含字符串】指向结构体变量或数组的指针作参数----地址传递。\n");
	void address_func_str(struct data_str  *parm);
	struct data_str arg;
	arg.a=27;   arg.b=3;    arg.c=arg.a+arg.b; strcpy(arg.str,"ABC");
	printf("arg.a=%d arg.b=%d arg.c=%d arg.str=%s\n",arg.a,arg.b,arg.c,arg.str); // arg.a=27 arg.b=3 arg.c=30 arg.str=ABC
	address_func_str(&arg);
	printf("arg.a=%d arg.b=%d arg.c=%d arg.str=%s\n",arg.a,arg.b,arg.c,arg.str); // arg.a=18 arg.b=5 arg.c=90 arg.str=funcABC
}

void address_func_str(struct data_str  *parm)
{   
	printf("parm->a=%d parm->b=%d parm->c=%d parm->str=%s\n",parm->a,parm->b,parm->c,parm->str);  // parm->a=27 parm->b=3 parm->c=30 parm->str=ABC
	parm->a=18;     parm->b=5;    parm->c=parm->a*parm->b;  strcpy(parm->str,"funcABC");
	printf("parm->a=%d parm->b=%d parm->c=%d parm->str=%s\n",parm->a,parm->b,parm->c,parm->str);   // parm->a=18 parm->b=5 parm->c=90 parm->str=funcABC
}


void union_test()
{
	printf("===================union_test()\n");
	union
	{
		int i; char ch; float f;
	} a;

	a.i=1;
	a.ch='a';
	a.f=1.5;
	printf("%d\n",a.i); // 1069547520
	printf("%f\n",a.i); // 0.000000
	printf("%f\n",a.f); // 1.500000
}

void union_test1()
{
	printf("===================union_test1()\n");
	union
	{
		int i; char ch[2];
	} x;

	x.i=24897;
	printf("i=%o\n");
	printf("ch0=%o,ch1=%o\n",x.ch[0],x.ch[1]);
}


void ch9()
{
	printf("=========ch9()\n");
	value_trans();
	address_trans();
	value_trans_str();
	address_trans_str();
	union_test();
	union_test1();
}
