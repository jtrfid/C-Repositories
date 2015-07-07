#include "stdafx.h"
#include <stdio.h>

/*******************************
 * 复习
 *******************************/

/********
 不在下面测试程序中的注意事项：
 1. 位运算|、&、^，<<,>>运算
 2. 局部变量屏蔽全局变量
 3. 复合赋值运算优先级（14），低于算术运算符（3,4），x *= x+1  --> x=x*(x+1)
 4. 静态局部变量具有局部可见性和继承性，静态变量的生存期贯穿于整个程序的运行期间，未初始化的static变量是0;
 5. 变量的作用域，局部变量，形式参数是局部变量，复合语句中定义的变量作用域是本复合语句{}
 6. 全局变量的作用域一定比局部变量的作用域范围大--》是错误的，例如全局变量定义在文件末尾，而没用extern语句扩展，则该全局变量的作用域为0
 7. 值传递与地址传递
 8. while(非0){}则执行循环体，条件i%2,即表示i不能被2整除时，执行循环体
 9. 冒泡排序的循环条件
 *****************/
void test_1()
{
	printf("===========test_1(),交换x和y中的值.\n");
	int x,y;
	x=10;y=20;
	x+=y; y=x-y; x-=y;
	printf("x=%d,y=%d\n",x,y);
}

void test_1_1()
{
	printf("===========test_1_1(),异或(优先级:9),【低于】,移位(优先级：5)，结合性：自左向右.\n");
	int a=3,b=6,c;
	c=a^b<<2;
	printf("c=%0x\n",c);//c=1b,即，00011011
}

void test_2()
{
	printf("========test_2(),(--y)表达式的值是变化后的y，即(y=y-1,y); (y--)表达式的值是未变化的y值，即表达式的值=y,然后y=y-1\n");
	int y=10;
	do {
		y--;
		printf("y=%d\n",y);
	} while(--y);  // (--y)表达式的值是变化后的y，即(y=y-1,y)
	printf("%d\n",y--); // 0
	printf("y=%d\n",y); // -1

	printf("=========\n");
	int x=3;
	do {
		printf("%3d",x-=2);
	} while(!(--x));
	printf("\n");  // 1  -2
}

void test_2_2()
{
	printf("========test_2_2(),(--y)表达式的值是变化后的y，即(y=y-1,y); (y--)表达式的值是未变化的y值，即表达式的值=y,然后y=y-1\n");
	int x=0,y=5,z=3;
	while(z-->0&&++x<5) y=y-1;
	printf("%d,%d,%d\n",x,y,z); // 3,2,-1
}

void test_2_3()
{ 
	printf("========test_2_3(),(--y)表达式的值是变化后的y，即(y=y-1,y); (y--)表达式的值是未变化的y值，即表达式的值=y,然后y=y-1\n");
	int p[8]={11,12,13,14,15,16,17,18},i=0,j=0;
	while(i++<7) 
	{
		printf("%d,",i); // 1,2,3,4,5,6,7
		if(p[i]%2) j+=p[i];  // 13+15+17
	}
	printf("\n%d\n",j); // 45
}


void test_3()
{
	printf("========test_3(),||,有1为1；&&，有0为0；停止后续运算\n");
	int a,b,c;
	a=b=c=1;
	++a||++b&&++c;
	printf("b=%d\n",b); // 1

	printf("%d\n",(a=2)&&(b=-2)); // 1

	int x,a1,b1,c1;
	x=(a1=4,b1=8,c1=12);
	printf("x=%d\n",x);
}

void test_3_2()
{
	printf("=========test_3_2(),||,有1为1；&&，有0为0；停止后续运算\n");
	int a=1,b=2,c=3,d=4,m=2,n=2;
	int k;
	k=(m=a>b)&&(n=c>d);
	printf("m=%d,n=%d,k=%d\n",m,n,k); // 0,2,0
}

void test_4()
{
	printf("==========test_4(),整数相除，结果为去掉小数的整数\n");
	int a=7; float x=2.5,y=4.7;
	printf("%f\n",x+a%3*(int)(x+y)%2/4);  // 2.5

	{
		int a=9,b=2;
		float x=6.6,y=1.1,z;
		z=a/2+b*x/y+1/2;
		printf("z=%5.2f\n",z); // 16.00
	}
}

