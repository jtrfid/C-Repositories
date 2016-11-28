//#include "stdafx.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/********************************************************
 * ch6,p125, 1. 指出下列说明语句中哪些是正确的，哪些是错误的，并说明原因。
 *********************************************************/
void ch6_1()
{
     printf("ch6_1(),判断正误，见注释部分.\n"); 
     // (1) 
     int n=10,a1[n]; // 正确，C语言在编译阶段给数组开辟空间，如果n也初始化，则正确。
     // int n,a2[n]; // 错误，n没有初始化，编译系统不知道应该给数组开辟多大的空间 
     
     // (2)
     #define MAX 512
     char a4[MAX*2]; // 正确
     
     // (3)
     int a5[5],b[5];
     // scanf("%d",&a5); // 错误，只能通过循环语句给每个数组元素输入值，不能一次性给整个数组输入值 
     // b=a5;  // 错误，数组名表示编译时给数组分配内存的地址常量，因此数组名不能用作赋值语句的左端。
     
     // (4)
     int a6[]={0}; // 正确，该数组被初始化为1个元素，即a[0]=0 
}


/********************************************************
* ch6,P125, 2. 从键盘输入10个整数，用冒泡排序法将其按递减次序排列并输出。 
*********************************************************/
void ch6_2()
{
    printf("ch6_2(), 冒泡排序法，递减排序。\n");
       
    int i,j,t;
    int N = 10; // 数据个数 
	int a[N];   // 需排序的数据 
	int flag=0; // 0:需要继续下一趟比较，1：不需下一趟比较
	
	printf("请输入10个整数，然后递减排序输出。\n");
	for(i=0;i<N;i++) { scanf("%d",&a[i]); } 

	printf("待排序数据：");
	for (i=0;i<N;i++) { printf("%4d",a[i]); } printf("\n");

	for (j=1;j<=N-1;j++){   // N-1趟排序 
		printf("No. j=%d\n",j);
		flag = 1;
		for (i=0;i<N-j;i++) {  // 相邻元素两两比较 
			//if (a[i]>a[i+1]){    // 由小到大排序 
			if (a[i]<a[i+1]){  // 由大到小排序 
				t=a[i];
				a[i]=a[i+1];
				a[i+1]=t;
				flag = 0;
			}
		}
        
        // 本趟排序输出 
		for (i=0;i<N;i++) { printf("%4d",a[i]); } printf("\n");

		if (flag == 1) { printf("不需下一趟比较了!!\n"); break;  } // 不需下一趟比较了 
	}
	
	printf("排序后：");
	for (i=0;i<N;i++) { printf("%4d",a[i]); } printf("\n");	
}

/********************************************************
* ch6,P125, 3. 分别求出一个4*4矩阵的两条对角元素之和。
*  分析：左对角线元素，下标相等，i==j
*        右对角线元素，下标之和为3，i+j=3 
*********************************************************/
void ch6_3()
{
    printf("ch6_3(), 4*4矩阵的两条对角元素之和。\n");
    int a[4][4] = { { 1, 2, 3, 4},
                    { 5, 6, 7, 8},
                    { 9,10,11,12},
                    {13,14,15,16} };
    long int LeftSum = 0L,RightSum = 0L; // 左、右对角线之和
    int i,j;
    
    for(i=0;i<4;i++)
    {
     for(j=0;j<4;j++) 
     {
         if (i==j) {
            LeftSum += a[i][j]; // 做对角线之和，00,11,22,33
            // 验证 
            printf("左对角线:Left a[%d][%d]=%d\n",i,j,a[i][j]);
         } 
         if (i+j == 3) {
            // 验证 
            RightSum += a[i][j]; // 右对角线之和，03,12,21,30
            printf("右对角线:Left a[%d][%d]=%d\n",i,j,a[i][j]);
         }
     }
    }
    printf("左对角线之和=%ld,右对角线之和=%ld\n",LeftSum,RightSum); 
} 

