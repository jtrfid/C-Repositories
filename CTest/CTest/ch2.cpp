#include "stdafx.h"
#include <stdio.h>

void ch2_1()
{
	int a,b;
	a=3*5,a*4; // ==》(a=3*5),a*4;
	b=(3*5,a*4);
	printf("a=%d,b=%d\n",a,b); //a=15,b=60
}

void ch2_2()
{
	int a=2,b=2,c=2;
	printf("ab=%d,abc=%d\n",a/b,a/b&&c); //ab=1,abc=1
}

void ch2_3()
{
	float a=33333.333333;
	double b=3333.33333333333333;
	printf("test31:a=%f,b=%f,%f\n",a,b,1.0/3*3); // test31:a=33333.332031,b=3333.333333,1.000000
}

void ch2_4()
{		
	printf("ch2_4()=========\n");
	char i1=8,i2=-8; // 08[0000 1000], [-8]补=[-8]反+1=[1111 0111]+1=F8
	printf("test3=%d,0x%x,%d,0x%x\n",i1,i1,i2,i2);  // test3=0x8,0xfffffff8
	printf("%d,0x%x,%d,0x%x\n",i1<<1,i1<<1,i2<<1,i2<<1); // 16,0x10,-16,0xfffffff0
	printf("%d,0x%x,%d,0x%x\n",i1<<2,i1<<2,i2<<2,i2<<2); // 32,0x20,-32,0xffffffe0


#if 1
	// 左移，变量中的每一位向左移动，移出的最高位将丢失（溢出），而右端补入0。	
	// 对于无符号数，左移一位相当于乘2，左移两位相当于乘4.
	char a=0x80,b=0x40; // 1000 0000,  0100 0000
	unsigned char u1=0x80,u2=0x40;
	a<<=1;b<<=1;u1<<=1;u2<<=1;
	printf("a,b,u1,u2=0x%x,%x,%x,%x\n",a,b,u1,u2); //a,b,u1,u2=0x0,ffffff80,0,80
	printf("a,b,u1,u2=%d,%d,%d,%d\n",a,b,u1,u2);   //a,b,u1,u2=0,-128,0,128
	a<<=1;b<<=1;u1<<=1;u2<<=1;
	printf("a,b,u1,u2=0x%x,%x,%x,%x\n",a,b,u1,u2); //a,b,u1,u2=0x0,0,0,0
	printf("a,b,u1,u2=%d,%d,%d,%d\n",a,b,u1,u2);   //a,b,u1,u2=0,0,0,0
	a=0x81,b=0x41; // 1000 0001,  0100 0001
	u1=0x81,u2=0x41;
	a<<=1;b<<=1;u1<<=1;u2<<=1;
	printf("a,b,u1,u2=0x%x,%x,%x,%x\n",a,b,u1,u2); //a,b,u1,u2=0x2,ffffff82,2,82
	printf("a,b,u1,u2=%d,%d,%d,%d\n",a,b,u1,u2);   //a,b,u1,u2=2,-126,2,130
	a<<=1;b<<=1;u1<<=1;u2<<=1;
	printf("a,b,u1,u2=0x%x,%x,%x,%x\n",a,b,u1,u2); //a,b,u1,u2=0x4,4,4,4
	printf("a,b,u1,u2=%d,%d,%d,%d\n",a,b,u1,u2);   //a,b,u1,u2=4,4,4,4

	printf("RRRRRRRRR\n");
	// 右移
	a=0x80,b=0x40; // 1000 0000,  0100 0000
	u1=0x80,u2=0x40;
	printf("a,b,u1,u2=0x%x,%x,%x,%x\n",a,b,u1,u2); //a,b,u1,u2=0xffffff80,40,80,40
	a>>=1;b>>=1;u1>>=1;u2>>=1;
	printf("a,b,u1,u2=0x%x,%x,%x,%x\n",a,b,u1,u2); //a,b,u1,u2=0xffffffc0,20,40,20
	printf("a,b,u1,u2=%d,%d,%d,%d\n",a,b,u1,u2);   //a,b,u1,u2=-64,32,64,32
#endif


	a=0xC0; //1100 0000
	printf("a=%d,%x\n",a,a); // a=-64,ffffffc0
	printf("<<1 a=%d,%x\n",a<<1,a<<1); // <<1 a=-128,ffffff80
}