void test_5()
{
	printf("===========test_5(),switch case 无break语句，继续执行下面的case;\n");
	int i;
	for (i=0;i<3;i++)
		switch(i){
		case 1: printf("%d",i);
		case 2: printf("%d",i);
		default: printf("%d",i);
	}
	printf("\n");  // 011122

	int x=1,a=0,b=0;
	switch(x)
	{
	case 0: b++;
	case 1: a++;
	case 2: a++; b++;
	}
	printf("a=%d,b=%d\n",a,b);  // 2,1
}

void test_6()
{
	printf("=======test_6(),循环语句的break和continue语句\n");
	int x,y;
	for(x=1,y=1;y<50;y++){
		if(x>=10) break;
		if(x%2==1) { x += 5; continue;}
		x -= 3;
	}
	printf("x=%d\n",x);  // 10
}

int test_7(int x)
{
	printf("=======test_7()，递归调用f(x)=x-f(x-2),xx=%d\n",x);
	int p;
	if (x==0 || x==1) return 3;
	else p=x-test_7(x-2);

	printf("x=%d,p=%d\n",x,p);
	return p;
}

void test_8()
{  
	printf("============test_8(),for循环\n");
	int k=0,i,s[]={1,-9,7,2,-10,3};
	for (i=0;i<6;i++)
		if (s[i]>s[k]) 
			k=i;
	printf("\n%d\n",k); // 2

	printf("=======，for语句执行顺序，初始化-->判断循环条件-->执行循环体-->循环变量(第二分号后的语句)-->判断循环条件-->...\n");
	int ii,jj;
	for(ii=0,jj=1;ii<=jj+1;ii+=2,jj--)  
		printf("%d\n",ii); // 0，仅执行一次
}

void test_9()
{ 
	printf("=======test_9()结构数组的初始化\n");
	struct cmplx{ 
		int x; int y; 
	}cnum[2]={1,3,2,7};
	printf("%d\n",cnum[0].y /cnum[0].x * cnum[1].x); // 6
}

void test_10()
{
	printf("========test_10(),数组下标与指针，a[10],p=a;a[i]=p[i]=*(p+i);p=a+i-->a[i]; p=&a[2]\n");
	char a[10]={'1','2','3','4','5','6','7','8','9',0},*p;
	int i;
	i=8;
	p=a+i;
	printf ("%s\n",p-3); // 6789
}

static int e_i;
static int e_c;
static float e_f;
void test_11()
{
	printf("===========test_11(),未初始化的静态变量和全局变量值：整数：0，浮点数：0.0，字符型，空字符(\'\\0\')\n");
	static int i;
	static int c='\0';
	static float f;
	printf("空字符=%c,%d\n",'\0','\0');  // 不知为何，vc++打印空字符是'a'
	printf("i,c,f=%d,%c,%f,%d\n",i,c,f,c);// 0,a,0.0,0
	printf("e_i,e_c,e_f=%d,%c,%f,%d\n",e_i,e_c,e_f,e_c); // 0,a,0.0,0
}

void test_12()
{
	printf("============test_12()\n,联合（公用体）的每个成员起始地址都相同\n");
	union dt {int a;char b;double c;} data;
	data.a=5;
	printf("%d,%lf\n",data.a,data.c); // 5, 0.000
}

void test_13()
{
	printf("=============test_13()带参数的宏定义\n");
#define f(x) x*x
	int i;
	i=f(4+4)/f(2+2);  // 4+4*4+4/2+2*2+2
	printf("%d\n",i); // 28
}

void test_14()
{
	printf("==========test_14(),do {} while()\n");
	int i=0,s=0;
	do
	{  
		if(i%2){i++;continue;}
		i++;
		s+=i;
	}while(i<7);
	printf(" %d\n ",s); // 16
}

void test_15() 
{	
	printf("==========test_15()，逆序输出\n");
	int n1,n2; 
	//scanf("%d",&n2);
	n2=1298;
	while(n2!=0) 
	{ 
		n1=n2%10; 
		n2=n2/10; 
		printf("%d",n1); // 8921
    } 
} 

