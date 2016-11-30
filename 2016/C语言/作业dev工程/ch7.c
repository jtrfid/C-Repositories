//#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*************************************** 
 * ch7,p155, 1. 定义一个带参数的宏(swap(x1,y1)实现两个整数之间的交换)，并利用它将数组x和y进行交换。 
 ***************************************/
// 按位运算对象只能是char,int型的变量，不能是表达式，因此宏体中的参数可以不用括号扩起来
// 注意，使用','表达式，可以不用{} 
#define SWAP1(x1,y1) x1=x1^y1,y1=y1^x1,x1=x1^y1
// 注意，加上{},是一个安全的选择 
#define SWAP2(x1,y1) { x1=x1^y1;y1=y1^x1;x1=x1^y1; }
#define SWAP3(x1,y1) { int tmp; tmp=x1; x1=y1; y1=tmp; }
#define SWAP(x1,y1) SWAP3(x1,y1)
void ch7_1()
{
     printf("ch7_1(),定义带参数的宏，实现两个整数之间的交换\n"); 
     int a=10,b=20;
     SWAP(a,b);
     printf("a,b=%d,%d\n",a,b);
     
     int x[5] = {1,2,3,4,5},y[5] = {11,22,33,44,55},i; 
     for(i=0;i<5;i++) SWAP(x[i],y[i]);
     //输出验证 
     for(i=0;i<5;i++) printf("%4d",x[i]);  
     putchar('\n');
     for(i=0;i<5;i++) printf("%4d",y[i]);
     putchar('\n');
}

/*************************************** 
 * ch7,p155, 2. 对输入的一行纯字母组成的字符串，
 * 使用条件编译使之按统一的大写字母或小写字母格式输出。 
 ***************************************/
//输出大写 
#define CAPITAL 
#define UPPER(c) putchar((c)>='a' && (c)<='z' ?(c)-32 : (c))
#define LOWER(c) putchar((c)>='A' && (c)<='Z' ?(c)+32 : (c))
void ch7_2()
{
     printf("ch7_2(),条件编译，对输入的字符串，大写或小写输出\n"); 
     char s[80],c;
     int i;
     
     puts("请输入字符串："); 
     gets(s);
     
     puts("输出的字符串："); 
     #ifdef CAPITAL
     // 大写输出 
     i = 0;
     while((c=s[i++])!='\0') UPPER(c);
     #else
     // 小写输出 
     i = 0;
     while((c=s[i++])!='\0') LOWER(c);  
     #endif
     putchar('\n'); 
}

/*************************************** 
 * ch7,p155, 3. 输入半径r，求圆的面积和周长，用带参数的宏实现，并求半径为r+1的圆的面积和周长。 
 ***************************************/
#define PI 3.14159
#define AREA(r) PI*(r)*(r)
#define C(r) 2*PI*(r) 
void ch7_3()
{
     printf("ch7_3(),定义带参数的宏，求面积和周长\n");
     float r;
     r=3.0;
     printf("半径为%f,面积=%f，周长=%f\n",r,AREA(r),C(r)); 
     printf("半径为%f,面积=%f，周长=%f\n",r+1,AREA(r+1),C(r+1)); 
}

/**************************************************
 * ch7,p155, 第4题见 ch5, p106, 5. 输入两个正整数m和n，求其最大公约数和最小公倍数 
 ***************************************************/ 

/******************************************************
 * ch7,p155, 5. 编写一个函数，由实参传来一个字符串，统计此字符串中的字母，数字，空格和其它字符的个数。
 *              在主函数中输入字符串及输出上述结果。 
 * ch8,p192, 第3题用指针方法实现。 
 ******************************************************/ 
void ch7_5(char s[])
{
     printf("ch7_5(char s[]),统计s中字母、数字、空格和其它字符的个数\n");
     
     int i;
     int Letter = 0, Number = 0, Space = 0, Other = 0; // 字母、数字、空格和其它字符的个数
     char c;
     
     for (i=0; (c = s[i]) != 0; i++) {  //或 (c = s[i]) != '\0'
         if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) Letter++;
         else if ('0' <= c && c <= '9') Number++;
         else if (c == ' ') Space++;
         else Other++;
     } 
     
      printf("字母、数字、空格和其它字符的个数=%d,%d,%d,%d\n",Letter,Number,Space,Other);
} 

