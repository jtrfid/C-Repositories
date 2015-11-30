#include <stdio.h>

/************************************************************
 * ch9,p231, 1. ������Ϊtime_struct�Ľṹ�����ͣ����а���������Ա:hour(����)��minute(����)��second(����)��
 * ���ʵ�ָýṹ�����ͱ����ĳ�ʼ�����������¸�ʽ��ʾʱ��:16:45:11.
 *************************************************************/ 
void ch9_1()
{
     /************* ��ʽһ ************** 
     typedef struct
     {
        int hour;
        int minute;
        int second;
     }time_struct; 
     
     time_struct ts={16,45,1}; // ����ṹ�����������ʼ�� 
     *****************************************/
     /************* ��ʽ�� **************/
     struct time_struct
     {
        int hour;
        int minute;
        int second;
     }; 
     
     struct time_struct ts={16,45,1}; // ����ṹ�����������ʼ�� 
     
     printf("�ṹ�������Ա��ʾ��%02d:%02d:%02d\n",ts.hour,ts.minute,ts.second);  
}

/************************************************************
 * ch9,p231, 2. �޸ĵ�һ����򣬷ֱ�����������ʵ�ֱ����ĳ�ʼ���ͱ�������ʾ�� 
 *************************************************************/
struct time_struct
     {
        int hour;
        int minute;
        int second;
     }; 
// �ṹ�������ʼ��(����һ) 
void varInit_1(struct time_struct *ts,int hour,int minute,int second)
{
     ts->hour = hour;
     ts->minute = minute;
     ts->second = second;
}

// �ṹ�������ʼ��(������) 
struct time_struct varInit_2(struct time_struct ts,int hour,int minute,int second)
{
     ts.hour = hour;
     ts.minute = minute;
     ts.second = second;
     return ts;
}

// ��ʾ�ṹ����� 
void print_time_struct(struct time_struct ts)
{
      printf("�ṹ�������Ա��ʾ��%02d:%02d:%02d\n",ts.hour,ts.minute,ts.second);  
}

void ch9_2()
{
     struct time_struct ts;
     varInit_1(&ts,12,0,0);
     print_time_struct(ts);
     // ��
     ts = varInit_2(ts,13,0,0);
     print_time_struct(ts);
} 

/************************************************************
 * ch9,p231, 3. ���һ������Update�����ܵ�һ��������Ľṹ�����Ͳ�����Ȼ��Ըò�������ʾ��ʱ���1�룬�����޸Ľ���� 
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
 * ch9,p231, 4. ����ṹ������date�������������ͳ�Ա: day,month,year��
 *   ����ģ�黯������ʽ���������������:
 *   (1) �ú��������Ա���ݣ�
 *   (2) ������ĺ����������ڵ���Ч�ԣ�
 *   (3) �õ�����������ӡ������ڣ���ʽΪ��April 29,2006; 
 *   (4) ��д����nextday(),���㵱ǰ���ڵ���һ������Ϊ���١� 
 *************************************************************/
struct date
     {
        int day;
        int month;
        int year;
     }; 
