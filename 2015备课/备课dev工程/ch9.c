//#include "stdafx.h"
#include <stdio.h>
#include <string.h>

struct data
{   int a, b, c; };

void value_trans()
{   
	printf("===========value_trans() �ṹ�������������������ֵ���ݡ�\n");
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
	printf("===========address_trans() ָ��ṹ������������ָ��������----��ַ���ݡ�\n");
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


///////////////////// �����ַ�����
struct data_str
{   int a, b, c; char str[20];};

void value_trans_str()
{   
	printf("===========value_trans_str() �����ַ������ṹ�������������������ֵ���ݡ��ṹ���е��ַ����ǵ�ַ���ݡ�\n");
	void value_func_str(struct data_str  parm);
	struct data_str arg;
	arg.a=27;   arg.b=3;    arg.c=arg.a+arg.b; strcpy(arg.str,"ABC");
	printf("arg.a=%d arg.b=%d arg.c=%d arg.str=%s\n",arg.a,arg.b,arg.c,arg.str); // arg.a=27 arg.b=3 arg.c=30 arg.str=ABC
	value_func_str(arg);
	printf("arg.a=%d arg.b=%d arg.c=%d arg.str=%s\n",arg.a,arg.b,arg.c,arg.str); // arg.a=27 arg.b=3 arg.c=30 arg.str=ABC
}

void value_func_str(struct data_str  parm)
{   
	printf("parm.a=%d parm.b=%d parm.c=%d parm.str=%s\n",parm.a,parm.b,parm.c,parm.str);  // parm.a=27 parm.b=3 parm.c=30 parm.str=ABC
	parm.a=18;     parm.b=5;    parm.c=parm.a*parm.b; strcpy(parm.str,"funcABC");
	printf("parm.a=%d parm.b=%d parm.c=%d parm.str=%s\n",parm.a,parm.b,parm.c,parm.str);  // parm.a=18 parm.b=5 parm.c=90 parm.str=funcABC
}

void address_trans_str()
{   
	printf("===========address_trans_str() �����ַ�����ָ��ṹ������������ָ��������----��ַ���ݡ�\n");
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
	/***********
	// ʹ���޷�������׼ȷ 
	union
	{
		unsigned int i; unsigned char ch[2];
	} x;
	********/

	union
	{
		int i; char ch[2];
	} x;

    printf("int size=%d\n",sizeof(int)); // 4
	x.i=24897;
	printf("i=%d\n",x.i);
	printf("ch0=%d,ch1=%d\n",x.ch[0],x.ch[1]);
	printf("i=%#x\n",x.i);
	printf("ch0=%#x,ch1=%#x\n",x.ch[0],x.ch[1]);
	/***
	 �����
     x.i=24897;
     i=0x6141  �����ֽ�  ch0 ch1
     ch0=0x41,ch1=0x61�����ֽ���ǰ�����ֽ��ں�
     ***/ 
}

void union_test2()
{
	printf("===================union_test2()\n");
	union
	{
		int i; char ch[2];
	} x;

    printf("int size=%d\n",sizeof(int)); // 4
	x.i=248;
	printf("i=%d\n",x.i);
	printf("ch0=%d,ch1=%d\n",x.ch[0],x.ch[1]);
	printf("i=%#x\n",x.i);
	printf("ch0=%#x,ch1=%#x\n",x.ch[0],x.ch[1]);
	/***
	 x.i=248;  
     i=0xf8 һ���ֽڣ���ch[0]���룬��Ϊ��������������λ(�����ң����ĸ��ֽ����λ��0)
     ch0=0xfffffff8,ch1=0��һ���ֽڴ���ch[0]�����ڴ�ӡchar���͵�ԭ�򣬽�ԭ����f8��ӡ��fffffff8
	 ***/
}

void union_test3()
{
	printf("===================union_test3()\n");
	union
	{
		int i; unsigned char ch[4];
	} x;

    printf("int size=%d\n",sizeof(int)); // 4
	x.i=-248;
	printf("i=%d\n",x.i);
	printf("ch0=%d,ch1=%d\n",x.ch[0],x.ch[1]);
	printf("i=%#x\n",x.i);
	printf("ch0=%#x,ch1=%#x,ch2=%#x,ch3=%#x\n",x.ch[0],x.ch[1],x.ch[2],x.ch[3]);
	/***
	 i=-248  
     ch0=8,ch1=255
     i=0xffffff08  // -248�Ĳ����ʾ 
     ch0=0x8,ch1=0xff,ch2=0xff,ch3=0xff
	 ***/
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
	union_test2();
	union_test3(); 
}