void test_16()
{
	printf("=========test_16()\n");
	printf("输入整型数组，最大的与第一个元素交换，最小的与最后一个元素交换，输出数组。数组长度为10.\n");

	int i;
	//int a[10]={0,1,2,3,4,5,6,7,8,9};
	//int a[10]={9,1,2,3,4,5,6,7,8,0};
	//int a[10]={10,1,2,12,4,5,6,7,8,9};
	int a[10]={10,1,2,12,12,5,6,7,8,9};
	int max,min,m1,m2; // 最大最小元素及其下标
	int tmp;

	//for (i=0;i<10;i++) scanf("%d",a[i]);

	printf("原数组:\n");
	for (i=0;i<10;i++) printf((i==9)?"%d\n":"%d,",a[i]);

	min=a[0];max=a[0];
	m1=m2=0;
	for (i=0;i<10;i++) {
		if(a[i]>max) { max=a[i]; m1=i; }
		if(a[i]<min) { min=a[i]; m2=i; }
	}

	printf("最大最小元素及其下标: max,min=%d,%d,m1,m2=%d,%d\n",max,min,m1,m2);
	
	// 最大元素与数组的第一个元素交换
	tmp=a[0]; a[0]=max; a[m1]=tmp;
	// 如果最大最小元素正好在数组的两头，则不进行第二次交换，仅有上一句交换即可
	if (!((m1==0&&m2==9) || (m1==9&&m2==0))) {
		printf("最小元素与数组的最后一个元素交换\n");
	    tmp=a[9]; a[9]=min; a[m2]=tmp;
	}

	printf("交换后数组:\n");
	for (i=0;i<10;i++) printf((i==9)?"%d\n":"%d,",a[i]);
}

void test_16_1()
{
	printf("=========test_16_1()\n");
	printf("输入整型数组，最大的与第一个元素交换，最小的与最后一个元素交换，输出数组。数组长度为10.\n");

	int i;
	//int a[10]={0,1,2,3,4,5,6,7,8,9};
	//int a[10]={9,1,2,3,4,5,6,7,8,0};
	//int a[10]={10,1,2,12,4,5,6,7,8,9};
	int a[10]={10,1,2,12,12,5,6,7,8,9};
	int tmp;

	//for (i=0;i<10;i++) scanf("%d",a[i]);

	printf("原数组:\n");
	for (i=0;i<10;i++) printf((i==9)?"%d\n":"%d,",a[i]);

	// a[0]最大
	for (i=0;i<10;i++) {
		if(a[0]<a[i]) { tmp=a[0]; a[0]=a[i]; a[i]=tmp; }
	}

	// a[9]最小
	for (i=0;i<10;i++) {
		if(a[9]>a[i]) { tmp=a[9]; a[9]=a[i]; a[i]=tmp; }
	}

	printf("交换后数组:\n");
	for (i=0;i<10;i++) printf((i==9)?"%d\n":"%d,",a[i]);
}

//void test_17(char ss[],char c) // 等效于下一句
void test_17(char *ss,char c)
{
	printf("=========test_17()等效test_18()\n");
	printf("求出ss所指字符串中指定字符的个数，并返回此值。例如，若输入字符串123412132，输入字符1，则输出3。\n");
    int i,n;
	for (i=0,n=0;ss[i]!='\0';i++) {
		if (ss[i]==c) 
			n++;
	}
	printf("字符个数：%d\n",n);
}

void test_18(char *ss,char c)
{
	printf("=========test_17()等效test_18()\n");
	printf("求出ss所指字符串中指定字符的个数，并返回此值。例如，若输入字符串123412132，输入字符1，则输出3。\n");
	int n=0;
	//*,++运算符优先级均为2，自右向左
	//*ss++ --> *(ss++),先使用，后自增，即表达式的值是*ss，然后ss++;
	//*++ss --> *(++ss),先自增，后使用，即ss=ss+1,然后表达式的值是*ss
	// '\0'的ASCII码是0,'0'的ascii码是49

	/***************
	do 
	{
		if (*ss==c) n++;
	} while (*++ss);
	****************/

    // 或者
	while (*ss != '\0') // while(*ss)
	{
		if (*ss==c) n++;
		ss++;
	}
	printf("字符个数：%d\n",n);
}

void test_19(int n)
{
	printf("=========test_19()\n");
	printf("计算并输出下列多项式值：Sn = 1 + 1/1! + 1/2! + 1/3! + 1/4! + ... + 1/n! \n");

	int i,j;
	double sum=1.0,fac;

	for (i=1;i<=n;i++)
	{
		fac = 1.0;
		for(j=1;j<=i;j++) fac *= j;
		sum += 1.0/fac;
	}
	printf("n=%d,sum=%lf\n",n,sum);
}

void test_19_1(int n)
{
	printf("=========test_19_1()\n");
	printf("计算并输出下列多项式值：Sn = 1 + 1/(1+2) + 1/(1+2+3) + ... + 1/(1+2+3+...+n) \n");

	int i,j;
	long fac;
	double sum=0.0;

	for (i=1;i<=n;i++)
	{
		fac = 0;
		for(j=0;j<=i;j++) fac += j;
		sum += 1.0/fac;
	}
	printf("n=%d,sum=%lf\n",n,sum);
}

