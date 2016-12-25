#include <stdio.h>
#include <stdlib.h>

void ch1(); 
void ch2();
void ch3();
void ch4(); 
void ch5();
void ch6();
void ch7();
void ch8(); 
void ch9();
void test();

int main(int argc, char *argv[])
{
  printf("C语言程序设计作业汇总:\n"); 
  //ch1();  
  //ch2(); 
  //ch3();
  //ch4(); 
  //ch5();
  //ch5_2();
  //ch7();
  //ch8(); 
  //ch9();
  //ch9_4();
  
  //double ch7_9(int m);      // 一定要有函数原型说明，否则认为是返回int 
  //printf("%lf\n",ch7_9(5)); // 没有上一句的函数说明，这里会返回0.000000 
  
  test();
  
  system("PAUSE");	 // 同system("pause");
  return 0;
}

void test(){
    char *s = "ab\018"; char ch;
    while(ch = *s++) printf("%c,",ch); // a,b,\01,8,
    printf("\n"); 
    char *s1 = "ab\09"; 
    while(ch = *s1++) printf("%c,",ch); // a,b,
    printf("\n"); 
    char *s2 = "ab\012"; 
    while(ch = *s2++) printf("%c,",ch); // a,b,\012
    printf("\n"); 
    char *s3 = "\ta\018bc"; 
    while(ch = *s3++) printf("%c,",ch); // \t,a,\01,8,b,c,
    printf("\n"); 
    
    printf("%d\n",strlen("ab\018")); // 4
    printf("%d\n",strlen("ab\09")); // 2
    printf("%d\n",strlen("ab\012")); // 3
    printf("%d\n",strlen("\ta\018bc")); // 6

}
