#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/*****************************
 * 1. 选择题 
 ****************************/
void A1_6()
{
  int a,b,c,x;
  a=b=c=0;x=35;
  if (!a) { x--; b++; c++; }
  else if (b) x=3;
  else if (c) x=4;
  else x=5;
  printf("x=%d\n",x); // 34
}

void A1_8(char *s,char c)
{
  int i,j;
  for(i=j=0;s[i]!='\0';i++)
    if (s[i] != c) s[j++]=s[i];  // 填空 
  s[j]='\0';   
}

void A1_13()
{
   char ch; int i=0;
   for (ch='A';ch<='Z';ch++,i++)
     if (i<5) continue;
     else break;
    printf("%c\n",ch);  // F
}

/////////////////// B1_14
/*****************************
下列函数的功能是在一维数组a中查找x值，若找到则返回所在的下标值，否则返回-1；
数列放在a[0]~a[n-1]中。不能正确执行此功能的函数是____
*****************************/
int funa(int *a,int n,int x) // ok
{
    int i = 0; 
    while(i<n && a[i]!=x) i++;
    if (i>=n) i=-1;
    return i;  
}

int funb(int *a,int n,int x) // ok
{
    int i; 
    for (i=0;i<n;i++)
      if (a[i]==x) return i; 
    return -1;  
}

int func(int a[],int n,int x) 
{
    int i,*p=a; 
    for (i=0;i<n;i++)  // 故意丢掉p++ 就错了 
    //for (i=0;i<n;i++,p++)
      if (*p==x) return i; 
    return -1;  
}

int fund(int a[],int n,int x)  // ok
{
    int *p; 
    for (p=a;p<a+n;p++)
      if (*p==x) return p-a; 
    return -1;  
}

//////////B1_15 
//函数long int fac(int n)的功能是求参数n(n>=0)的阶乘，不能完成此功能的是
long int fac1(int n) // ok 
{
  long int f = 1;
  int i;
  for(i=1;i<=n;i++) f *= i;
  return f;
}

long int fac2(int n) 
{
  long int f = 1;
  int i = 1;
  do 
    f = f*i++;     
  //while(i<=n); // ok
  while(i<n); // 故意写成错误
  return f;
}

long int fac3(int n) // ok
{
  long int f = 1;
  int i = n;
  while(i>0) 
   f = f*i--; 
  return f;
}

long int fac(int n) // ok
{
  if (n==1 || n==0) return 1; 
  else return n*fac(n-1);
}

int choice()
{
  //////////////// A1_1
  {
    int a=1;
    printf("A1_1:%d,%d\n",2%a,a%2);// 0,1
  }
  
  //////////////// A1_3
  {
    int a=1;
    printf("A1_3:%d,%d,%d,%d\n",a%2==0,!(a%2!=0),!(a%2),!a%2); // 0,0,0,0
    a=2;
    printf("A1_3:%d,%d,%d,%d\n",a%2==0,!(a%2!=0),!(a%2),!a%2); // 1,1,1,0
  }
  
  /////////////// A1_6
  printf("A1_6()====\n");
  A1_6(); // 34
  
  ////////////// A1_14
  {
    printf("A1_14\n");
    int a=1;
    switch(a+1)
    {
        case 1: a++;break;
        case 2: a++;
        case 3: a++;break;
        default: a++;
    }
    printf("%d\n",a); // 3
  }

  //////////////// A1_8
  printf("A1_8()========\n");
  char s[]="c123accac";
  //char *s="123aa";  // dev-c++中s末尾没有自动加'\0' 
  A1_8(s,'c');
  puts(s); 

  printf("A1_13()========\n");
  A1_13();

  

//  printf("B1_14()========\n");

//  int a[]={1,2,3};
//  printf("funa(a,3,2)=%d\n",funa(a,3,2));
//  printf("funa(a,3,1)=%d\n",funa(a,3,1));
//  printf("funa(a,3,3)=%d\n",funa(a,3,3));
//  printf("funa(a,3,-3)=%d\n",funa(a,3,-3));
//  
//  printf("funb(a,3,2)=%d\n",funb(a,3,2));
//  printf("funb(a,3,1)=%d\n",funb(a,3,1));
//  printf("funb(a,3,3)=%d\n",funb(a,3,3));
//  printf("funb(a,3,-3)=%d\n",funb(a,3,-3));
//  
//  printf("func(a,3,2)=%d\n",func(a,3,2));
//  printf("func(a,3,1)=%d\n",func(a,3,1));
//  printf("func(a,3,3)=%d\n",func(a,3,3));
//  printf("func(a,3,-3)=%d\n",func(a,3,-3));
//  
//  printf("fund(a,3,2)=%d\n",fund(a,3,2));
//  printf("fund(a,3,1)=%d\n",fund(a,3,1));
//  printf("fund(a,3,3)=%d\n",fund(a,3,3));
//  printf("fund(a,3,-3)=%d\n",fund(a,3,-3));

  printf("B1_15()========\n");
  printf("fac1(4)=%ld\n",fac1(4)); // 24
  printf("fac2(4)=%ld\n",fac2(4)); // 24
  printf("fac3(4)=%ld\n",fac3(4)); // 24
  printf("fac(4)=%ld\n",fac(4)); // 24
  printf("fac1(0)=%ld\n",fac1(0)); // 1
  printf("fac2(0)=%ld\n",fac2(0)); // 1
  printf("fac3(0)=%ld\n",fac3(0)); // 1
  printf("fac(0)=%ld\n",fac(0)); // 1
//  

}
