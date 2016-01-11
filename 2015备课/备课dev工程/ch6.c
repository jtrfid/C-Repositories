//#include "stdafx.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void arrayAddress()
{
	printf("===arrayAddress()\n");
	int a[2][2]={{1,2},{3,4}};
    int i,j,b[4];
	for (i=0;i<2;i++) {
		for (j=0;j<2;j++) {
			printf("%d  ",a[i][j]);
			b[i*2+j]=a[i][j];
		}
		printf("\n");
	}
	// 1 2
	// 3 4
	printf("=====\n");

	for (i=0;i<2*2;i++)
		printf("a[%d]= %d ",i,*(a[i]));
	printf("======\n");

	//b=a[0]; // 无法从“int [2]”转换为“int [4]”。实际上数组名代表地址常量，因此不能是左值
	int *p;
	p=a[0]; // 指向数组列元素的指针

	for (i=0;i<4;i++,p++) 
		printf("%d ",*p); // 1 2 3 4
	printf("=====\n");

	for (i=0;i<4;i++)
		printf("%d ",b[i]); // 1 2 3 4

	printf("=====\n");
	// &取地址运算符，只能作用于变量，包括基本类型变量和数组的元素、结构体类型变量或结构体的成员，不能用于数组名、常量或寄存器变量
	// 下列语句&a不合法，但是不知为何，能编译运行
	printf("%0x,%0x,%0x,%0x,%0x,%0x\n",a[0],&a,&a[0][0],&a[0],a,a[0][0]); // 15f0bc,15f0bc,15f0bc,15f0bc,15f0bc,1
	printf("%0x,%p,%p,%p,%0x,%0x\n",a[0],&a,&a[0][0],&a[0],a,a[0][0]);    // 15f0bc,0015F0BC,0015F0BC,0015F0BC,15f0bc,1
	
}

// 所有数组元素的和
int add_ave(int m,int n,int arr[])
{
	int i,total=0;
	for (i=0;i<m*n;i++){
		total = total+arr[i];
	}
	return (total);
}

// 打印大于平均值的元素,m*n数组,m行，n列
void part_up(int m,int n,float average,int arr[])
{
	int i,j;
	printf("The number of Bigger than average are:\n");
	for (i=0;i<m;i++)
	{
		for (j=0;j<n;j++)
		{
			if (arr[i*n+j]>average)
				printf("arr[%d][%d]=%d\t",i,j,arr[i*n+j]);
		}
	}
	printf("\n");
}

// 打印大于平均值的元素,m*n数组，m行，n列=4
void part_up1(int m,int n,float average,int arr[][4])
{
	int i,j;
	printf("The number of Bigger than average are:\n");
	for (i=0;i<m;i++)
	{
		for (j=0;j<n;j++)
		{
			if (arr[i][j]>average)
				printf("arr[%d][%d]=%d\t",i,j,arr[i][j]);
		}
	}
	printf("\n");
}

void sum()
{
	printf("======sum()\n");
	int A[3][4] = {{6,4,9,-13},{-1,3,8,7},{3,4,10,2}};
	int i=3,j=4,sum;
	float ave;
	sum = add_ave(i,j,A[0]); // A[0],指向二维数组的首列
	//sum = add_ave(i,j,A);  // 不能将参数 3 从“int [3][4]”转换为“int []”

	printf("The sum of array=%d\n",sum);
	ave=(float)(sum/(i*j)); 
	printf("The average of array=%5.2f\n",ave);
	//part_up(i,j,ave,A[0]);   // ok，A[0],指向二维数组的首列
	//part_up(i,j,ave,&A[0][0]); // ok，指向二维数组的首列
	part_up(i,j,ave,*A);       // ok，指向二维数组的首列
	//part_up(i,j,ave,A);  // 不能将参数 3 从“int [3][4]”转换为“int []”

	part_up1(i,j,ave,A); //ok
}

void str()
{
	printf("======str()\n");
	char str1[20],str2[20] = {"Hello!"};  // ok
	//str1={"Hello!"};    // error
	//str2=str1;          // error 左操作数必须为左值,str1,str2分别是两个数组的首地址常量，因此常量不能出现在左值
	// 只能用 
	int i;
	for (i=0;str2[i]!='\0';i++)
	{
		printf("%d,%c\t",i,str2[i]); // 0,H     1,e     2,l     3,l     4,o     5,!     
	} 
	printf("%d,%0x\n",i,str2[i]); // 6,0

	strcpy(str2,str1);
	strcpy(str1,"hello!");

	char  s1[10]={'A','\0','B','C','\0','D'};
	char  s2[ ]="\t\v\\\0will\n";
	char  s3[ ]="\x69\082\n";
	printf("s1====%s\n",s1); // s1====A
	printf("s2====%s\n",s2); // s2==== 竖向跳格（不起作用）反斜杠
	printf("s3====%s\n",s3); // s3====i (0x69=105的ASCII) ，\082不认为是3位8进制，而是\0代表字符串结束 
	printf("s1,s2,s3 length=%d,%d,%d\n",strlen(s1),strlen(s2),strlen(s3)); // s1,s2,s3 length=1,3,1
}

void strcmp_test()
{   
	printf("======strcmp_test()\n");
	char str1[] = "Hello!",str2[] = "How are you?",str[20];
	int len1,len2,len3;
	len1=strlen(str1);       len2=strlen(str2);
	if(strcmp(str1, str2)>0)
	{   strcpy(str,str1);      strcat(str,str2);   }      
	else  if (strcmp(str1, str2)<0)
	{   strcpy(str,str2);      strcat(str,str1);   }
	else    strcpy(str,str1);
	len3=strlen(str);
	puts(str); //  How are you?Hello!
	printf("Len1=%d,Len2=%d,Len3=%d\n",len1,len2,len3);  // Len1=6,Len2=12,Len3=18
}

