//#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>

/*******************************************
 * ͨ�ú�����m*n�����ӡ���(m�У�n��)��m=1,��Ϊһά���� 
 * int matrix1[3][3] = {{1,2,3},{4,5,6},{7,8,9}};
 * //matrixPrint(matrix1,3,3);   // [Warning] passing arg 1 of `matrixPrint' from incompatible pointer type  
 * //matrixPrint(matrix1[],3,3); // [Warning] passing arg 1 of `matrixPrint' from incompatible pointer type
 * matrixPrint(&matrix1[0][0],3,3); // ok
 * // matrixPrint(*matrix1,3,3);    // ok
 *******************************************/
void matrixPrint(int a[],int m,int n)    // �����������Ϊ��ά���飬�������±귽ʽָ����������������a[3][3]��a[][3]������ 
// void  matrixPrint(int *a,int m,int n) // ��Ч 
{
     int i,j;
     for(i=0;i<m;i++)
     {
       for(j=0;j<n;j++)  
       {
          // ���ڲ���˵��Ϊһά���飬�������������� 
          //printf("%4d",a[i][j]); // Error: subscripted value is neither array nor pointer 
          
          printf("%4d",a[i*n+j]); // һά�������ά����Ĺ�ϵ��a[i*n+j]  --> a[i][j]
       }
       putchar('\n');
     }
} 

/*******************************************
 * ͨ�ú�����n*n����ת�ã���n*n����ת�� 
 * int matrix1[3][3] = {{1,2,3},{4,5,6},{7,8,9}};
 * //NmatrixTranspose(matrix1,3);   // [Warning] passing arg 1 of `matrixPrint' from incompatible pointer type  
 * //NmatrixTranspose(matrix1[],3); // [Warning] passing arg 1 of `matrixPrint' from incompatible pointer type
 * NmatrixTranspose(&matrix1[0][0],3); // ok
 * // NmatrixTranspose(*matrix1,3);    // ok
 *******************************************/
void NmatrixTranspose(int a[],int n)  // �ʺ��ڷ��� 
{
     int i,j,tmp;
     for(i=0;i<n;i++)
     {
       for(j=i;j<n;j++) // �ӵ�i�п�ʼ�����д�ǰ��������һ�еı����������Ѿ��滻 
       {
          /**** ��ά���飬��->�У���->�� 
          tmp = a[i][j];
          a[i][j] = a[j][i];
          a[j][i] = tmp;
          *****/
          // ���ڲ���˵��Ϊһά���飬������3������滻�������伴�ɡ� 
          tmp = a[i*n+j];
          a[i*n+j] = a[j*n+i];
          a[j*n+i] = tmp;
       }
     }
} 

/*******************************************
 * ͨ�ú�������m*n����a[m][n]��ת������n*m�ľ���b[n][m] 
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
          // ��ά���飬��->�У���->�� 
          //b[j][i] = a[i][j]
          
          // ���ڲ���˵��Ϊһά���飬����������滻������� 
          b[j*m+i] = a[i*n+j];
       }
     }
} 

/***************************************
 * ch8,p192, 1.дһ��������һ���ַ����ĳ��ȡ� 
 * ע�⣬����s���ַ�������ˣ�������'\0'��β��
 * ��ʹ�ÿ⺯�� 
 ***************************************/
int ch8_1(char *s)
//int ch8_1(char s[]) // ��Ч 
{
      int len = 0;
      // ָ������,�ɵ�ַ������'*'��'++'���ȼ���ͬ��2��������������ϣ����ҽ����
      // ��ˣ�*s++�е�s��������'++'��ϣ�����'*'ȡ��ַ���ݽ�ϣ��൱��*(s++) 
      while(*s++ != '\0') len++;
      return len; 
}

/***************************************
 * ch8,p192, 2.��һ���ַ���������n���ַ���
 * ��дһ�������������ַ�����m���ַ���ʼ��ȫ���ַ����Ƴ�һ���ַ����� 
 * ע�⣬�����������ַ�������ˣ�������'\0'��β��
 * s1: �����ַ�����s2�������µ��ַ�����m�������ַ��ĸ���
 * ��s1����m���ַ���s2 
 ***************************************/
void ch8_2(char *s1, char *s2, int m)
{
      int i; // �����ַ����� 
      for (i=0; (*s1 != '\0') && ( i < m); s1++,s2++,i++)
      {
          *s2 = *s1;
      }
      // �����ˣ�'\0' 
      *s2 = '\0';
}

