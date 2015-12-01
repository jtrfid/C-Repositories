//#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>

/*******************************************
 * 通用函数，m*n矩阵打印输出(m行，n列)，m=1,即为一维数组
 * 实参：二维数组，形参一维数组 
 * 方法一：用两重循环遍历二维数组。 
 * int matrix1[3][3] = {{1,2,3},{4,5,6},{7,8,9}};
 * //matrixPrint(matrix1,3,3);   // [Warning] passing arg 1 of `matrixPrint' from incompatible pointer type  
 * //matrixPrint(matrix1[],3,3); // [Warning] passing arg 1 of `matrixPrint' from incompatible pointer type
 * matrixPrint(&matrix1[0][0],3,3); // ok
 * // matrixPrint(*matrix1,3,3);    // ok
 *******************************************/
void matrixPrint1(int a[],int m,int n)    // 参数如果定义为二维数组，必须以下标方式指明行数、列数，如a[3][3]或a[][3]不方便 
// void  matrixPrint(int *a,int m,int n) // 等效 
{
     int i,j;
     for(i=0;i<m;i++)
     {
       for(j=0;j<n;j++)  
       {
          // 由于参数说明为一维数组，因此下面语句会出错。 
          //printf("%4d",a[i][j]); // Error: subscripted value is neither array nor pointer 
          
          printf("%4d",a[i*n+j]); // 一维数组与二维数组的关系，a[i*n+j]  --> a[i][j]
       }
       putchar('\n');
     }
}

/*******************************************
 * 通用函数，m*n矩阵打印输出(m行，n列)，m=1,即为一维数组 
 * 实参：二维数组，形参一维数组 
 * 方法二：用指针遍历 
 * int matrix1[3][3] = {{1,2,3},{4,5,6},{7,8,9}};
 * //matrixPrint(matrix1,3,3);   // [Warning] passing arg 1 of `matrixPrint' from incompatible pointer type  
 * //matrixPrint(matrix1[],3,3); // [Warning] passing arg 1 of `matrixPrint' from incompatible pointer type
 * matrixPrint(&matrix1[0][0],3,3); // ok
 * // matrixPrint(*matrix1,3,3);    // ok
 *******************************************/
void matrixPrint(int a[],int m,int n)    // 参数如果定义为二维数组，必须以下标方式指明行数、列数，如a[3][3]或a[][3]不方便 
// void  matrixPrint(int *a,int m,int n) // 等效 
{
     int i,j;
     int *p = a; // 指向数组第一个元素，或者：p=&a[0]; 
     for(i=0;i<m;i++)
     {
       for(j=0;j<n;j++)  
       {
          // 由于参数说明为一维数组，因此下面语句会出错。 
          //printf("%4d",a[i][j]); // Error: subscripted value is neither array nor pointer 
          
          printf("%4d",*p++); // 先使用p，即先取得p指向的内容，再p自增，指向下一个元素 
       }
       putchar('\n');
     }
}  

/*******************************************
 * 通用函数，n*n矩阵转置，即n*n方阵转置
 * 实参：二维数组，形参一维数组  
 * int matrix1[3][3] = {{1,2,3},{4,5,6},{7,8,9}};
 * //NmatrixTranspose(matrix1,3);   // [Warning] passing arg 1 of `matrixPrint' from incompatible pointer type  
 * //NmatrixTranspose(matrix1[],3); // [Warning] passing arg 1 of `matrixPrint' from incompatible pointer type
 * NmatrixTranspose(&matrix1[0][0],3); // ok
 * // NmatrixTranspose(*matrix1,3);    // ok
 *******************************************/
void NmatrixTranspose(int a[],int n)  // 适合于方阵 
{
     int i,j,tmp;
     for(i=0;i<n;i++)
     {
       for(j=i;j<n;j++) // 从第i列开始，该行此前的列在上一行的遍历过程中已经替换 
       {
          /**** 二维数组，行->列，列->行 
          tmp = a[i][j];
          a[i][j] = a[j][i];
          a[j][i] = tmp;
          *****/
          // 由于参数说明为一维数组，用下列3行语句替换上述三句即可。 
          tmp = a[i*n+j];
          a[i*n+j] = a[j*n+i];
          a[j*n+i] = tmp;
       }
     }
} 

