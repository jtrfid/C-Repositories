#include "stdafx.h"
#include <stdio.h>

void swap1(int x,int y)
{
	int temp;
	temp=x;
	x=y;
	y=temp;
}

void swap2(int *p1,int *p2)
{
	int p;
	p = *p1;
	*p1 = *p2;
	*p2 = p;
}

void swap3(int *p1,int *p2)
{
	int *p;
	p = p1;
	p1 = p2;
	p2 = p;
}

void swap()
{
	printf("=====swap()\n");
	int a=5,b=9;
	swap1(a,b);
	printf("=====swap1() a=%d,b=%d\n",a,b); // 5,9
	a=5;b=9;
	int *p1,*p2;
	p1=&a;  p2=&b;
	swap2(p1,p2);
	printf("=====swap2() a=%d,b=%d,*p1=%d,*p2=%d\n",a,b,*p1,*p2); // 9,5
	a=5;b=9;
	p1=&a;  p2=&b;
	swap3(p1,p2);
	printf("=====swap2() a=%d,b=%d,*p1=%d,*p2=%d\n",a,b,*p1,*p2); // 5,9
}

void  pointer()
{   
	printf("======pointer()\n");
	int  a[]={5,8,7,6,2,7,3};
	int *p = a;
	int i=2;
	// a[i],p[i],*(a+i),*(p+i),
	printf("%d,%d,%d,%d,%p,%p\n",a[i],p[i],*(a+i),*(p+i),a+i,p+i); // 7,7,7,7,0028F298,0028F298
	printf("%p,%p,%p\n",a,&a[0],p); // 0028F290,0028F290,0028F290
	printf("%d,%d,%d\n",*a,a[0],*p); // 5,5,5
	//a++;  // “++”需要左值, 这里a是地址常量，因此不能进行a=a+1这样的变量运算
	p++;  // p是指针变量，可以有p=p+1;
	printf("%d,%d\n",*a,*p); // 5,8
} 

void pointer_2d(){
	printf("======pointer_2d()\n");
	// int a[3][4]={0,1,2,3,4,5,6,7,8,9,10,11};
	int a[3][4]={{0,1,2,3},{4,5,6,7},{8,9,10,11}};  // 等效
	printf("%d,",a);
	printf("%d,",*a);
	printf("%d,",a[0]);
	printf("%d,",&a[0]);
	printf("%d\n",&a[0][0]); // 以上相同
	printf("%d,",a+1);
	printf("%d,",*(a+1));
	printf("%d,",a[1]);
	printf("%d,",&a[1]);
	printf("%d\n",&a[1][0]); // 以上相同
	printf("%d,",a+2);
	printf("%d,",*(a+2));
	printf("%d,",a[2]);
	printf("%d,",&a[2]);
	printf("%d\n",&a[2][0]); // 以上相同
	printf("%d,",a[1]+1);    // a[1][1]地址
	printf("%d\n",*(a+1)+1); // a[1][1]地址
	printf("%d,%d\n",*(a[1]+1),*(*(a+1)+1)); // a[1][1] = 5
}

// 指针自增、自减
// *,++,--,优先级：2，结合方向：自右向左
// ++p：先自增，再使用(作为表达式的值)
// p++：先使用(作为表达式的值)，后自增
void pointer1_0()
{
	printf("======pointer1_0()\n");
	int i,*p,a[7];
	p=a;
	for (i=0;i<7;i++) *p++ = i;  // [*p++ = *(p++)] = i; *p = i; p = p + 1;  
	p=a;
	for (i=0;i<7;i++) printf("%d ",*p++);  // *p++ = *(p++); *p; p = p + 1;  
	printf("\n");  // 0 1 2 3 4 5 6
	
	i=5;
	p=a+i;
	printf("i=%d,*p++=%d, a[i++]=%d\n",i,*p++,a[i++]); // i=5,*p++=5, a[i++]=5
	i=5;
	p=a+i;
	printf("i=%d,*++p=%d, a[++i]=%d\n",i,*++p,a[++i]); // i=6,*++p=6, a[++i]=6
	i=5;
	p=a+i;
	printf("*++p=%d, a[++i]=%d,i=%d\n",*++p,a[++i],i); // *++p=6, a[++i]=6,i=6

}

void  pointer1_1()
{   
	printf("======pointer1_1()\n");
	int  a[]={5,8,7,6,2,7,3};
	int y,*p=&a[1];
	y=(*--p)++; // 先p减1 --> y=(*p)++ --> y=(*p) --> (*p)++
	printf("%d  ",y); // 5
	printf("%d %d %d\n",a[0],a[1],*p); // 6 8 6
} 

void  pointer1_2()
{   
	printf("======pointer1_2()\n");
	int  a[]={5,8,7,6,2,7,3};
	int y,*p=&a[1];
	y=(*p++)++;  // y=(*(p++))++; --> y=(*p)++; y=(*p); (*p)++; p=p+1; 
	printf("y=%d,*p=%d,a[1]=%d,a[2]=%d\n",y,*p,a[1],a[2]);  // y=8,*p=7,a[1]=9,a[2]=7
} 

/************************************************************************/
/* 指向二维数组元素的指针变量
/* a[0],*(a+0),*a，&a[0][0]                                             */
/* 第0行第0列元素地址                                                    */
/************************************************************************/
void pointer2_1()
{   
	printf("======pointer2_1()\n");
	static int a[3][4]={1,3,5,7,9,11,13,15,17,19,21,23};
	int *p;
	for(p=a[0];p<a[0]+12;p++) 
	{   if((p-a[0])%4==0)   printf("\n");
	    printf("%4d  ",*p);
    }
	printf("\n");
}