/*************************************** 
 * ch7,p156, 6. 利用弦截法计算方程x*x*x-5*x*x+16*x-80=0的根。 
 * 弦截法是一种求方程根的基该方法，思路：任取两个数x1、x2，求得对应的函数值f(x1）、f(x2）。
 * 如果两函数值同号，则重新取数，直到这两个函数值异号为止。
 * 连接（x1,f(x1））与（x2,f(x2））这两点形成的直线与x轴相交于一点x，求得对应的f(x），
 * 判断其与f(x1）、f(x2）中的哪个值同号。如f(x）与f(x1）同号，则f(x）为新的f(x1)。
 * 将新的f(x1）与f(x2）连接，如此循环。体现的是极限的思想。
 *
 * 注：输入x1,x2不合适，即使异号，也得不到正确的结果 
 ***************************************/
//计算f(x）的值, 独立函数，替换为不同的函数，算法不变 
float f(float x)
{ 
    return x*(x*x-5*x+16)-80;
    
    // 以下使用验证方程 
    // 输入两个数x1,x2:1.2 1.4  方程的一个解为: 1.324723
    //return x*x*x-x-1; 
}

//计算与x轴交点的x值
float point(float x1, float x2)
{
    // f(x1)/(x-x1) = f(x2)/(x2-x) ==> x
    return (x1*f(x2)-x2*f(x1)/(f(x2)-f(x1)));
}

void ch7_6()
{
    printf("ch7_5(),弦截法计算方程的根\n"); 
    //输入两个数x1,x2
    float x1,x2,x;
    do
    {
      printf("输入两个数x1,x2:");
      scanf("%f%f",&x1,&x2);
    }
    while (f(x1)*f(x2) >= 0); // 当同号时，继续重新输入
    
    //关键循环步骤：
    do
    {
      x=point(x1,x2); //得到交点的值
      if(f(x)*f(x1) > 0)
        x1 = x;  //新的x1
      else
        x2 = x;
    }
    while (fabs(f(x)) > 0.0001); //0.0001为取值精度
    
    printf("方程的一个解为: %f\n",x);
}

/**********************************************
 * ch7,p155, 7. 用递归方法求n阶勒让德多项式的值，递归公式为： 
 *  P(x,n) = 1  n=0
 *  p(x,n) = x  n=1
 *  p(x,n) = ((2n-1)x*p(x,n-1)-(n-1)p(x,n-2))/n   n>1
 **********************************************/ 
double ch7_7(double x,int n)
{
    printf("float ch7_7(float x,int n),勒让德公式\n"); 
    if (n == 0) return 1.0;
    if (n == 1) return x;
    return ((2*n-1)*x*ch7_7(x,n-1)-(n-1)*ch7_7(x,n-2))/n;
} 

/****************************************
 * 循环法求ch7_7 错误 
 ****************************************/ 
 double xxxxch7_7_1(double x,int n)
 {
   double p1,p2;
   int i;
   p1 = 1.0;   p2 = x;
   for(i = 2; i <= n; i++) {
       p1 = ((2*i-1)*x*p2-(i-1)*p1)/i;  
       p2 = ((2*i-1)*x*p1-(i-1)*p2)/i;  // 由于有i关系，不能套用f1 = f1 + f2; f2 = f2 + f1; p104页的Fibonacci数列的方法 
   } 
   return p2;
 }
 
 /****************************************
 * 循环法求ch7_7 正确 
 ****************************************/ 
 double ch7_7_1(double x,int n)
 {
   double p1,p2,p;
   int i;
   p1 = 1.0;   p2 = x;
   for(i = 2; i <= n; i++) {
       p = ((2*i-1)*x*p2-(i-1)*p1)/i;
       p1 = p2; 
       p2 = p;
   } 
   return p;
 }
 
 /****************************************
 * 循环法求ch7_7 正确 
 ****************************************/ 
 double okch7_7_1(double x,int n)
 {
   double p[100];
   int i;
   p[0] = 1.0;   p[1] = x;
   for(i = 2; i <= n; i++) {
       p[i] = ((2*i-1)*x*p[i-1]-(i-1)*p[i-2])/i;
       printf("%d,%lf\n",i,p[i]);
   } 
   return p[n];
 }

/*************************************** 
 * ch7,p156, 8. 用函数递归方法以字符串形式输出一个整数。
 解法一， 同p153，例7.20
 ***************************************/
void ch7_8_1(int n)
{
	//printf("ch7_8_1(int n), 以字符串形式输出一个整数。解法一，同p153，例7.20\n");
	
	// 处理负数 
	if(n < 0) { putchar('-'); n = -n; } 
	 
	if(n/10) ch7_8_1(n/10);
	// 个位数，输出 
	putchar(n%10+'0'); // 数字字符整数值(ascii码) = 数字 + '0'; 如，49 = 1+'0',表示'1'
}