/*******************************************
 * 通用函数，由m*n矩阵a[m][n]，转置生成n*m的矩阵b[n][m]
 * 实参：二维数组，形参一维数组  
 * int a[3][2] = {{1,2},{3,4},{5,6}};
 * int b[2][3]
 * matrixTranspose(&a[0][0],3,2,&b[0][0]); // ok
 * // matrixTranspose(*a,3,2,*b);    // ok
 *******************************************/
void MNmatrixTranspose(int a[],int m,int n,int b[])  
{
     int i,j;
     for(i=0;i<m;i++)
     {
       for(j=0;j<n;j++) 
       {
          // 二维数组，行->列，列->行 
          //b[j][i] = a[i][j]
          
          // 由于参数说明为一维数组，用下列语句替换上述语句 
          b[j*m+i] = a[i*n+j];
       }
     }
} 

/***************************************
 * ch8,p192, 1.写一函数，求一个字符串的长度。 
 * 注意，参数s是字符串，因此，必须以'\0'结尾。
 * 不使用库函数 
 ***************************************/
int ch8_1(char *s)
//int ch8_1(char s[]) // 等效 
{
      int len = 0;
      // 指针运算,由地址求内容'*'和'++'优先级形同（2），自右先做结合，即右结合性
      // 因此，*s++中的s，首先与'++'结合，后与'*'取地址内容结合，相当于*(s++) 
      while(*s++ != '\0') len++;
      return len; 
}

/***************************************
 * ch8,p192, 2.有一个字符串，包含n个字符。
 * 编写一个函数，将此字符串从m个字符开始的全部字符复制成一个字符串。 
 * 注意，两个参数是字符串，因此，必须以'\0'结尾。
 * s1: 输入字符串，s2：生成新的字符串，m：复制字符的个数
 * 从s1复制m个字符至s2 
 ***************************************/
void ch8_2(char *s1, char *s2, int m)
{
      int i; // 复制字符个数 
      for (i=0; (*s1 != '\0') && ( i < m); s1++,s2++,i++)
      {
          *s2 = *s1;
      }
      // 别忘了，'\0' 
      *s2 = '\0';
}

/******************************************************
 * ch8,p192, 3. 编写一个函数，由实参传来一个字符串，统计此字符串中的字母，数字，空格和其它字符的个数，并输出。
 * ch7,p155, 第5题用字符数组实现。
 *  指针参数：Letter，Number, Space, Other，传递字母、数字、空格和其它字符的个数 
 ******************************************************/ 
void ch8_3(char *s, int *Letter, int *Number, int *Space, int *Other)
{    
     char c;
     *Letter = 0, *Number = 0, *Space = 0 , *Other = 0; 
     while((c = *s++) != 0){
         if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) (*Letter)++;
         else if ('0' <= c && c <= '9') (*Number)++;
         else if (c == ' ') (*Space)++;
         else (*Other)++;
     } 
} 

/*********************************************************
 * ch8,p192, 4. 编写一个函数将3*3的矩阵转置。
 * 参数 a[][]： 传递转置前后方阵
 *      n: 行数或列数，对于方阵是相同的
 * 方法1，本函数，与方法2的的区别是，参数a描述为二维数组，必须以下标方式指明行数和列数，如，int a[3][3]或a[][3] 
 * 方法2，void NmatrixTranspose(int a[],int n),以一维数组描述，不用定义时指定行数和列数 
 *********************************************************/
void ch8_4(int a[3][3],int n)
{
     int i,j,tmp;
     for(i=0;i<n;i++)
     {
       for(j=i;j<n;j++)
       {
          tmp = a[j][i];
          a[j][i] = a[i][j];
          a[i][j] = tmp;  
       }
     }
     
    printf("-----\n");
    for(i=0;i<3;i++)
    {
     for(j=0;j<3;j++) printf("%3d",a[i][j]);
     printf("\n");
    }
} 

/*********************************************************
 * ch8,p192, 5. 将一个5*5的矩阵中最大的元素放在中心，四角分别放四个最小元素（按从左到右，从上到下的顺序依次从小到大存放），
 * 编写一函数实现之。 
 * 首先编写一个排序函数，根据排序后的数组实现本题。 
 *********************************************************/