void rand_test()
{
	printf("=====rand_test()\n");
	int i,r;

   /* Seed the random-number generator with current time so that
    * the numbers will be different every time we run.
    */
   srand( (unsigned)time( NULL ) );

   /* Display 0-9 numbers. */
   for( i = 0;   i < 10;i++ ) {
	   r=rand();
	   printf( "  i=%d r=%6d --->%6d\n", i,r,r%(10-i)+i );
   }
  
}

#define N 5
void order()
{
    printf("=====order()\n");
	int i,j,t;
	int a[N]={13,23,1,9,6};
	int flag=0; // 0:需要继续下一趟比较，1：不需下一趟比较

	printf("待排序数据：");
	for (i=0;i<N;i++) { printf("%2d\t",a[i]); } printf("\n");

	for (j=1;j<=N-1;j++){
		printf("No. j=%d\n",j);
		flag = 1;
		for (i=0;i<N-j;i++) {
			printf("i=%2d\t",i);
			if (a[i]>a[i+1]){    //逆序
			//if (a[i]<a[i+1]){
				t=a[i];
				a[i]=a[i+1];
				a[i+1]=t;
				flag = 0;
			}
		}
		
		printf("\n");
		for (i=0;i<N;i++) { printf("%2d\t",a[i]); } printf("\n");

		if (flag == 1) break;
	}
	printf("排序后：");
	for (i=0;i<N;i++) { printf("%2d\t",a[i]); } printf("\n");
}

void testString()
{
	printf("===========testString()\n");
	char s[]={'a','b'}; // 不会自动s[2]='\0'
    printf("s[0]=%c,%d\n",s[0],s[0]);  // a,97
	printf("s[1]=%c,%d\n",s[1],s[1]);  // b,98
	printf("s[2]=%c,%d\n",s[2],s[2]);  // 随机数
	printf("%s\n",s);  // s[2]不确定
	char s1[3]={'a','b'}; // 自动s[2]='\0'
	printf("s1[0]=%c,%d\n",s1[0],s1[0]);  // a,97
	printf("s1[1]=%c,%d\n",s1[1],s1[1]);  // b,98
	printf("s1[2]=%c,%d\n",s1[2],s1[2]);  // a,0  , ascii 0 是不可打印字符，这里的a应该是随机数
	printf("%s",s1);
}

void p119() 
{
    #define  N  10
    { 
     int array[N] = {4,6,7,10,13,2,1,9,20,3};
     int i,j,k,t;
     
     printf("排序前: \n");
     for (i=0; i<N; i++)
        printf("%4d",array[i]);
     printf("\n"); 
     
     for (i=0; i<N-1; i++)  /*外循环控制比较的趟数 */
     { 
         k=i;
         for(j=i+1;j<N;j++)  /*内循环，用k记住所找数中最小数的下标*/
           if (array[j]<array[k]) k=j;
         if(i!=k)
         { 
           t=array[i];
           array[i]=array[k];
           array[k]=t; 
           printf("第%d列和第%d列交换，最小数是:%d,",i+1,k+1,array[i]);
         }
         // 输出第i+1趟比较后的输出结果
         printf("第%d趟比较后：\n",i+1); 
         for (t=0; t<N; t++)
            printf("%4d",array[t]);
         printf("\n"); 
     }
     printf("The sorted numbers: \n");
     for (i=0; i<N; i++)
        printf("%4d",array[i]);
     printf("\n"); 
} 

/************
 排序前:
   4   6   7  10  13   2   1   9  20   3
第1列和第7列交换，最小数是:1,第1趟比较后：
   1   6   7  10  13   2   4   9  20   3
第2列和第6列交换，最小数是:2,第2趟比较后：
   1   2   7  10  13   6   4   9  20   3
第3列和第10列交换，最小数是:3,第3趟比较后：
   1   2   3  10  13   6   4   9  20   7
第4列和第7列交换，最小数是:4,第4趟比较后：
   1   2   3   4  13   6  10   9  20   7
第5列和第6列交换，最小数是:6,第5趟比较后：
   1   2   3   4   6  13  10   9  20   7
第6列和第10列交换，最小数是:7,第6趟比较后：
   1   2   3   4   6   7  10   9  20  13
第7列和第8列交换，最小数是:9,第7趟比较后：
   1   2   3   4   6   7   9  10  20  13
第8趟比较后：
   1   2   3   4   6   7   9  10  20  13
第9列和第10列交换，最小数是:13,第9趟比较后：
   1   2   3   4   6   7   9  10  13  20
The sorted numbers:
   1   2   3   4   6   7   9  10  13  20
 *************/ 

}

void test()
{
     int j,k;
     char c,s1[80]="a-b-e8-9",s2[80];
     
     j=k=0;
     while((c=s1[j++])!='\0')
        if(s1[j]=='-'&&s1[j+1]>=c)
        {
          j++;
          while(c<s1[j]) 
            s2[k++]=c++;
        }
        else
           s2[k++]=c;
     s2[k]='\0';
     printf("%s\n",s2);
} 

void ch6()
{
	printf("=======ch6=====\n");
	test(); 
	//p119();
    /**** 
	arrayAddress();
	sum();
	str();
	strcmp_test();
	rand_test();
	order();
	testString();
	****/
}