void ch2_5()
{
	printf("ch2_5()=========\n");
	int a; unsigned u1,u2;
	a=1;
	u1=8; u2=8u;
	printf("a=%d,u1=%d,u1=%u,u2=%d,u2=%u\n",a,u1,u1,u2,u2);
	printf("a=%x,u1=%x,u2=%x\n",a,u1,u2);
	printf("a-u1=%d,a-u1=%u,a-u1=%x\n",a-u1,a-u1,a-u1);
	printf("a-u2=%d,a-u2=%u,a-u2=%x\n",a-u2,a-u2,a-u2);
	a=65536;
	printf("a=%x,%d\n",a,a);
	u1=a-u1;
	// int=4,long=4,float=4,double=8,char=1,short=2
	printf("int=%d,long=%d,float=%d,double=%d,char=%d,short=%d\n",sizeof(int),sizeof(long),sizeof(float),sizeof(double),sizeof(char),sizeof(short));  // 4byte
	printf("u1=%d,%u,%x,s=%d\n",u1,u1,u1);
}

void ch2_6()
{
	printf("ch2_6()==========\n");
	unsigned int a=0;
	printf("%x,%u,%d\n",~a,~a,~a);
}

void ch2_7()
{
	printf("ch2_7()==========\n");
	// int=4,long=4,float=4,double=8,char=1,short=2
	printf("bytes:int=%d,long=%d,float=%d,double=%d,char=%d,short=%d\n",sizeof(int),sizeof(long),sizeof(float),sizeof(double),sizeof(char),sizeof(short));  // 4byte
#if 0
	// 因是4个字节组成，不会溢出
	unsigned int a = 32768;  
	int b;
	b=a;
	printf("a=%d,b=%d\n",a,b);     // a=32768,b=32768
	printf("a=0x%x,b=0x%x\n",a,b); // a=0x8000,b=0x8000
#endif
#if 1
	unsigned short int a = 32768; // 0x8000
	short int b;
	b=a;
	printf("a=%d,b=%d\n",a,b);     // a=32768,b=-32768
	printf("a=0x%x,b=0x%x\n",a,b); // a=0x8000,b=0xffff8000(补码表示，溢出，以最小数表示-0)
	unsigned char u = 128; // 0x80
	char c;
	c=u;
	printf("u=%d,c=%d\n",u,c);     // u=128,c=-128
	printf("u=0x%x,c=0x%x\n",u,c); // u=0x80,c=0xffffff80(补码表示，溢出，以最小数表示-0)
	u = 0xFF;
	c=u;
	printf("u=%d,c=%d\n",u,c);     // u=255,c=-1
	printf("u=0x%x,c=0x%x\n",u,c); // u=0xff,c=0xffffffff(补码表示,负数的源码=补码的反码+1)
#endif
}

/*****
P.55 3:   假设m是一个已知3位数，从左到右用a,b,c表示各
位数字，则由数abc如何求数bac，写出表达式
******/
void ch2_8()
{
	printf("ch2_8()======\n");
	printf("please input 3bit number abc,eg. 123\n");
	int a,b,c,m,bac;
	while (1) 
	{
		scanf("%d",&m);
		if (m==0) break;
		a=m/100;
		b=(m-a*100)/10;
		//b=(m/10)%10;   // ok
		//b=(m%100)/10;  // ok
		//b=m/10%10;     // ok,"/"和"%"运算符优先级同（3），结合方向：自左向右，因此可以不加括号
		//b=m%100/10;    // ok,"/"和"%"运算符优先级同（3），结合方向：自左向右，因此可以不加括号
		c= m%10;
		printf("bac=%d%d%d\n",b,a,c);
		bac=b*100+a*10+c;
		printf("bac=%d\n",bac);
	}
}