/********************************** 
 * 冒泡排序函数 
 * 输入a数组，返回排序后的数组
 * N：数组元素个数，
 * ASC: 1,小->大,升序;  0, 大->小,降序 
 ***********************************/
void order(int a[],int N,int ASC)
{      
    int i,j,t;
	int flag=0; // 0:需要继续下一趟比较，1：不需下一趟比较
	
	printf("待排序数据：");
	for (i=0;i<N;i++) { printf("%4d",a[i]); } printf("\n");

	for (j=1;j<=N-1;j++){   // N-1趟排序 
		flag = 1;
		for (i=0;i<N-j;i++) {  // 相邻元素两两比较
            if ((ASC==1 && a[i]>a[i+1]) || (ASC==0 && a[i]<a[i+1])) { // 由小到大排序，升序  || 由大到小排序，降序 
				t=a[i];
				a[i]=a[i+1];
				a[i+1]=t;
				flag = 0;
			}
		}
		if (flag == 1) { printf("不需下一趟比较了!!\n"); break;  } // 不需下一趟比较了 
	}
	
	printf("排序后：");
	for (i=0;i<N;i++) { printf("%4d",a[i]); } printf("\n");	
}

void ch8_5()
{
     int a[5][5] ={{1,2,3,4,500},{6,7,8,9,10},{11,12,13,88,15},{16,17,18,19,20},{21,22,23,24,25}};
     static int b[5][5]; // 目标矩阵 
     
     // 升序排列a，小->大 
     order(a[0],25,1); // ok,实参是二维数组，形参是一维数组，传递数组第一个元素地址即可 
     // order(a,25,1);  // gcc: [Warning] passing arg 1 of `order' from incompatible pointer type。vc++编译系统认为错误。 
     // order(&a[0][0],25,1); // ok 
     // order(*a,25,1); // ok
     
     // b的中心放置最大的元素，即a的最后一个元素 
     b[2][2] = a[4][4];
     
     // b的四角放置最小的四个元素，即a的前四个元素 
     b[0][0] = a[0][0]; b[0][4] = a[0][1];
     b[4][0] = a[0][2]; b[4][4] = a[0][3];
  
    
     // 放置其他元素，a数组通过指针访问，较方便。 
     int i,j; // i,j表示b的下标
     int *pa = &a[0][4];  // a从第5个元素开始, 相当于int *pa; pa=&a[0][4];  
     
     for(i=0;i<5;i++) { 
       for(j=0;j<5;j++) 
       {
          //b的四角和中心跳过 
          if ((i==0&&j==0) || (i==0&&j==4) || (i==4&&j==0) || (i==4&&j==4) || 
               (i==2&&j==2)) 
          {
               continue;
          }     
          b[i][j] = *pa++;         
       } 
     }
      
    printf("结果矩阵：\n");
    for (i=0;i<5;i++)
    { 
      for(j=0;j<5;j++)
       { printf("%4d",b[i][j]); } 
      printf("\n");
    }	 
}

/******************************************************
 * ch8,p192, 6. 编写程序，实现两个字符串比较的自定义版：
 *    int strcmp(char *str1,char *str2);
 *    // 当str1>str2时，返回正数
 *    // 当str1==str2时，返回0
 *    // 当str1<str2时，返回负数 
 ******************************************************/
 
int ch8_6(const char *str1,const char *str2)
{
    for(;*str1 == *str2; str1++,str2++)
    {
        if (*str1 == '\0') return 0;                       
    } 
    return ((*str1 < *str2) ? -1 : 1);             
} 

/******************************************************
 * ch8,p192, 7. 编写程序，实现复制字符串的自定义版：
 *    char *strcpy(char *dest,char *source);
 *    // 该函数返回dest的值，即字符串首地址 
 ******************************************************/
char* ch8_7(char *dest,const char *source)
{   
    char *s = dest;
    
    while ((*s++ = *source++) != '\0')
      ;
    
    return (dest);  // 此时s指向最后一个字符'\0',因此，不能返回s.          
} 

