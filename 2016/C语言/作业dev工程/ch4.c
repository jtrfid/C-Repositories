#include <stdio.h>

/*************************************
 * ch4,p87, 2. 判断下列关系式或逻辑表达式的运算结果：
 * (1) 10 == 9+1
 * (2) 10 && 8
 * (3) 6 || 0
 * (4) !(2+5)
 * (5) 0&&-3
 * (6) 设x=15,y=10: x>=9 && y<=x 
 *************************************/
void ch4_2()
{
     printf("=========ch4_2()\n"); 
     printf("(1) 10 == 9+1 [%d]\n",10 == 9+1);  // 1
     printf("(2) 10 && 8 [%d]\n",10 && 8); // 1
     printf("(3) 6 || 0 [%d]\n",6 || 0);   // 1
     printf("(4) !(2+5) [%d]\n",!(2+5));   // 0
     printf("(5) 0&&-3 [%d]\n",0&&-3);     // 0
     int x=15,y=10;
     printf("(6) 设x=15,y=10: x>=9 && y<=x [%d]\n",x>=9 && y<=x); // 1
} 

/*******************************************
 * ch4,p88，3. 求下列各表达式的值。设a=3,b=4,c=5
 * (1) a+b>c&&b==c
 * (2) a||b+c&&b-c
 * (3) !(x=a)&&(y=b)&&0
 * (4) !(a+b)+c-1&&b+c/2
 ********************************************/
void ch4_3()
{
     printf("=========ch4_3()\n"); 
     int a=3,b=4,c=5,x,y;
     printf(" (1) a+b>c&&b==c [%d]\n",a+b>c&&b==c);  // 0
     printf(" (2) a||b+c&&b-c [%d]\n",a||b+c&&b-c);  // 1
     printf(" (3) !(x=a)&&(y=b)&&0 [%d]\n",!(x=a)&&(y=b)&&0);   // 0 
     printf(" (4) !(a+b)+c-1&&b+c/2 [%d]\n",!(a+b)+c-1&&b+c/2); // 1
}

/********************************************
 * ch4,p88，5. 编程求某数x的绝对值.
 ********************************************/ 
void ch4_5()
{
    printf("=========ch4_5()\n");
    float x;
    printf("输入数值，求绝对值。输入0，退出\n"); 
    while(1) {
      scanf("%f",&x);
      if (x==0.0) break; 
      printf("%f的绝对值是: %f\n",x,x < 0.0 ? -x : x);
    }  
}

/********************************************
 * ch4,p88，6. 有3个整数a,b,c，由键盘输入，输出其中最大的数.
 ********************************************/ 
void ch4_6()
{
    printf("=========ch4_6()\n");
    int a,b,c,max;
    printf("输入3个整数，空格隔开，输出最大的数。输入3个0，退出\n"); 
    while(1) {
      scanf("%d%d%d",&a,&b,&c);
      if (a==0) break;
      max = a; 
      if (b > max) max = b;
      if (c > max) max = c;  
      printf("%d,%d,%d,最大的是: %d\n",a,b,c,max);
    }  
}

/********************************************
 * ch4,p88，7. 求一元二次方程a*x*x+b*x+c=0的根,其中a,b,c是任意实数。
 * 判别式d = b*b-4*a*c 
 * 当 d = 0时，方程有两个相等的实根：
 *   x1=x2=-b/(2*a) 
 * 当d > 0时，方程有两个不相等的实根：
 *   x1=(-b+sqrt(d))/(2*a) 
 *   x2=(-b-sqrt(d))/(2*a)  
 * 当d < 0时，方程有两个虚根： 
 *   x1=jp+ipi           x2=jp-ipi
 *   实部 jp=-b/(2*a)      虚部 ip=sqrt(-d)/(2*a)
 ********************************************/ 
void ch4_7()
{
     printf("=========ch4_7()\n");
     float a,b,c,d,x1,x2,jp,ip;
     printf("输入一元二次方程3个系数a,b,c，空格隔开。\n"); 
     scanf("%f%f%f",&a,&b,&c);
     d=b*b-4*a*c;
     if (d == 0.0)       /* 相等的实根  */
     { 
       printf("The equation has two equal roots:\n");
       printf("x1=x2=%8.4f\n",-b/(2*a)); 
     }
     else if (d > 0.0)  /* 不相等的实根 */
     { 
        x1=(-b+sqrt(d))/(2*a); 
        x2=(-b-sqrt(d))/(2*a);
        printf("The equation has two real roots:\n") ;
        printf("x1=%8.4f,x2=%8.4f\n",x1,x2);
     }
     else             /*  虚根 */
     { 
        jp=-b/(2*a);  ip=sqrt(-d)/(2*a);
        printf("The equation has two complex roots: \n");
        printf("x1=%8.4f+%8.4fi\n",jp,ip);
        printf("x2=%8.4f-%8.4fi\n",jp,ip);
     }
}

