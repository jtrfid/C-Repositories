//#include "stdafx.h"
#include <stdio.h>

// ch1,p3,3. �㷨��
/********************************
 * ��Ǯ�ټ�,��֪����5��Ǯ1ֻ��ĸ��3��Ǯ1ֻ��С��1��Ǯ3ֻ����100��Ǯ����100ֻ����
 * �ʹ�����ĸ����С������ֻ? 
 *******************************/ 
void ch1_1()
{
     int x,y,z; // ������ĸ����С������
     int flag = 0;
     for(x=0;x<=100;x++) {
        for(y=0;y<=100;y++) {
          for(z=0;z<=100;z++) {
            if(5*x+3*y+z/3 == 100 && x+y+z == 100 && z%3 == 0) {
              printf("����=%d��ĸ��=%d��С��=%d\n",x,y,z);
              flag = 1;
            }
          }
        } 
     } 
     if(flag == 0) 
       printf("�޽⣡\n");
} 

/******************* 
 *
 *******************/
void ch1_2()
{
     int i = 1, sign = 1;
     float item = 1.0, sum = 0.0, pi;
     do {
         item = 1.0/i;
         sum += sign*item;
         i += 2; 
         sign = - sign;
     }while(item > 1.0E-6);
     pi = sum*4.0;
     printf("pi = %f, i = %d\n",pi,i-2); 
}
 
void ch1()
{
     printf("ch1()=========\n");
     ch1_1();
     ch1_2(); 
} 