/*****************************************************
 * ch8,p193, 8. 编写程序，输入命令行参数为两个字符串，
 *    用strcmp()比较并输出结果。
 * 本函数模拟main函数，接收命令行参数，
 * 例如：char *argv[] = {"parameter1","parameter2"};
 *       ch8_8(2,argv);
 *****************************************************/
int ch8_8(int argc, char *argv[])
{
    if (argc >= 2) {
      printf("\"%s\"和\"%s\"比较结果：%d\n",argv[0],argv[1],strcmp(argv[0],argv[1])); 
      return 0; // 表示正常结束 
    }
    return 1; // 表示未按要求执行程序，非正常结束 
}

/*******************************************************
 * ch8, p193, 9. 编写程序：
 * (1)初始化一个矩阵A(5*5),元素值取自随机函数，并输出；
 * (2)将其传递给函数，实现矩阵转置；
 * (3)在主函数中输出结果。 
 *******************************************************/ 
void ch8_9()
{
     //(1)初始化矩阵：
     int a[5][5];
     int i,j;
     
     // void srand(unsigned int seed)是随机数发生器的初始化函数，其参数是发生器的种子，使用系统当前时间作为种子，time(0) 
     // int rand(void)函数返回随机数，范围0~RAND_MAX,至少是双字节,32767。
     // 用"int x = rand() % 100;"来生成 0 到 100 之间的随机数这种方法是不或取的，
     // 比较好的做法是： j=(int)(n*rand()/(RAND_MAX+1.0))产生一个0到n之间的随机数  
     srand(time(0)); 
     for(i=0;i<5;i++)
     {
       for(j=0;j<5;j++)
       {
           a[i][j] = (int) (100.0*rand())/(RAND_MAX+1.0); // 0~100的随机数       
       }
     }
     
     printf("随机5*5矩阵：\n"); 
     matrixPrint(&a[0][0],5,5);    // 打印输出 
     
     // （2）转置 
     printf("转置后：\n"); 
     NmatrixTranspose(&a[0][0],5); // 方阵转置 
     matrixPrint(&a[0][0],5,5);    // 打印输出 
} 

/*******************************************************
 * ch8, p193, 10. 利用指针数组编写程序，输入月份号，输出该月的英文名字。
 *   如，输入10，输出"October" 
 *******************************************************/ 
void ch8_10()
{
     char *month[] ={"January","February","March","Aril","May","June","July",
                     "August","September","October","November","December"};
     
     int m;               
     printf("请输入月份号(1~12),输入0，结束。\n");
     while(1) 
     { 
       scanf("%d",&m); 
       if (m==0) break;
       if (m < 1 || m >12) puts("非法月份号,重新输入。"); 
       puts(month[m-1]); 
     }
} 
  
// 学习指导，p60,指针
void exercise()
{ 
	printf("=========exercise()\n");
	int a[2][3]={{1,2,3},{4,5,6}};
	int m,*p=&a[0][0];
	m=(*p)*(*(p+2))*(*(p+4));  // m= 1*3*5 
	printf("m=%d\n",m); // 15
}