void floatTest()
{
	printf("floatTest()====\n");
	float a,b;
	a=123456.789E5; // warning C4305: “=”: 从“double”到“float”截断
	b=a+20;
	printf("a=%f,b=%f\n",a,b); // a=12345678848.000000,b=12345678848.000000

	float c1=12345678.9, c2=1234567.89, c3=123456789.123; //  warning C4305: “初始化”: 从“double”到“float”截断
	printf("c=%f,%f,%f\n",c1,c2,c3); // c=12345679.000000,1234567.875000,123456792.000000

	float d=3.4E30; // warning C4305: “初始化”: 从“double”到“float”截断
	printf("d=%f,%e\n",d,d); // d=3399999900045657700000000000000.000000,3.400000e+030

	// 有效位7,保证7为数字（小数点前后加起来）是正确的，超过7位，为无效数字。
	float f1=123.456789, f2=1.23456789,f3=12.345678,f4=123456.7; // warning C4305: “初始化”: 从“double”到“float”截断
	printf("f=%f,%f,%f,%f\n",f1,f2,f3,f4); // f=123.456787,1.234568,12.345678,123456.703125

	float f11=123.4567, f21=1.234567,f31=12.34567,f41=123456.7; // warning C4305: “初始化”: 从“double”到“float”截断
	printf("f=%f,%f,%f,%f\n",f11,f21,f31,f41); // f=123.456703,1.234567,12.345670,123456.703125


	float f12=1234567000, f22=0.001234567,f32=123456000,f42=0.00123456; // warning C4305: “初始化”: 从“double”到“float”截断
	printf("f=%f,%f,%f,%f\n",f12,f22,f32,f42); // f=1234567040.000000,0.001235,123456000.000000,0.001235
}


void doubleTest()
{
	printf("doubleTest()======\n");
	double a,b;
	a=123456.789E5; 
	b=a+20;
	printf("a=%f,b=%f\n",a,b); // a=12345678900.000000,b=12345678920.000000
	printf("a=%e,b=%e\n",a,b); // a=1.234568e+010,b=1.234568e+010
}

void testprintf()
{
	printf("=========testprintf()\n");

	// #常与％o和％x连用，构成%#o和％#x
	// 用于将数据以标准八进制(0零ddd)或十六进制(0x)格式输出
	printf("%o,%#o,%x,%#x,%d\n",123,123,123,123,123);  // 173,0173,7b,0x7b,123

	// *可以改变输出语句的输出宽度
	int i;
	for(i=1;i<=5;i++){ printf("@%*d\n",i,i);}

    // ％f:输出全部整数,并保留6位小数,有效位是前7位
	// %e:保留一位非零整数,保留5位小数,指数保留两位
	printf("%f,%e\n",0.0098765432,0.0098765432);       //  0.009877,9.876543e-003
	printf("%f,%e\n",12.0098765432,12.0098765432);     // 12.009877,1.200988e+001

	//printf("\tab\rcd\n\'ef\\gh\x"); //error C2153: 十六进制常量必须至少有一个十六进制数字
	printf("\tab\rcd\n\'ef\\ghx");
	printf("\nmod=%d,%d,%d\n",-7%4,7%-4,-7%-4);
	printf("%d\n",4&&-5); // 1
	printf("%d\n",8>4-!0); // 1
	printf("%d\n",(8>4)-!0); // 0

	printf("%lf,%lf,%lf\n",(22/5),(float)22/5,(float)(22/5)); //-0.000000,0.000000,0.000000
	printf("%f,%f,%f\n",(22/5),(float)22/5,(float)(22/5)); //-0.000000,0.000000,0.000000
	printf("%f,%f\n",(float)22/5,(float)(22/5)); // 4.400000,4.000000

	printf("%d\n",12==12.0);// 1,12自动转换为实数12.0

	printf("%d\n",-1234567);
	printf("%ld\n",-1234567);
	short int a= -1234567;
	printf("%d\n",a);  //10617
	printf("%ld\n",a); //10617

}