/********************************************************
* ch6,P125, 4. 若数组a包含10个整型元素，将a中所有的后项除以前项的商取整后，存入数组b中，
*              并按每行3个元素的形式输出。
* double ceil(double x); 计算不小于x的最小整数。 向x轴的右边取值, 上取整 
* double floor(double x); 计算不大于x的最大整数。向x轴的左边取值，下取整 
* 四舍五入取整：获取浮点数x最接近的整数：x < 0.0 ? ceil(x-0.5) : floor(x + 0.5) 
* double round(double x): 四舍五入取整，有些编译器没有此函数，使用上面表达式。
*
printf("floor:%lf,%lf,%lf,%lf\n",floor(3.14),floor(9.9),floor(-3.14),floor(-9.9)); 
printf("ceil:%lf,%lf,%lf,%lf\n",ceil(3.14),ceil(9.9),ceil(-3.14),ceil(-9.9)); 
printf("round:%lf,%lf,%lf,%lf\n",round(3.14),round(9.9),round(-3.14),round(-9.9)); 
floor:3.000000,9.000000,-4.000000,-10.000000
ceil:4.000000,10.000000,-3.000000,-9.000000
round:3.000000,10.000000,-3.000000,-10.000000
*********************************************************/
void ch6_4()
{
    printf("ch6_4(), 数组操作。\n"); 
    int a[10] = {-1,2,3,4,5,6,7,8,9,-10}; 
    int i,b[9];
    float f;
    
    for(i=0;i<9;i++) 
    {
       f = (float)a[i]/a[i+1];  // 注意不同于： (float)(a[i]/a[i+1])
       b[i] = f < 0.0 ? ceil(f-0.5) : floor(f + 0.5); // 四舍五入取整 
       printf("b[%d]=%d ",i,b[i]);
       if((i+1)%3==0) printf("\n"); 
    }
    printf("\n");
} 

/********************************************************
* ch6,P125, 5. 打印杨辉三角形(要求打印出10行)
  1
  1  1
  1  2  1
  1  3  3  1
  1  4  6  4  1
  1  5  10 10 5 1
  ... 
  分析： 
  （1）每一行有数字的个数（列数）=该行的行数，因此用a[10][10]可以表示10行的所有数据。
  （2）每一行开始和结尾均为1，从第三行开始，其它元素为上一行相邻两列之和。
  （3）用双重循环填充a[10][10],外层是行循环，内层是列循环 
*********************************************************/
void ch6_5()
{
     printf("ch6_5(), 杨辉三角形。\n"); 
     int i,j;
     int a[10][10];
      
     for(i=0;i<10;i++) { // 行 
      for(j=0;j<i+1;j++) { // 每一行有数字的个数(列数)=该行的行数(i+1) 
          // 每一行开始和结尾均为1
          if (j==0 || j == i) { 
             a[i][j] = 1;
             continue; 
          }
          
          // 第一、二行的数据填充在上列条件完成，不用继续。 
          if (i <= 1) continue;
           
          // 从第三行开始，其它元素为上一行相邻两列之和。开始和结尾，已经排除。 
          a[i][j] = a[i-1][j-1] + a[i-1][j]; 
       } 
      }
      
      // 输出 
      for(i=0;i<10;i++) {
        for(j=0;j<i+1;j++) {
          printf("%4d",a[i][j]);
        } 
        printf("\n");
      }
     
}

/********************************************************
* ch6,P126, 6. 输入一个字符串，将其中的字符逆置后输出。  
* 分析： gets(字符数组)函数接收一个字符串，字符串中可以有空格，结尾自动追加'\0' 
*        使用循环语句，前后交换各个字符 
*********************************************************/
void ch6_6()
{
    printf("ch6_6(), 字符串逆置输出。\n");
       
	int i,len;
	char s[80],tmp;
	
    printf("请输入字符串(长度小于80)，然后逆置输出。\n");
    gets(s);
    // 逆置 
	len=strlen(s); // 不包含最后的'\0' 
	for(i=0;i<len/2;i++)
	{
		tmp=s[i];
		s[i]=s[len-1-i];
		s[len-1-i]=tmp;
	}
	// 输出验证
    printf("逆置后的字符串：%s\n",s); 
}