// (1) �ú��������Ա���ݣ�
struct date readDate(struct date d,int day,int month,int year)
{
    d.day = day;
    d.month = month;
    d.year = year;
    return d; 
}
// (2) ������ĺ����������ڵ���Ч�ԣ�
// ��ȷ������0�����򣬷���1 
int checkDate(struct date d)
{
      // ���Ƚ��д��жϣ����Ϸ������룬����1 
      if (d.day > 31 || d.day < 1) return 1;
      if (!(d.month >= 1 && d.month <= 12)) return 1;
      if (d.year < 0) return 1;
      
      // ��θ��ݴ��»�С���Լ�2�·��ж��Ƿ��ǺϷ����� 
      switch(d.month)
      {
           case 2: // ƽ��28�죬����29�� 
                   // ��������(1)��(2)��Ϊ���ꡣ(1)����ܱ�4���������ܱ�100������(2)����ܱ�400����
                   if ((d.year%4 == 0 && d.year%100 != 0) || (d.year%400 == 0)) // ���� 
                   {
                      if (d.day > 29) return 1; // �Ƿ� 
                   }
                   else // ƽ�� 
                   {
                      if (d.day > 28) return 1; // �Ƿ� 
                   } 
                   break;
           case 1: 
           case 3:
           case 5:
           case 7:
           case 8:
           case 10:
           case 12: // ����31�� 
                    if (d.day > 31) return 1; // �Ƿ� 
                    break;       
           default: // С��30�� 
                    if (d.day > 30) return 1; // �Ƿ� 
                    break;       
      }
      
      // �Ϸ�����         
      return 0; 
}
// (3) �õ�����������ӡ������ڣ���ʽΪ��April 29,2006; 
void printDate(struct date d)
{
      char *month[] ={"January","February","March","Aril","May","June","July",
                     "August","September","October","November","December"};
      printf("%s %d,%d\n",month[d.month-1],d.day,d.year);
}
// (4) ��д����nextday(),���㵱ǰ���ڵ���һ������Ϊ���١�
struct date nextday(struct date d)
{
      // ���ȣ�������1 
      d.day++;
      
      // ��θ��ݴ��»�С���Լ�2�·��ж��Ƿ���Ҫ�·ݼ�1�������ж��Ƿ�������ŵļ�1 
      switch(d.month)
      {
           case 2: // ƽ��28�죬����29�� 
                   // ��������(1)��(2)��Ϊ���ꡣ(1)����ܱ�4���������ܱ�100������(2)����ܱ�400����
                   if ((d.year%4 == 0 && d.year%100 != 0) || (d.year%400 == 0)) // ���� 
                   {
                      if (d.day > 29)
                      {
                         d.day = 1;
                         d.month++;
                      }
                   }
                   else // ƽ�� 
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
           case 12: // ����31�� 
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
           default: // С��30�� 
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
        puts("������ �� �գ�����0 0 0����");     
        scanf("%d%d%d",&year,&month,&day);
        if (year==0 && month==0 && day==0) break;
        // �����Ա 
        d = readDate(d,day,month,year); 
        // ��� 
        if (checkDate(d)) {
           puts("���벻�Ϸ����������룡"); 
           continue; 
        } 
        // ��� 
        printf("��������ǣ�");
        printDate(d); 
        
        // ��һ����
        d = nextday(d); 
        printf("�����ǣ�"); 
        printDate(d);   
     }
}

/***********************************************************
 * ch9,p232, 5. ����һ����ʾ����(vector,����������)�Ľṹ�����ͣ���д���������������
 * (1) ����һ��������
 * (2) �޸ĸ���Ԫ�ص�ֵ��
 * (3) �����ͱ����ĳˣ�
 * (4) �����и�ʽ��ʾ������ֵ: (10,20,30,...)
 ************************************************************/ 
 // ����һ���������ͣ���ʾ�������� 
 #define NUM 5
 typedef struct
     {
        int a;
     }vector[NUM]; 
      
// (1) ����һ����������n������������a,��ʼ������v 
void buildVector(vector v,int a[],int n)
{
     int i;
     for(i=0;i<n;i++)
     {
       v[i].a = a[i];
     } 
} 
// (2) �޸ĸ���Ԫ�ص�ֵ���޸ĵ�i��(��0��ʼ)������ֵΪa 
void modifyVector(vector v, int i, int a)
{
     v[i].a = a; 
} 
// (3) �����ͱ����ĳˣ�v*a
void productVector(vector v,int a) 
{
     int i;
     for(i=0;i<NUM;i++)
     {
       v[i].a *= a;
     }
}
// (4) �����и�ʽ��ʾ������ֵ: (10,20,30,...)
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
     
     // (1) ����һ����������n������������a,��ʼ������v 
     buildVector(v,a,NUM);
     
     // (2) �޸ĸ���Ԫ�ص�ֵ���޸ĵ�i��(��0��ʼ)������ֵΪa  
     modifyVector(v,1,-20); 
     
     // (3) �����ͱ����ĳˣ�v*a
     productVector(v,10);
     
     // (4) �����и�ʽ��ʾ������ֵ: (10,20,30,...)
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
