#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/*****************************
 * 1. ѡ���� 
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
    if (s[i] != c) s[j++]=s[i];  // ��� 
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
���к����Ĺ�������һά����a�в���xֵ�����ҵ��򷵻����ڵ��±�ֵ�����򷵻�-1��
���з���a[0]~a[n-1]�С�������ȷִ�д˹��ܵĺ�����____
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
    for (i=0;i<n;i++)  // ���ⶪ��p++ �ʹ��� 
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
//����long int fac(int n)�Ĺ����������n(n>=0)�Ľ׳ˣ�������ɴ˹��ܵ���
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
  while(i<n); // ����д�ɴ���
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
  //char *s="123aa";  // dev-c++��sĩβû���Զ���'\0' 
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