/********************************************************
* ch6,P126, 7. 从键盘输入字符串a和b，要求不要用库函数strcat
*  把串b的前5个字符连接到串a中；如果b的长度小于5，则把b的所有元素都连接到a中。 
*********************************************************/
void ch6_7()
{
    printf("ch6_7(), 字符串连接。\n");
    char a[80],b[80];
    int i,len;
     
    printf("输入字符串a:\n");
    gets(a);
    printf("输入字符串b:\n");
    gets(b);
    
    // 求a的长度，不用库函数strlen(char *s)
    len = 0; 
    while(a[len] != '\0') len++; 
    
    // 在a的末尾连接a的前5个字符。 
    i = 0;
    while(b[i] != '\0') {
       if (i > 4) break; // 仅连接b的前5个字符
       a[len+i]=b[i];
       i++; 
    }
    a[len+i] = '\0'; // 不要忘记，a的末尾一定是'\0' 
    
    // 输出a，验证 
    puts("连接后的a:");
    puts(a);  
} 

/****************************
 * 用于字符串比较的冒泡排序函数
 * 参数,n个字符串 
 ****************************/ 
void string_order(char s[][80],int n)
{
    int i,j;
    char t[80];  
	int flag=0; // 0:需要继续下一趟比较，1：不需下一趟比较

	for (j=1;j<=n-1;j++){   // N-1趟排序 
		// printf("No. j=%d\n",j);
		flag = 1;
		for (i=0;i<n-j;i++) {  // 相邻元素两两比较 
			if (strcmp(s[i],s[i+1]) > 0){    // 由小到大排序 
			//if (strcmp(s[i],s[i+1]) < 0){  // 由大到小排序 
				strcpy(t,s[i]);
				strcpy(s[i],s[i+1]);
				strcpy(s[i+1],t);
				flag = 0;
			}
		}
        
        // 本趟排序输出 
		// for (i=0;i<n;i++) { printf("%s\n",s[i]); }

		if (flag == 1) { printf("不需下一趟比较了!!\n"); break;  } // 不需下一趟比较了 
	}
	
	// printf("排序后：\n");
	// for (i=0;i<n;i++) { printf("%s\n",s[i]); }	
} 

/********************************************************
* ch6,P126, 8. 输入n个字符串，将它们按字母大小的顺序排列并输出。
* 分析：二维数组存储n个字符串，使用冒泡排序方法进行排序。 
*********************************************************/
void ch6_8()
{
    printf("ch6_8(), 5个字符串排序。\n");
    char s[5][80]; // 以5个字符串为例
    int i;
    
    i = 0;
    do { 
       printf("输入字符串[%d]\n",i+1);
       gets(s[i]); 
    }while(i++ < 4);

    string_order(s,5);
    
    puts("排序后：");    
    i = 0;
    do { 
       puts(s[i]); 
    }while(i++ < 4);
}