/******************************************************
 * ch8,p192, 3. ��дһ����������ʵ�δ���һ���ַ�����ͳ�ƴ��ַ����е���ĸ�����֣��ո�������ַ��ĸ������������
 * ch7,p155, ��5�����ַ�����ʵ�֡�
 *  ָ�������Letter��Number, Space, Other��������ĸ�����֡��ո�������ַ��ĸ��� 
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
 * ch8,p192, 4. ��дһ��������3*3�ľ���ת�á�
 * ���� a[][]�� ����ת��ǰ����
 *      n: ���������������ڷ�������ͬ��
 * ����1�����������뷽��2�ĵ������ǣ�����a����Ϊ��ά���飬�������±귽ʽָ���������������磬int a[3][3]��a[][3] 
 * ����2��void NmatrixTranspose(int a[],int n),��һά�������������ö���ʱָ������������ 
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
 * ch8,p192, 5. ��һ��5*5�ľ���������Ԫ�ط������ģ��ĽǷֱ���ĸ���СԪ�أ��������ң����ϵ��µ�˳�����δ�С�����ţ���
 * ��дһ����ʵ��֮�� 
 * ���ȱ�дһ������������������������ʵ�ֱ��⡣ 
 *********************************************************/

/********************************** 
 * ð�������� 
 * ����a���飬��������������
 * N������Ԫ�ظ�����
 * ASC: 1,С->��,����;  0, ��->С,���� 
 ***********************************/
void order(int a[],int N,int ASC)
{      
    int i,j,t;
	int flag=0; // 0:��Ҫ������һ�˱Ƚϣ�1��������һ�˱Ƚ�
	
	printf("���������ݣ�");
	for (i=0;i<N;i++) { printf("%4d",a[i]); } printf("\n");

	for (j=1;j<=N-1;j++){   // N-1������ 
		flag = 1;
		for (i=0;i<N-j;i++) {  // ����Ԫ�������Ƚ�
            if ((ASC==1 && a[i]>a[i+1]) || (ASC==0 && a[i]<a[i+1])) { // ��С������������  || �ɴ�С���򣬽��� 
				t=a[i];
				a[i]=a[i+1];
				a[i+1]=t;
				flag = 0;
			}
		}
		if (flag == 1) { printf("������һ�˱Ƚ���!!\n"); break;  } // ������һ�˱Ƚ��� 
	}
	
	printf("�����");
	for (i=0;i<N;i++) { printf("%4d",a[i]); } printf("\n");	
}

void ch8_5()
{
     int a[5][5] ={{1,2,3,4,500},{6,7,8,9,10},{11,12,13,88,15},{16,17,18,19,20},{21,22,23,24,25}};
     static int b[5][5]; // Ŀ����� 
     
     // ��������a��С->�� 
     order(a[0],25,1); // ok,ʵ���Ƕ�ά���飬�β���һά���飬���������һ��Ԫ�ص�ַ���� 
     // order(a,25,1);  // gcc: [Warning] passing arg 1 of `order' from incompatible pointer type��vc++����ϵͳ��Ϊ���� 
     // order(&a[0][0],25,1); // ok 
     // order(*a,25,1); // ok
     
     // b�����ķ�������Ԫ�أ���a�����һ��Ԫ�� 
     b[2][2] = a[4][4];
     
     // b���ĽǷ�����С���ĸ�Ԫ�أ���a��ǰ�ĸ�Ԫ�� 
     b[0][0] = a[0][0]; b[0][4] = a[0][1];
     b[4][0] = a[0][2]; b[4][4] = a[0][3];
  
    
     // ��������Ԫ�أ�a����ͨ��ָ����ʣ��Ϸ��㡣 
     int i,j; // i,j��ʾb���±�
     int *pa = &a[0][4];  // a�ӵ�5��Ԫ�ؿ�ʼ, �൱��int *pa; pa=&a[0][4];  
     
     for(i=0;i<5;i++) { 
       for(j=0;j<5;j++) 
       {
          //b���ĽǺ��������� 
          if ((i==0&&j==0) || (i==0&&j==4) || (i==4&&j==0) || (i==4&&j==4) || 
               (i==2&&j==2)) 
          {
               continue;
          }     
          b[i][j] = *pa++;         
       } 
     }
      
    printf("�������\n");
    for (i=0;i<5;i++)
    { 
      for(j=0;j<5;j++)
       { printf("%4d",b[i][j]); } 
      printf("\n");
    }	 
}

