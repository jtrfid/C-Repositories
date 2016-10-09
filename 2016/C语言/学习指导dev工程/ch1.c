//#include "stdafx.h"
#include <stdio.h>

// ch1,p3,3. 算法题
/********************************
 * 百钱百鸡,已知公鸡5个钱1只，母鸡3个钱1只，小鸡1个钱3只，用100个钱买了100只鸡，
 * 问公鸡、母鸡、小鸡各几只? 
 *******************************/ 
void ch1_1()
{
     int x,y,z; // 公鸡、母鸡、小鸡个数
     int flag = 0;
     for(x=0;x<=100;x++) {
        for(y=0;y<=100;y++) {
          for(z=0;z<=100;z++) {
            if(5*x+3*y+z/3 == 100 && x+y+z == 100 && z%3 == 0) {
              printf("公鸡=%d、母鸡=%d、小鸡=%d\n",x,y,z);
              flag = 1;
            }
          }
        } 
     } 
     if(flag == 0) 
       printf("无解！\n");
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