/********************************************
 * ch4,p88，8. 函数: 
               y = x       (x<1)
               y = 2x-1    (1<=x<3)
               y= 3*x*x-10 (x>=3)
               输入x,输出y值。 
 ********************************************/ 
void ch4_8()
{
    printf("=========ch4_8()\n");
    float x,y;
    printf("输入x，输出y。输入0，退出\n"); 
    while(1) {
      scanf("%f",&x);
      if (x==0.0) break;
      if (x < 1.0) y = x;
      else if (1.0<=x && x<3.0) y = 2.0*x-1.0;
      else y = 3.0*x*x-10.0; 
      printf("x = %f，y = %f\n",x,y);
    }  
}

/********************************************
 * ch4,p88，9. 输入百分制成绩，要求输出成绩等级
   'A', 90分以上
   'B', 80~89分
   'C', 70~79分
   'D', 60~69分
   'E', 60分以下 
   解法一，if else if ...
   解法二，switch 
 ********************************************/ 
void ch4_9_1(float x)
{
    printf("=========ch4_9(),解法一\n");
    
    if (x >= 90.0) putchar('A');
    // else if (80.0<=x && x<=89.0) putchar('B');  // 错误,例如89.5，输出'E' 
    // else if (70.0<=x && x<=79.0) putchar('C');
    // else if (60.0<=x && x<=69.0) putchar('D');
    else if (80.0<=x) putchar('B'); 
    else if (70.0<=x) putchar('C');
    else if (60.0<=x) putchar('D');
    else putchar('E');; 
    putchar('\n'); 
}

/********************************************
 * ch4,p88，9. 输入百分制成绩，要求输出成绩等级
   'A', 90分以上
   'B', 80~89分
   'C', 70~79分
   'D', 60~69分
   'E', 60分以下 
   解法一，if else if ...
   解法二，switch 
 ********************************************/ 
void ch4_9_2(float x)
{
    printf("=========ch4_9(),解法二\n");
   
    switch((int)(x/10.0)) 
    {
      case 10: 
      case 9: putchar('A'); break;
      case 8: putchar('B'); break;
      case 7: putchar('C'); break;
      case 6: putchar('D'); break;
      default: putchar('E'); 
    } 
    putchar('\n');
}

/********************************************
 * ch4,p88，9. 输入百分制成绩，要求输出成绩等级
   'A', 90分以上
   'B', 80~89分
   'C', 70~79分
   'D', 60~69分
   'E', 60分以下 
   解法一，if else if ...
   解法二，switch 
 ********************************************/ 
void ch4_9()
{
    printf("=========ch4_9()\n");
    float x;
    printf("输入成绩x，输出等级。输入0，退出\n"); 
    while(1) {
      scanf("%f",&x);
      if (x==0.0) break;
      ch4_9_1(x); // 解法一 
      ch4_9_2(x); // 解法二 
    }  
}

/********************************************
 * ch4,p88，10. 输入两个整数的四则运算式(+、-、*、/) ,输出计算结果。
   如输入: 123+456，应该输出123+456=579； 
 ********************************************/ 
void ch4_10()
{
    printf("=========ch4_10()\n");
    int a,b;
    char op;
    printf("输入整数 运算符 整数，无空格输入。输入0+0，退出\n"); 
    while(1) {
      scanf("%d%c%d",&a,&op,&b); // 不要用空格隔开，否则，op=空格，无空格输入,如3+2 
      // 用下列语句验证输入： 
      printf("验证接收的输入：a,op,b = %d,%c,%d\n",a,op,b);
      if (a==0) break;
      switch(op)
       {
          case '+': printf("%d+%d=%d\n",a,b,a+b); break;
          case '-': printf("%d-%d=%d\n",a,b,a-b); break;
          case '*': printf("%d*%d=%d\n",a,b,a*b); break;
          case '/': if(b==0.0) 
                       printf("除数为零\n"); 
                     else
                       printf("%d/%d=%.2f\n",a,b,(float)a/b); break;  // 一定要强制转换 
          default: printf("操作符错误\n");
       } 
    }  
}
 
void ch4()
{
     printf("===========ch4()\n");
     ch4_2();
     ch4_3(); 
     ch4_5(); 
     ch4_6(); 
     ch4_7();
     ch4_8();  
     ch4_9();
     ch4_10(); 
} 
