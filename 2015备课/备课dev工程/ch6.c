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

	//b=a[0]; // �޷��ӡ�int [2]��ת��Ϊ��int [4]����ʵ���������������ַ��������˲�������ֵ
	int *p;
	p=a[0]; // ָ��������Ԫ�ص�ָ��

	for (i=0;i<4;i++,p++) 
		printf("%d ",*p); // 1 2 3 4
	printf("=====\n");

	for (i=0;i<4;i++)
		printf("%d ",b[i]); // 1 2 3 4

	printf("=====\n");
	// &ȡ��ַ�������ֻ�������ڱ����������������ͱ����������Ԫ�ء��ṹ�����ͱ�����ṹ��ĳ�Ա������������������������Ĵ�������
	// �������&a���Ϸ������ǲ�֪Ϊ�Σ��ܱ�������
	printf("%0x,%0x,%0x,%0x,%0x,%0x\n",a[0],&a,&a[0][0],&a[0],a,a[0][0]); // 15f0bc,15f0bc,15f0bc,15f0bc,15f0bc,1
	printf("%0x,%p,%p,%p,%0x,%0x\n",a[0],&a,&a[0][0],&a[0],a,a[0][0]);    // 15f0bc,0015F0BC,0015F0BC,0015F0BC,15f0bc,1
	
}

// ��������Ԫ�صĺ�
int add_ave(int m,int n,int arr[])
{
	int i,total=0;
	for (i=0;i<m*n;i++){
		total = total+arr[i];
	}
	return (total);
}

// ��ӡ����ƽ��ֵ��Ԫ��,m*n����,m�У�n��
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

// ��ӡ����ƽ��ֵ��Ԫ��,m*n���飬m�У�n��=4
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
	sum = add_ave(i,j,A[0]); // A[0],ָ���ά���������
	//sum = add_ave(i,j,A);  // ���ܽ����� 3 �ӡ�int [3][4]��ת��Ϊ��int []��

	printf("The sum of array=%d\n",sum);
	ave=(float)(sum/(i*j)); 
	printf("The average of array=%5.2f\n",ave);
	//part_up(i,j,ave,A[0]);   // ok��A[0],ָ���ά���������
	//part_up(i,j,ave,&A[0][0]); // ok��ָ���ά���������
	part_up(i,j,ave,*A);       // ok��ָ���ά���������
	//part_up(i,j,ave,A);  // ���ܽ����� 3 �ӡ�int [3][4]��ת��Ϊ��int []��

	part_up1(i,j,ave,A); //ok
}

void str()
{
	printf("======str()\n");
	char str1[20],str2[20] = {"Hello!"};  // ok
	//str1={"Hello!"};    // error
	//str2=str1;          // error �����������Ϊ��ֵ,str1,str2�ֱ�������������׵�ַ��������˳������ܳ�������ֵ
	// ֻ���� 
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
	printf("s2====%s\n",s2); // s2==== �������񣨲������ã���б��
	printf("s3====%s\n",s3); // s3====i (0x69=105��ASCII) ��\082����Ϊ��3λ8���ƣ�����\0�����ַ������� 
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
	int flag=0; // 0:��Ҫ������һ�˱Ƚϣ�1��������һ�˱Ƚ�

	printf("���������ݣ�");
	for (i=0;i<N;i++) { printf("%2d\t",a[i]); } printf("\n");

	for (j=1;j<=N-1;j++){
		printf("No. j=%d\n",j);
		flag = 1;
		for (i=0;i<N-j;i++) {
			printf("i=%2d\t",i);
			if (a[i]>a[i+1]){    //����
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
	printf("�����");
	for (i=0;i<N;i++) { printf("%2d\t",a[i]); } printf("\n");
}

void testString()
{
	printf("===========testString()\n");
	char s[]={'a','b'}; // �����Զ�s[2]='\0'
    printf("s[0]=%c,%d\n",s[0],s[0]);  // a,97
	printf("s[1]=%c,%d\n",s[1],s[1]);  // b,98
	printf("s[2]=%c,%d\n",s[2],s[2]);  // �����
	printf("%s\n",s);  // s[2]��ȷ��
	char s1[3]={'a','b'}; // �Զ�s[2]='\0'
	printf("s1[0]=%c,%d\n",s1[0],s1[0]);  // a,97
	printf("s1[1]=%c,%d\n",s1[1],s1[1]);  // b,98
	printf("s1[2]=%c,%d\n",s1[2],s1[2]);  // a,0  , ascii 0 �ǲ��ɴ�ӡ�ַ��������aӦ���������
	printf("%s",s1);
}

void p119() 
{
    #define  N  10
    { 
     int array[N] = {4,6,7,10,13,2,1,9,20,3};
     int i,j,k,t;
     
     printf("����ǰ: \n");
     for (i=0; i<N; i++)
        printf("%4d",array[i]);
     printf("\n"); 
     
     for (i=0; i<N-1; i++)  /*��ѭ�����ƱȽϵ����� */
     { 
         k=i;
         for(j=i+1;j<N;j++)  /*��ѭ������k��ס����������С�����±�*/
           if (array[j]<array[k]) k=j;
         if(i!=k)
         { 
           t=array[i];
           array[i]=array[k];
           array[k]=t; 
           printf("��%d�к͵�%d�н�������С����:%d,",i+1,k+1,array[i]);
         }
         // �����i+1�˱ȽϺ��������
         printf("��%d�˱ȽϺ�\n",i+1); 
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
 ����ǰ:
   4   6   7  10  13   2   1   9  20   3
��1�к͵�7�н�������С����:1,��1�˱ȽϺ�
   1   6   7  10  13   2   4   9  20   3
��2�к͵�6�н�������С����:2,��2�˱ȽϺ�
   1   2   7  10  13   6   4   9  20   3
��3�к͵�10�н�������С����:3,��3�˱ȽϺ�
   1   2   3  10  13   6   4   9  20   7
��4�к͵�7�н�������С����:4,��4�˱ȽϺ�
   1   2   3   4  13   6  10   9  20   7
��5�к͵�6�н�������С����:6,��5�˱ȽϺ�
   1   2   3   4   6  13  10   9  20   7
��6�к͵�10�н�������С����:7,��6�˱ȽϺ�
   1   2   3   4   6   7  10   9  20  13
��7�к͵�8�н�������С����:9,��7�˱ȽϺ�
   1   2   3   4   6   7   9  10  20  13
��8�˱ȽϺ�
   1   2   3   4   6   7   9  10  20  13
��9�к͵�10�н�������С����:13,��9�˱ȽϺ�
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

