//#include <stdafx.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>

void for_getchar()
{
     printf("====for_getchar()\n");
     int i; char c;
     
     scanf("%c",&c); // 输入回车 
     printf("c=%d",c); // 10 换行符 
     
     c=getchar(); // 输入回车 
     printf("c=%d",c); // 10 换行符 
     
     c=getch();  // 输入回车 
     printf("c=%d",c); // 13 回车符 
     
     
     printf("getchar()遇到回车换行开始接收这一行的第一个字符\n");
     for (i=0;(c=getchar())!= '\n'; i++)
       printf("i=%d,c=%c,%d\n",i,c,c);
     
     printf("getche()即可接收字符\n");  
     for (i=0;(c=getche())!= '\r'; i++)
       printf("i=%d,c=%c,%d\n",i,c,c);
}

void password()
{
     printf("====password()\n");
     int i; char c; 
     char password[7];  // 6位密码，下标从0开始，password[6]='\0' 
     for (i=0;((c=getch())!='\r') && i<=5; i++) 
       { putchar('*'); password[i]=c; }
     password[6] = '\0'; // 最后一位是字符串结束符
     printf("\n验证密码=%s\n",password);
}

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
}

/**********************************
 * 已知三条边a、b、c，求三角形面积   *
 **********************************/
void area1()
{
	printf("======area1()\n");
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
    printf("===========guide_p33()\n");
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
	printf("===========guide_p154()\n");
    char c1='a',c2='B',c3='c';
	int i1=10,i2=20,i3=30;
	double d1=0.1,d2=0.2,d3=0.3;
	printf("%d\n",c1+i2*i3/i2%i1); // 97+20*30/20%30=97+30%30=97
	printf("%d\n",i1+++i2%i3);     // 10+20%30=10+20
	printf("%d\n",c1>i1?i1:c2);    // 上一句中i1++,因此，11
	printf("%d\n",!i1&&i2--);      // 0
	printf("%d\n",i2--);           // 20，上一句中，i2--，未执行，因为a&&b，a为0时，不计算b;即，有0为0
}

void test_format()
{
    printf("===========test_format()\n");
    int i;
    
    for (i=1;i<=5;i++) 
      printf("@%*d\n",i,i); //  printf("@%*d\n",1,1);  printf("@%*d\n",2,2);
} 

void test_getche() 
{
    printf("=====testgetche()\n"); 
    printf("回显测试\n");
	char ch;
	ch=getche(); 
	printf("\n回显测试ch=%c\n",ch);
	//ch=getch();
	//putchar(ch);
	//putchar('\n');
	//getchar(); 
}

void return_scanf()
{  
     printf("==========return_scanf()\n");
     printf("scanf()返回成功读取输入的数量\n");
     int a,b,s=0;
     do {
       printf("input int a,b,非整数结束\n");
       s=scanf("%d%d",&a,&b);
       printf("a,b,s=%d,%d,%d\n",a,b,s);
     } while(s>=2);
}

/*********************
 * ch3. p71,2.写出利用函数getche()和putchar()进行字符串的输入/输出，并对字符的个数进行累加并输出结果的程序。
 * 参考ch5. p91例题 
 *********************/
void p_71_2()
{
     printf("======p_71_2()\n");
     printf("假定输入回车符为止。\n");
     char ch; int len=0;
     puts("Type in a sentence,then press <Enter>\n");
     while((ch=getch())!= '\r') {
        putchar(ch);
        len++;                  
     } 
     printf("len=%d\n",len);
}

void ch3()
{
	printf("======ch3()\n");
	for_getchar();
    password();
	p_71_2();
	lf_test();
    //area1();
	//guide_p33();
	guide_p154();
	test_format(); 
	test_getche();
	return_scanf();
	
}