/******************************************************
 * ch8,p192, 6. ��д����ʵ�������ַ����Ƚϵ��Զ���棺
 *    int strcmp(char *str1,char *str2);
 *    // ��str1>str2ʱ����������
 *    // ��str1==str2ʱ������0
 *    // ��str1<str2ʱ�����ظ��� 
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
 * ch8,p192, 7. ��д����ʵ�ָ����ַ������Զ���棺
 *    char *strcpy(char *dest,char *source);
 *    // �ú�������dest��ֵ�����ַ����׵�ַ 
 ******************************************************/
char* ch8_7(char *dest,const char *source)
{   
    char *s = dest;
    
    while ((*s++ = *source++) != '\0')
      ;
    
    return (dest);  // ��ʱsָ�����һ���ַ�'\0',��ˣ����ܷ���s.          
} 

/*****************************************************
 * ch8,p193, 8. ��д�������������в���Ϊ�����ַ�����
 *    ��strcmp()�Ƚϲ���������
 * ������ģ��main���������������в�����
 * ���磺char *argv[] = {"parameter1","parameter2"};
 *       ch8_8(2,argv);
 *****************************************************/
int ch8_8(int argc, char *argv[])
{
    if (argc >= 2) {
      printf("\"%s\"��\"%s\"�ȽϽ����%d\n",argv[0],argv[1],strcmp(argv[0],argv[1])); 
      return 0; // ��ʾ�������� 
    }
    return 1; // ��ʾδ��Ҫ��ִ�г��򣬷��������� 
}

/*******************************************************
 * ch8, p193, 9. ��д����
 * (1)��ʼ��һ������A(5*5),Ԫ��ֵȡ������������������
 * (2)���䴫�ݸ�������ʵ�־���ת�ã�
 * (3)������������������ 
 *******************************************************/ 
void ch8_9()
{
     //(1)��ʼ������
     int a[5][5];
     int i,j;
     
     // void srand(unsigned int seed)��������������ĳ�ʼ��������������Ƿ����������ӣ�ʹ��ϵͳ��ǰʱ����Ϊ���ӣ�time(0) 
     // int rand(void)�����������������Χ0~RAND_MAX,������˫�ֽ�,32767��
     // ��"int x = rand() % 100;"������ 0 �� 100 ֮�����������ַ����ǲ���ȡ�ģ�
     // �ȽϺõ������ǣ� j=(int)(n*rand()/(RAND_MAX+1.0))����һ��0��n֮��������  
     srand(time(0)); 
     for(i=0;i<5;i++)
     {
       for(j=0;j<5;j++)
       {
           a[i][j] = (int) (100.0*rand())/(RAND_MAX+1.0); // 0~100�������       
       }
     }
     
     printf("���5*5����\n"); 
     matrixPrint(&a[0][0],5,5);    // ��ӡ��� 
     
     // ��2��ת�� 
     printf("ת�ú�\n"); 
     NmatrixTranspose(&a[0][0],5); // ����ת�� 
     matrixPrint(&a[0][0],5,5);    // ��ӡ��� 
} 

/*******************************************************
 * ch8, p193, 10. ����ָ�������д���������·ݺţ�������µ�Ӣ�����֡�
 *   �磬����10�����"October" 
 *******************************************************/ 
void ch8_10()
{
     char *month[] ={"January","February","March","Aril","May","June","July",
                     "August","September","October","November","December"};
     
     int m;               
     printf("�������·ݺ�(1~12),����0��������\n");
     while(1) 
     { 
       scanf("%d",&m); 
       if (m==0) break;
       if (m < 1 || m >12) puts("�Ƿ��·ݺ�,�������롣"); 
       puts(month[m-1]); 
     }
} 
  
