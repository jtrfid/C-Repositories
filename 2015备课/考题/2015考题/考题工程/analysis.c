#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
/*****************************
 * 2. 程序分析题 ，写出输出结果 
 ****************************/
 
 ///////////////////////// A2_1
 int fun()
 {
    static int x=1;
    x *= 2;
    return x;
 } 
 void A2_1()
 {
     int i,s=1;
     for(i=1;i<=2;i++) s=fun();
     printf("%d\n",s);  // 4
 }
 
 ////////////////////////// A2_2
 void strcat_cat(char *s1,char *s2)
 {
     int i=0;
     strcat(s1,s2);
     while(s1[i++]!='\0') s2[i]=s1[i];
     s2[i] = '\0';
 } 
 void A2_2()
 {
     char a[80]="12",b[80]="abcd";
     strcat_cat(a,b);
     puts(b);  // a2abcd
 }
 
 ////////////////////////// A2_3
 int item(int n);
 void A2_3()
 {   
    int i,n;
    scanf("%d",&n);
    for(i=1;i<=n;i++)
       printf(i<n?"%d ":"%d\n",item(i)); // 1 1 2 3 5 8
 }
 int item(int n)
 {
     int item1,item2;
     if(n==1||n==2) { return 1; }
     item1 = item(n-1);
     item2 = item(n-2);
     return (item1+item2);     
 }
 
 ////////////////////////// A2_4
 int sum_product(int a,int b,int *sum)
 {
    *sum = a+b;
    a++; b++;
    return (a*b); 
 } 
 void A2_4()
 {
     int a,b,sum,product;
     scanf("%d%d",&a,&b);
     product = sum_product(a,b,&sum);
     printf("%d,%d,%d,%d\n",a,b,sum,product); // 5,6,11,42
 }

 ////////////////////////// A2_5
 void A2_5()
 {
     struct mn 
     {
       int x;
       int *y;
     };
     struct mn *p;
     int a[]={10,20,30};
     struct mn aa[]={{15,&a[0]},{25,&a[1]},{35,&a[2]}};
     for(p=aa;p<aa+3;p++) 
        printf("%d %d ",p->x,*(p->y)); // 15,10,25,20,35,30
     printf("\n");
 }
 
 ///////////////////////// B2_1
 int fun1(int *a)
 {
    static int d=5;
    (*a)++;d++;
    return d;
 } 
 void B2_1() 
 {
     int i,a=1,d;
     for(i=1;i<=2;i++) d=fun1(&a);
     printf("%d,%d\n",a,d);  // 3,7
 }
 
 ///////////////////////// B2_2
 int f(int a[],int n);
 void B2_2()
 {
      int a[]={1,2,3,4},s;
      s=f(a,4);
      printf("%d\n",s);
 } 
 
 int f(int a[],int n)
 {
     if(n==0) return 0;
     if(n>0) return a[n-1]+f(a,n-1); 
 }
 
 ///////////////////////// B2_3
 void fun2(char *a,int n)
 {
      char c,*p1,*p2;
      p1=a; p2=a+n-1;
      while(p1<p2) 
      {
         c=*p1; *p1=*p2; *p2=c;
         p1++; p2--;
      }
 }
 
 void B2_3()
 {
      char a[80];
      gets(a);
      fun2(a,strlen(a));
      puts(a);
 }
 
 void analysis()
 {
      /****
      printf("A2_1()=====\n"); 
      A2_1();
      printf("A2_2()=====\n"); 
      A2_2();
      printf("A2_3()=====,输入6\n"); 
      A2_3();
      printf("A2_4()=====,输入5 6\n"); 
      A2_4();
      printf("A2_5()=====,\n"); 
      A2_5();
      *****/
      
      printf("B2_1()=====\n"); 
      B2_1();
      
      printf("B2_2()=====\n"); 
      B2_2();
      
      printf("B2_3()=====\n"); 
      B2_3();
 } 