/************************************************************************/
/* 指向二维数组元素的指针变量
/* a[0],*(a+0),*a，&a[0][0]                                             */
/* 第0行第0列元素地址                                                    */
/************************************************************************/
void pointer2_2()
{   
	printf("======pointer2_2()\n");
	static int a[3][4]={1,3,5,7,9,11,13,15,17,19,21,23};
	int *p;
	for(p=*a;p<*a+12;p++) 
	{   if((p-*a)%4==0)   printf("\n");
	    printf("%4d  ",*p);
	}
	printf("\n");
}

/************************************************************************/
/* 指向二维数组元素的指针变量
/* a[0],*(a+0),*a，&a[0][0]                                             */
/* 第0行第0列元素地址                                                    */
/************************************************************************/
void pointer2_3()
{   
	printf("======pointer3_2()\n");
	static int a[3][4]={1,3,5,7,9,11,13,15,17,19,21,23};
	int *p;
	for(p=&a[0][0];p<&a[0][0]+12;p++) 
	{   if((p-&a[0][0])%4==0)   printf("\n");
	printf("%4d  ",*p);
	}
	printf("\n");
}

/************************************************************************/
/* 指向二维数组元素的指针变量
/* a[0],*(a+0),*a，&a[0][0]                                             */
/* 第0行第0列元素地址                                                    */
/************************************************************************/
void pointer3_3()
{   
	printf("======pointer3_2()\n");
	static int a[3][4]={1,3,5,7,9,11,13,15,17,19,21,23};
	int *p;
	for(p=(int *)a;p<(int *)a+12;p++) 
	{   if((p-(int *)a)%4==0)   printf("\n");
	printf("%4d  ",*p);
	}
	printf("\n");
}

// 二维数组行指针和列指针
void row_pointer()
{ 
	printf("=========row_pointer()\n");
	int a[3][4]={{1,2,3,4},{3,4,5,6},{5,6,7,8}};
	int i;
	int (*p)[4]=a,*q=a[0]; // p：行指针，q：列指针
	for(i=0;i<3;i++)
	{   
		if(i==0) (*p)[i+i/2]=*q+1;
		else  p++,++q;
	}
	for(i=0;i<3;i++) printf("%d,",a[i][i]);
	printf("%d,%d\n",*((int *)p),*q);

	// 列出修改后的数组元素,以下等效
	printf("-----------------\n");
	p=a;
	for(i=0;i<3;i++) {
		printf("%d,%d,%d,%d\n",**p,*(*p+1),*(*p+2),*(*p+3));
		p++;
	}
	printf("-----------------\n");
	p=a;
	for(i=0;i<3;i++) {
		printf("%d,%d,%d,%d\n",(*p)[0],(*p)[1],(*p)[2],(*p)[3]);
		p++;
	}
	printf("-----------------\n");
	p=a;
	for(i=0;i<3;i++) {
		q=a[i];  // 本行首列指针
		printf("%d,%d,%d,%d\n",*q,*(q+1),*(q+2),*(q+3));
		p++;
	}
}

// 二维数组行指针
void row_pointer1()
{ 
	printf("=========row_pointer1()\n");
	int a[3][4]={{1,3,5,7},{2,4,6,8},{10,20,30,40}};
	int i,j,(*p)[4]; // p：行指针
	p=a;
	for (i=0;i<3;i++)
	{
		for (j=0;j<4;j++) printf("%4d",p[i][j]);
		printf("\n");
	}
	printf("-------------\n");
	for (i=0;i<3;i++)
	{
		for (j=0;j<4;j++) printf("%4d",*(*(p+i)+j));
		printf("\n");
	}
	printf("-------------\n");
	for (i=0;i<3;i++)
	{
		for (j=0;j<4;j++) printf("%4d",(*(p+i))[j]);
		printf("\n");
	}
}

// 学习指导，p60,指针
void exercise()
{ 
	printf("=========exercise()\n");
	int a[2][3]={{1,2,3},{4,5,6}};
	int m,*p=&a[0][0];
	m=(*p)*(*(p+2))*(*(p+4));
	printf("m=%d\n",m);
}


void average(float *p,int n)
{
	printf("========average()\n");
	float *p_end,sum=0,aver;
	p_end = p+n-1;
	for (;p<p_end;p++)
	{
		sum = sum + (*p);
	}
	aver = sum/n;
	printf("average=%5.2f\n",aver);
}

void search(float (*p)[4],int n)
{
	printf("=========search()\n");
	int i;
	printf("No. %d\n",n);
	for (i=0;i<4;i++)
		printf("%5.2f\t",*(*(p+n-1)+i));
	printf("\n");
}

void average_search()
{
	printf("=========average_search()\n");
	float score[3][4]=
	{{65,67,79,60},{80,87,90,81},
	{90,99,100,98}};

	//average(score,12); // 不能将参数 1 从“float [3][4]”转换为“float *”	
	average(*score,12);

	search(score,2);        // ok
	search(&score[0],2);    // ok
	//search(score[0],2); // 不能将参数 1 从“float [4]”转换为“float (*)[4]”	


}

// 字符串
void string_ex()
{   
	printf("==========string_ex()\n");
	//char string[]="I love China!"; // ok
	char string[]="I love China!";   // ok
	printf("%s\n",string);
	printf("%s\n",string+7);
}


void ch8()
{
	printf("==========ch8()\n");
	swap();
	pointer();
	pointer_2d();
	pointer1_0();
	pointer1_1();
	pointer1_2();
	pointer2_1();
	pointer2_2();
	pointer2_3();

	// 二维数组行指针和列指针
	row_pointer();

	// 二维数组行指针
	row_pointer1();

	// 学习指导，p60,指针
	exercise();

	average_search();

	// 字符串
	string_ex();
}