// ѧϰָ����p60,ָ��
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
    /*********************ͨ�ú�������*****************************************/
    	
	// ͨ�ú�����m*n�����ӡ��� 
    printf("void matrixPrint(int a[],int m,int n),ͨ�ú���,m*n�����ӡ���.\n");
    int matrix1[3][3] = {{1,2,3},{4,5,6},{7,8,9}};
    printf("-----maxtrix1,3��3��\n"); 
    //matrixPrint(matrix1,3,3); // [Warning] passing arg 1 of `matrixPrint' from incompatible pointer type  
    //matrixPrint(matrix1[],3,3); // [Warning] passing arg 1 of `matrixPrint' from incompatible pointer type
    matrixPrint(&matrix1[0][0],3,3); // ok
    // matrixPrint(*matrix1,3,3);  // ok
    
    printf("-----maxtrix2,3��2��\n");
    int matrix2[3][2] = {{1,2},{3,4},{5,6}}; 
    matrixPrint(&matrix2[0][0],3,2);
    printf("-----maxtrix3,2��3��\n");
    int matrix3[2][3] = {{1,2,3},{4,5,6}}; 
    matrixPrint(&matrix3[0][0],2,3);
    printf("-----maxtrix4,1��3��\n");
    int matrix4[3] = {1,2,3}; 
    matrixPrint(matrix4,1,3);
    
    // ͨ�ú�����n*n����ת��
    printf("-----maxtrix1����ת��\n");
    NmatrixTranspose(&matrix1[0][0],3);
    matrixPrint(&matrix1[0][0],3,3);
    
    // ͨ�ú�����m*n����ת�ã���n*m����m��n������תΪn��m��
    printf("-----maxtrix2ת��\n");
    int matrix2_T[2][3]; 
    MNmatrixTranspose(&matrix2[0][0],3,2,*matrix2_T);
    matrixPrint(&matrix2_T[0][0],2,3);
    
    printf("-----maxtrix3ת��\n");
    int matrix3_T[3][2]; 
    MNmatrixTranspose(&matrix3[0][0],2,3,*matrix3_T);
    matrixPrint(&matrix3_T[0][0],3,2);
    
    printf("-----maxtrix4ת��\n");
    int matrix4_T[3][1]; 
    MNmatrixTranspose(matrix4,1,3,*matrix4_T);
    matrixPrint(&matrix4_T[0][0],3,1); 
    
    /*********************ͨ�ú�������, ����***********************************/
    /**************************************************************************/
	
	// int ch8_1(char *s)
	printf("int ch8_1(char *s),���ַ����ĳ��ȡ�\n");
    printf("\"12345\"�ַ�������Ϊ��%d\n",ch8_1("12345")); 
    
    // ch8_2(char *s1, char *s2, int m)
    printf("ch8_2(char *s1, char *s2, int m),��s1����m���ַ���s2\n");
    char s1[]="123456",s2[80] = "";  //s2[0]='\0',ע�⣬һ�����ַ�����ʼ��Ϊ��'\0'������
    ch8_2(s1,s2,3);
    printf("��%s����%d���ַ���%s\n",s1,3,s2);
    
    // ch8_3(char *s),ͳ��s����ĸ�����֡��ո�������ַ��ĸ���; 
    printf("ch8_3(char *s),ͳ��s����ĸ�����֡��ո�������ַ��ĸ���\n");
    int Letter,Number,Space,Other; 
    ch8_3("123  d1 --- ",&Letter, &Number, &Space, &Other); 
    printf("��ĸ�����֡��ո�������ַ��ĸ���=%d,%d,%d,%d\n",Letter,Number,Space,Other); 
    
    /**************************************************/
    // ch8_4(int a[][],int n) ����ת�á�
    printf("ch8_4(int a[][],int n),����ת�á�\n");
    int a[3][3] = {{1,2,3},{4,5,6},{7,8,9}}; 
    
    ch8_4(a,3);
    
    int i,j;
    for(i=0;i<3;i++)
    {
     for(j=0;j<3;j++) printf("%3d",a[i][j]);
     printf("\n");
    }
    /*****************************************************/
     
    // ch8_5(),5*5����Ԫ������
    printf("ch8_5(),5*5����Ԫ������\n");
    ch8_5(); 
    
    // int ch8_6(char *str1,char *str2),�Զ�����strcmp
    printf("int ch8_6(char *str1,char *str2),�Զ�����strcmp. \n"); 
    printf("123��1234�Ƚ�=%d,%d\n",ch8_6("123","1234"),strcmp("123","1234"));
    printf("123��123�Ƚ�=%d,%d\n",ch8_6("123","123"),strcmp("123","123"));
    printf("12a��123�Ƚ�=%d,%d\n",ch8_6("12a","123"),strcmp("12a","123"));
    
    // char* ch8_7(char *dest,const char *source)���Զ�����strcpy
    char dest[80]="";
    char source[] = "abcd"; 
    printf("copy %s to dest,%s\n",source,ch8_7(dest,source));
    
    // int ch8_8(int argc, char *argv[]),ģ��main���������������в���
    printf("int ch8_8(int argc, char *argv[]),ģ��main���������������в���\n");
    char *argv[] = {"parameter1","parameter2"};
    ch8_8(2,argv);
    
    // void ch8_9(),����һ�����5*5���󣬲�ת��
    printf("void ch8_9(),����һ�����5*5���󣬲�ת��.\n");
    ch8_9();
    
    // void ch8_10(),ָ���������
    printf("void ch8_10(),ָ���������.\n");
    ch8_10(); 
		
	// ѧϰָ����p60,ָ��
	exercise();

}