void testAdd1()
{
	printf("=========testAdd1()\n");
	int i=3,j=3;
	// 先使用，再自增
	printf("i=%d,i++=%d\n",i,i++); // i=3,i++=3
	printf("j++=%d,j=%d\n",j++,j); // j++=3,j=3 
	
	// 先自增，再使用
	i=3; j=3;
	printf("i=%d,++i=%d\n",i,++i); // i=4,++i=4
	printf("++j=%d,j=%d\n",++j,j); // ++j=4,j=4

	// 先自增两次，再使用
	i=3; j=3;
	printf("i=%d,++i=%d,++i=%d\n",i,++i,++i); // i=5,++i=5,++i=5
	printf("++j=%d,++j=%d,j=%d\n",++j,++j,j); // ++j=5,++j=5,j=5
}

void testAdd2()
{
	printf("=========testAdd2()\n");
	int i=5,j=5,p,q;
	// 理解为先使用i，即三个i相加(故P值为15),然后i再自增1三次,相当于加3,故i的最后值为8
	p=(i++)+(i++)+(i++);
	// 应理解为j先自增1，再参与运算，故q=18，j的最后值仍8
	q=(++j)+(j++)+(j++);
	printf("i,j,p,q=%d,%d,%d,%d\n",i,j,p,q); //i,j,p,q=8,8,15,18
	i=5;
	p=(++i)+(++i)+(++i);
	// 应理解为i先自增1，共3次，即相当于i=i+3=8; 然后3个8相加=p
	printf("i,p=%d,%d\n",i,p); // i,p=8,24
}

void testAdd3()
{
	printf("=========testAdd3()\n");
	int x;
	x=4;
	if (x++>5) printf("%d\n",x);
	else printf("%d\n",x--);  // 5

	printf("=========\n");
	x=6;
	if (x++>5) printf("%d\n",x); // 7
	else printf("%d\n",x--);  

	printf("=========\n");
	int a,b;
	int d1,d2,d3,d4;
	d1 =(a=3,a+3);
	d2 = (b=1,b+1);
	d3 =(a=3,a+3,a++); 
	d4 = (b=1,b+1,b+2);
	printf("d=%d,%d,%d,%d\n",d1,d2,d3,d4);	//6,2,3,3
}

// &&的有0为0和||的有1为1比运算符优先级更优先。例如本例的（b=10）虽然优先级更高，但是也没有该规则高。
// 逻辑运算符&&与||有一些较为特殊的属性，由&&与||连接的表达式按从左到右的顺序进行行求值，并且，在知道结果值为真或假后立即停止计算。
void test_and_or()
{
	printf("==========test_and_or()\n");
	int a=1,b=8,c=9,d;
	d=(a||(b=10)||(c=2));
	printf("a=%d,b=%d,c=%d,d=%d\n",a,b,c,d);
	a=0;
	d=(a||(b=10)||(c=2));
	printf("a=%d,b=%d,c=%d,d=%d\n",a,b,c,d);

	int i1=0,i2=5;
	printf("i1,i2,%d\n",!i1&&i2--);  // 1    
	printf("i2=%d\n",i2); // 4
	i1=1;
	printf("i1,i2,%d\n",!i1&&i2--); // 0     
	printf("i2=%d\n",i2);   // 4   
}

void ch2()
{
	printf("==========ch2()\n");
	ch2_1();
	ch2_2();
	ch2_3();
	ch2_4();
	ch2_5();
	ch2_6();
	ch2_7();
	ch2_8();
	floatTest();
	doubleTest();
	testAdd1();
	testAdd2();
	testAdd3();
	testprintf();
	test_and_or();
	getchar();
}