void ch8()
{
	printf("==========ch8()\n");

    /**************************************************************************/
    /*********************通用函数测试*****************************************/
    	
	// 通用函数，m*n矩阵打印输出 
    printf("void matrixPrint(int a[],int m,int n),通用函数,m*n矩阵打印输出.\n");
    int matrix1[3][3] = {{1,2,3},{4,5,6},{7,8,9}};
    printf("-----maxtrix1,3行3列\n"); 
    //matrixPrint(matrix1,3,3); // [Warning] passing arg 1 of `matrixPrint' from incompatible pointer type  
    //matrixPrint(matrix1[],3,3); // [Warning] passing arg 1 of `matrixPrint' from incompatible pointer type
    matrixPrint(&matrix1[0][0],3,3); // ok
    // matrixPrint(*matrix1,3,3);  // ok
    
    printf("-----maxtrix2,3行2列\n");
    int matrix2[3][2] = {{1,2},{3,4},{5,6}}; 
    matrixPrint(&matrix2[0][0],3,2);
    printf("-----maxtrix3,2行3列\n");
    int matrix3[2][3] = {{1,2,3},{4,5,6}}; 
    matrixPrint(&matrix3[0][0],2,3);
    printf("-----maxtrix4,1行3列\n");
    int matrix4[3] = {1,2,3}; 
    matrixPrint(matrix4,1,3);
    
    // 通用函数，n*n矩阵转置
    printf("-----maxtrix1方阵转置\n");
    NmatrixTranspose(&matrix1[0][0],3);
    matrixPrint(&matrix1[0][0],3,3);
    
    // 通用函数，m*n矩阵转置，即n*m矩阵，m行n列数组转为n行m列
    printf("-----maxtrix2转置\n");
    int matrix2_T[2][3]; 
    MNmatrixTranspose(&matrix2[0][0],3,2,*matrix2_T);
    matrixPrint(&matrix2_T[0][0],2,3);
    
    printf("-----maxtrix3转置\n");
    int matrix3_T[3][2]; 
    MNmatrixTranspose(&matrix3[0][0],2,3,*matrix3_T);
    matrixPrint(&matrix3_T[0][0],3,2);
    
    printf("-----maxtrix4转置\n");
    int matrix4_T[3][1]; 
    MNmatrixTranspose(matrix4,1,3,*matrix4_T);
    matrixPrint(&matrix4_T[0][0],3,1); 
    
    /*********************通用函数测试, 结束***********************************/
    /**************************************************************************/
	
	// int ch8_1(char *s)
	printf("int ch8_1(char *s),求字符串的长度。\n");
    printf("\"12345\"字符串长度为：%d\n",ch8_1("12345")); 
    
    // ch8_2(char *s1, char *s2, int m)
    printf("ch8_2(char *s1, char *s2, int m),从s1复制m个字符至s2\n");
    char s1[]="123456",s2[80] = "";  //s2[0]='\0',注意，一定给字符串初始化为以'\0'结束，
    ch8_2(s1,s2,3);
    printf("从%s复制%d个字符至%s\n",s1,3,s2);
    
    // ch8_3(char *s),统计s中字母、数字、空格和其它字符的个数; 
    printf("ch8_3(char *s),统计s中字母、数字、空格和其它字符的个数\n");
    int Letter,Number,Space,Other; 
    ch8_3("123  d1 --- ",&Letter, &Number, &Space, &Other); 
    printf("字母、数字、空格和其它字符的个数=%d,%d,%d,%d\n",Letter,Number,Space,Other); 
    
    /**************************************************/
    // ch8_4(int a[][],int n) 矩阵转置。
    printf("ch8_4(int a[][],int n),矩阵转置。\n");
    int a[3][3] = {{1,2,3},{4,5,6},{7,8,9}}; 
    
    ch8_4(a,3);
    
    int i,j;
    for(i=0;i<3;i++)
    {
     for(j=0;j<3;j++) printf("%3d",a[i][j]);
     printf("\n");
    }
    /*****************************************************/
     
    // ch8_5(),5*5矩阵元素排列
    printf("ch8_5(),5*5矩阵元素排列\n");
    ch8_5(); 
    
    // int ch8_6(char *str1,char *str2),自定义版的strcmp
    printf("int ch8_6(char *str1,char *str2),自定义版的strcmp. \n"); 
    printf("123和1234比较=%d,%d\n",ch8_6("123","1234"),strcmp("123","1234"));
    printf("123和123比较=%d,%d\n",ch8_6("123","123"),strcmp("123","123"));
    printf("12a和123比较=%d,%d\n",ch8_6("12a","123"),strcmp("12a","123"));
    
    // char* ch8_7(char *dest,const char *source)，自定义版的strcpy
    char dest[80]="";
    char source[] = "abcd"; 
    printf("copy %s to dest,%s\n",source,ch8_7(dest,source));
    
    // int ch8_8(int argc, char *argv[]),模拟main函数，接收命令行参数
    printf("int ch8_8(int argc, char *argv[]),模拟main函数，接收命令行参数\n");
    char *argv[] = {"parameter1","parameter2"};
    ch8_8(2,argv);
    
    // void ch8_9(),产生一个随机5*5矩阵，并转置
    printf("void ch8_9(),产生一个随机5*5矩阵，并转置.\n");
    ch8_9();
    
    // void ch8_10(),指针数组测试
    printf("void ch8_10(),指针数组测试.\n");
    ch8_10(); 
		
	// 学习指导，p60,指针
	exercise();

}