void test_19_2(int n)
{
	printf("=========test_19_2()\n");
	printf("计算并输出下列多项式值：Sn = 1 + 1/(1+2) + 1/(1+2+3) + ... + 1/(1+2+3+...+n) \n");

	int i;
	long fac;
	double sum=0.0;

	fac=0;
	for (i=1;i<=n;i++)
	{
		fac += i;
		sum += 1.0/fac;
	}
	printf("n=%d,sum=%lf\n",n,sum);

}

void test_19_3(int n)
{
	printf("=========test_19_3()\n");
	printf("计算并输出下列多项式值：Sn = 1 + 1/(1+2) + 1/(1+2+3) + ... + 1/(1+2+3+...+n) \n");

	int i;
	double sum=0.0,fac;

	for (i=1;i<=n;i++)
	{
		fac = (1+i)*i/2.0;
		sum += 1.0/fac;
	}
	printf("n=%d,sum=%lf\n",n,sum);

}

int func(int a,int b)
{
	int c;
	c=a+b;
	return c;
}

void test_20()
{
	printf("========test_20(),(x--,y++,x+y)表达式，x,y先使用,即整个表达式的值是未变化的x+y\n");
	int x=6,y=7,z=8,r;
	r=func((x--,y++,x+y),z--);  //func(6+7,8)
	printf("%d\n",r); // 21
}

void test_21()
{
	printf("=========test_21(),指针与数组及++，--\n");
	int a[]={1,2,3,4},y,*p=&a[1];
	y=(*--p)++;
	printf("%d\n",y); // a[0]
	
	int i;
	for(i=0;i<4;i++) printf((i==3)?"%3d\n":"%3d",a[i]); // 2 2 3 4
}

void test_22()
{
	printf("=========test_22(),指针与数组\n");
	int a[]={2,4,6,8,10},y=1,x,*p=&a[1];
	for(x=0;x<3;x++) y+=*(p+x);
	printf("%d\n",y); // 1+a[1]+a[2]+a[3] = 19
}

void test_23()
{
	printf("=========test_23(),contine继续【该层】循环\n");
	int i,n=0;
	for (i=2;i<5;i++)
	{
		do 
		{
			if(i%3) continue;
			n++;
		} while (!i);
		n++;
	}
	printf("n=%d\n",n);
}

void sum(int *a)
{
	a[0]=a[1];
}

void test_24()
{
	printf("============test_24(),指针作函数参数,指针与数组的等效使用\n");
	int a[10]={1,2,3,4,5,6,7,8,9,10},i;
	for(i=2;i>=0;i--) sum(&a[i]);
	for(i=0;i<=4;i++) printf(i==4?"%d\n":"%d,",a[i]); //4,4,4,4,5
}

void test_25(int n)
{
	printf("=====test_25(int n)\n");
	printf("编写一个函数，输入n为偶数时，调用函数求1/2+1/4+...+1/n,当输入n为奇数时，调用函数1/1+1/3+...+1/n;\n");
	int i;
	double sum=0.0;
	if (!(n%2)) {
		for (i=2;i<=n;i+=2) {
			sum += 1.0/i;  // 切记1.0/i，而不是1/i
		}
		printf("偶数n=%d，sum=%lf\n",n,sum);
	}
	else {
		for (i=1;i<=n;i+=2) {
			sum += 1.0/i; // 切记1.0/i，而不是1/i
		}
		printf("奇数n=%d，sum=%lf\n",n,sum);
	}
}

void test()
{
	test_1();
	test_1_1();
	test_2();
	test_2_2();
	test_2_3();
	test_3();
	test_3_2();
	test_4();
	test_5();
	test_6();
	printf("%d\n",test_7(9)); // 7
	test_8();
	test_9();
	test_10();
	test_11();
	test_12();
	test_13();
	test_14();
	test_15();
	test_16();
	test_16_1();
	test_17("123412132",'1');
	test_18("123412132",'1');
	test_19(1);  //2.0
	test_19(2);  //2.5
	test_19(3);  //2.67
	test_19_1(1); //1.0
	test_19_1(2); //1.33
	test_19_1(3); //1.5
	test_19_2(1); //1.0
	test_19_2(2); //1.33
	test_19_2(3); //1.5
	test_19_3(1); //1.0
	test_19_3(2); //1.33
	test_19_3(3); //1.5
	test_20();
	test_21();
	test_22();
	test_23();
	test_24();
	test_25(1);
	test_25(2);
	test_25(3);
	test_25(4);
}