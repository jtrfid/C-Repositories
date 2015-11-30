#include <stdio.h>

/************************************************************
 * ch9,p231, 1. 定义名为time_struct的结构体类型，其中包含三个成员:hour(整数)、minute(整数)和second(整数)。
 * 编程实现该结构体类型变量的初始化并按照以下格式显示时间:16:45:11.
 *************************************************************/ 
void ch9_1()
{
     /************* 形式一 ************** 
     typedef struct
     {
        int hour;
        int minute;
        int second;
     }time_struct; 
     
     time_struct ts={16,45,1}; // 定义结构体变量，并初始化 
     *****************************************/
     /************* 形式二 **************/
     struct time_struct
     {
        int hour;
        int minute;
        int second;
     }; 
     
     struct time_struct ts={16,45,1}; // 定义结构体变量，并初始化 
     
     printf("结构体变量成员显示：%02d:%02d:%02d\n",ts.hour,ts.minute,ts.second);  
}

/************************************************************
 * ch9,p231, 2. 修改第一题程序，分别用两个函数实现变量的初始化和变量的显示。 
 *************************************************************/
struct time_struct
     {
        int hour;
        int minute;
        int second;
     }; 
// 结构体变量初始化(方法一) 
void varInit_1(struct time_struct *ts,int hour,int minute,int second)
{
     ts->hour = hour;
     ts->minute = minute;
     ts->second = second;
}

// 结构体变量初始化(方法二) 
struct time_struct varInit_2(struct time_struct ts,int hour,int minute,int second)
{
     ts.hour = hour;
     ts.minute = minute;
     ts.second = second;
     return ts;
}

// 显示结构体变量 
void print_time_struct(struct time_struct ts)
{
      printf("结构体变量成员显示：%02d:%02d:%02d\n",ts.hour,ts.minute,ts.second);  
}

void ch9_2()
{
     struct time_struct ts;
     varInit_1(&ts,12,0,0);
     print_time_struct(ts);
     // 或
     ts = varInit_2(ts,13,0,0);
     print_time_struct(ts);
} 

/************************************************************
 * ch9,p231, 3. 设计一个函数Update，接受第一题所定义的结构体类型参数，然后对该参数所表示的时间加1秒，返回修改结果。 
 *************************************************************/
struct time_struct Update(struct time_struct ts)
{
     ts.second++;
     return ts;
}
void ch9_3()
{
     struct time_struct ts;
     varInit_1(&ts,12,0,0);
     ts = Update(ts);
     print_time_struct(ts);
}

/************************************************************
 * ch9,p231, 4. 定义结构体类型date，包含三个整型成员: day,month,year。
 *   开发模块化、交互式程序，完成以下任务:
 *   (1) 用函数读入成员数据；
 *   (2) 用另外的函数检验日期的有效性；
 *   (3) 用第三个函数打印输出日期，格式为：April 29,2006; 
 *   (4) 编写函数nextday(),计算当前日期的下一个日期为多少。 
 *************************************************************/
struct date
     {
        int day;
        int month;
        int year;
     }; 
// (1) 用函数读入成员数据；
struct date readDate(struct date d,int day,int month,int year)
{
    d.day = day;
    d.month = month;
    d.year = year;
    return d; 
}
// (2) 用另外的函数检验日期的有效性；
// 正确，返回0，否则，返回1 
int checkDate(struct date d)
{
      // 首先进行粗判断，不合法的输入，返回1 
      if (d.day > 31 || d.day < 1) return 1;
      if (!(d.month >= 1 && d.month <= 12)) return 1;
      if (d.year < 0) return 1;
      
      // 其次根据大月或小月以及2月份判断是否是合法输入 
      switch(d.month)
      {
           case 2: // 平年28天，闰年29天 
                   // 符合条件(1)或(2)即为闰年。(1)年号能被4整除但不能被100整除；(2)年号能被400整除
                   if ((d.year%4 == 0 && d.year%100 != 0) || (d.year%400 == 0)) // 闰年 
                   {
                      if (d.day > 29) return 1; // 非法 
                   }
                   else // 平年 
                   {
                      if (d.day > 28) return 1; // 非法 
                   } 
                   break;
           case 1: 
           case 3:
           case 5:
           case 7:
           case 8:
           case 10:
           case 12: // 大月31天 
                    if (d.day > 31) return 1; // 非法 
                    break;       
           default: // 小月30天 
                    if (d.day > 30) return 1; // 非法 
                    break;       
      }
      
      // 合法输入         
      return 0; 
}
// (3) 用第三个函数打印输出日期，格式为：April 29,2006; 
void printDate(struct date d)
{
      char *month[] ={"January","February","March","Aril","May","June","July",
                     "August","September","October","November","December"};
      printf("%s %d,%d\n",month[d.month-1],d.day,d.year);
}
// (4) 编写函数nextday(),计算当前日期的下一个日期为多少。
struct date nextday(struct date d)
{
      // 首先，天数加1 
      d.day++;
      