/*************************************** 
 * ch7,p156, 8. 用函数递归方法以字符串形式输出一个整数。
 * 解法二， 使用数组或指针，存储由整数转换的字符串 
 * 参数:char *s,或s[],返回数字字符串
 *      int n 输入整数值 
 ***************************************/
void ch7_8_2(char s[],int n)
// void ch7_8(char *s,int n) // 等效 
{
	//printf("ch7_8(char s[],n)，以字符串形式输出一个整数，解法二， 使用数组或指针，存储由整数转换的字符串\n");
	
	// 处理负数 
	if(n < 0) { putchar('-'); n = -n; } 
	
	if(n/10) ch7_8_2(s,n/10);
	
	// 以下处理个位数 
	int len=strlen(s);
	s[len]=n%10+'0';  // 数字字符整数值(ascii码) = 数字 + '0'; 如，49 = 1+'0',表示'1' 
	s[len+1]='\0';  // 字符串最后字符置'\0' 
}

/***************************************************
  ch7,156, 9. 编写函数，根据整型参数m的值，计算公式：
     t=1-1/(2*2)-1/(3*3)-...-1/(m*m)
 的值。例如：m=5,则输出0.536389
 ***************************************************/
double ch7_9(int m)
{
	printf("double ch7_9(int m), m=5,则输出0.536389。\n");
	double t=1.0;
	int i;
	
	for(i=2;i<=m;i++) 
	{
		t-=1.0/(i*i); // 注意，必须是1.0 
	}
	return (t);
}

/**********************************************
 * ch7,p156, 10. 编写函数factor(n),求n!
 * 再利用它求出表达式m!/(n!*(m-n)!)的值
 * 两种方法求n!:
 * 1. 循环语句， 2.递归调用
 * 下列两个函数均可 
 **********************************************/ 
long int factor(int n)
{
     int i;
     long int fac = 1L;  // 如果参数n=0,下列循环语句不执行，将返回1L，符合阶乘的定义 
     for (i=1;i<=n;i++) fac *= i; 
     return fac;
} 

long int factor1(int n)
{
     if (n == 0) return 1;
     if (n == 1) return 1;
     return n*factor(n-1);
} 

void ch7_10(int m,int n)
{
    printf("ch7_10(int m,int n)，调用求n!函数，求表达式的值。\n"); 
    
    if (m<0 || n<0 || m<n) { 
         printf("参数输入不合适，退出！\n");
         return; 
    } 
    float f;
    f = (float)factor(m)/(factor(n)*factor(m-n)); // 注意，一定要强制类型转换，否则失去精度。 
    printf("表达式的值=%f\n",f); 
} 

void ch7()
{
   
    // ch7_1(),定义带参数的宏，实现两个整数之间的交换
    ch7_1();
    
    // ch7_2(),条件编译，对输入的字符串，大写或小写输出
    ch7_2(); 
    
    
    // ch7_3(),定义带参数的宏，求面积和周长
    ch7_3();
    
    // ch7_5(char s[]),统计s中字母、数字、空格和其它字符的个数\n");
    ch7_5("12A ~cB "); 
    
    // ch7_6(),弦截法计算方程的根
    //ch7_6();
    
    // ch7_7(float x,int n),勒让德公式 
    printf("递归法勒让德: %lf\n",ch7_7(4,3));
    printf("循环法勒让德: %lf\n",ch7_7_1(4,3));
       
    // ch7_8_1(int n), 以字符串形式输出一个整数。解法一，同p153，例7.20
   	printf("ch7_8_1(int n), 以字符串形式输出一个整数。解法一，同p153，例7.20\n");
    ch7_8_1(-3781);
    putchar('\n');
    
    // ch7_8_2(char s[],n)，以字符串形式输出一个整数，解法二， 使用数组或指针，存储由整数转换的字符串
    printf("ch7_8(char s[],n)，以字符串形式输出一个整数，解法二， 使用数组或指针，存储由整数转换的字符串\n");
    char s[20]=""; // s[0]='\0',注意，一定给字符串初始化为以'\0'结束，才能用string.h中的函数 
    ch7_8_2(s,-6070);
    puts(s);
    
    // double ch7_9(int m), m=5,则输出0.536389
	printf("%lf\n",ch7_9(5)); 
	
	// ch7_10(int m,int n)，调用求n!函数，求表达式的值。\n"); 
	ch7_10(3,2);
}