/********************************************************
* ch6,P126, 9. 输入一段正文，并统计其中的某个单词出现的次数。
* 分析：以空格分割单词，计为当前单词nowStr，flag：表示有无nowStr；统计单词: countStr 
*       遍历正文： 
            遇到空格，根据flag判断有无nowStr,如果有，nowStr末尾置'\0'; 比较，计数countStr。flag=0; nowStr下标置0 
            未遇到空格，flag=1,记录nowStr中的字符            
*********************************************************/
void ch6_9()
{
    printf("ch6_9(), hello单词出现的次数。\n");
    char str[80]; // 输入的字符串 
    char countStr[] = "hello"; // 统计该单词出现的次数
    char nowStr[20]; // 当前单词
    int flag = 0;    // 0：没有当前单词；1：有当前单词  
    int i,j;         // 分别表示str和nowStr的下标 
    int num = 0;     // 统计次数 
         
    puts("请输入一段文字,长度小于80，统计hello单词出现的次数。\n"); 
    gets(str);
    
    i = 0; j = 0;  flag = 0; 
    while(str[i] != '\0')
    {
        if (str[i]== ' ') {  // 遇到空格，判断有无当前单词 
          i++;
          if (flag == 1) {  // 有当前单词 
            nowStr[j] = '\0';
            // 比较，计数 
            if (strcmp(nowStr,countStr) == 0) num++;
            printf("当前单词：%s\n",nowStr); 
          }
          flag = 0;  // 置为没有当前单词 
          j = 0;     // 为将来的当前单词下标复位 
          continue; 
        }
        
        // 记录新单词 
        flag = 1;  // 有当前单词 
        nowStr[j++]=str[i]; 
        i++;
    }
    
    // 有可能最后一个单词是countStr,但是由于最后没有遇到空格，在上述没有处理。
    if (flag == 1) {  // 有当前单词 
        nowStr[j] = '\0';
        // 比较，计数 
        if (strcmp(nowStr,countStr) == 0) num++;
        printf("当前单词：%s\n",nowStr); 
    }
     
    
    printf("\"%s\"出现的次数：%d\n",countStr,num); 
} 

/********************************************************
* ch6,P126, 10. 有一行电文，已按下面规律译成密码：
    A->Z  a->z 
    B->Y  b->Y
    C->X  c->x
    即第1个字母变成第26个字母，第i个字母变成第(26-i+1)个字母。非字母字符不变。
    要求编程将密码译回原文，并打印输出密码和源码。 
*********************************************************/
void ch6_10()
{
    printf("ch6_10(), 电文译码。\n");
    char A_code[26],a_code[26]; // 大小写字母的原码-密码对照表，
                                // A_code[大写字母原码 - 'A'] = 密码
                                // a_code[小写写字母原码 - 'A'] = 密码 
    char s1[80],s2[80]; // 电文,译文 
    char c1,c2;
    int i;
    
    puts("原码->密码对照表");
    for(i=1,c1 = 'A', c2 = 'a'; c1 <= 'Z'; i++,c1++,c2++) {
      A_code[i-1] = 'A'+26-i;
      a_code[i-1] = 'a'+26-i;
      printf("%c->%c\t%c->%c\n",c1,'A'+26-i,c2,'a'+26-i);
    }
    
    puts("请输入一行电文，将译为密文。");
    gets(s1);
    
    for(i=0;(c1=s1[i])!='\0'; i++)
    {
       if (c1>='A' && c1 <= 'Z') s2[i] = A_code[c1-'A'];  
       else if (c1>='a' && c1 <= 'z') s2[i] = a_code[c1-'a']; 
       else s2[i] = c1; 
    }
    s2[i] = '\0'; // 不要忘了字符串末尾是'\0' 
    puts("密文如下：");
    puts(s2);
    
    // 再由密文译为原文
    char s3[80], j;
    puts("对应的原文如下：");
    for(i=0;(c1=s2[i])!='\0'; i++)
    {
       if (c1>='A' && c1 <= 'Z') {
           for(j=0;;j++) if (A_code[j] == c1) { s3[i] = j+'A'; break; } 
       }  
       else if (c1>='a' && c1 <= 'z') {
           for(j=0;;j++) if (a_code[j] == c1) { s3[i] = j+'a'; break; } 
       }  
       else s3[i] = c1; 
    }
    s3[i] = '\0'; // 不要忘了字符串末尾是'\0' 
    puts(s3); 
}

void ch6()
{
	printf("=======ch6=====\n");
	ch6_1(); // 判断正误
	ch6_2(); // 冒泡排序法，递减排序
	ch6_3(); // 4*4矩阵的两条对角元素之和
	ch6_4(); // 数组操作
	ch6_5(); // 杨辉三角形
	ch6_6(); // 字符串逆置输出
    ch6_7(); // 字符串连接
    ch6_8(); // 5个字符串排序 
    ch6_9(); // hello单词出现的次数
    ch6_10(); // 电文译码
}