      // 其次根据大月或小月以及2月份判断是否需要月份加1，进而判断是否引起年号的加1 
      switch(d.month)
      {
           case 2: // 平年28天，闰年29天 
                   // 符合条件(1)或(2)即为闰年。(1)年号能被4整除但不能被100整除；(2)年号能被400整除
                   if ((d.year%4 == 0 && d.year%100 != 0) || (d.year%400 == 0)) // 闰年 
                   {
                      if (d.day > 29)
                      {
                         d.day = 1;
                         d.month++;
                      }
                   }
                   else // 平年 
                   {
                      if (d.day > 28)
                      {
                         d.day = 1;
                         d.month++;
                      }
                   } 
                   break;
           case 1: 
           case 3:
           case 5:
           case 7:
           case 8:
           case 10:
           case 12: // 大月31天 
                    if (d.day > 31)
                    {
                       d.day = 1;
                       d.month++;
                       if (d.month > 12) 
                       { 
                          d.month = 1;
                          d.year++;
                       }
                    }
                    break;       
           default: // 小月30天 
                    if (d.day > 30)
                    {
                       d.day = 1;
                       d.month++;
                    }
                    break;       
      }      
      
      return d; 
}

void ch9_4()
{
     struct date d;
     int day,month,year;
     while(1)
     {
        puts("输入年 月 日，输入0 0 0结束");     
        scanf("%d%d%d",&year,&month,&day);
        if (year==0 && month==0 && day==0) break;
        // 读入成员 
        d = readDate(d,day,month,year); 
        // 检查 
        if (checkDate(d)) {
           puts("输入不合法，重新输入！"); 
           continue; 
        } 
        // 输出 
        printf("您输入的是：");
        printDate(d); 
        
        // 下一日期
        d = nextday(d); 
        printf("明天是："); 
        printDate(d);   
     }
}

/***********************************************************
 * ch9,p232, 5. 定义一个表示向量(vector,即整数序列)的结构体类型，编写程序完成下列任务：
 * (1) 创建一个向量；
 * (2) 修改给定元素的值；
 * (3) 向量和变量的乘；
 * (4) 以下列格式显示向量的值: (10,20,30,...)
 ************************************************************/ 
 // 定义一个向量类型，表示整数序列 
 #define NUM 5
 typedef struct
     {
        int a;
     }vector[NUM]; 
      
// (1) 创建一个向量；用n个整数的数组a,初始化向量v 
void buildVector(vector v,int a[],int n)
{
     int i;
     for(i=0;i<n;i++)
     {
       v[i].a = a[i];
     } 
} 
// (2) 修改给定元素的值；修改第i个(以0开始)向量的值为a 
void modifyVector(vector v, int i, int a)
{
     v[i].a = a; 
} 
// (3) 向量和变量的乘；v*a
void productVector(vector v,int a) 
{
     int i;
     for(i=0;i<NUM;i++)
     {
       v[i].a *= a;
     }
}
// (4) 以下列格式显示向量的值: (10,20,30,...)
void printVector(vector v)
{
     int i;
     for(i=0;i<5;i++)
     {
         printf("%d ",v[i].a);
     } 
     printf("\n");
} 

void ch9_5()
{
     vector v;
     int a[NUM] = {10,20,30,40,50};
     
     // (1) 创建一个向量；用n个整数的数组a,初始化向量v 
     buildVector(v,a,NUM);
     
     // (2) 修改给定元素的值；修改第i个(以0开始)向量的值为a  
     modifyVector(v,1,-20); 
     
     // (3) 向量和变量的乘；v*a
     productVector(v,10);
     
     // (4) 以下列格式显示向量的值: (10,20,30,...)
     printVector(v);
}

void ch9()
{
     ch9_1();
     ch9_2();
     ch9_3();
     ch9_4(); 
     ch9_5(); 
}
