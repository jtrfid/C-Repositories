#include "stdafx.h"
#include <stdio.h>

/*******************************
 * ��ϰ
 *******************************/

/********
 ����������Գ����е�ע�����
 1. λ����|��&��^��<<,>>����
 2. �ֲ���������ȫ�ֱ���
 3. ���ϸ�ֵ�������ȼ���14�������������������3,4����x *= x+1  --> x=x*(x+1)
 4. ��̬�ֲ��������оֲ��ɼ��Ժͼ̳��ԣ���̬�����������ڹᴩ����������������ڼ䣬δ��ʼ����static������0;
 5. �����������򣬾ֲ���������ʽ�����Ǿֲ���������������ж���ı����������Ǳ��������{}
 6. ȫ�ֱ�����������һ���Ⱦֲ�������������Χ��--���Ǵ���ģ�����ȫ�ֱ����������ļ�ĩβ����û��extern�����չ�����ȫ�ֱ�����������Ϊ0
 7. ֵ�������ַ����
 8. while(��0){}��ִ��ѭ���壬����i%2,����ʾi���ܱ�2����ʱ��ִ��ѭ����
 9. ð�������ѭ������
 *****************/
void test_1()
{
	printf("===========test_1(),����x��y�е�ֵ.\n");
	int x,y;
	x=10;y=20;
	x+=y; y=x-y; x-=y;
	printf("x=%d,y=%d\n",x,y);
}

void test_1_1()
{
	printf("===========test_1_1(),���(���ȼ�:9),�����ڡ�,��λ(���ȼ���5)������ԣ���������.\n");
	int a=3,b=6,c;
	c=a^b<<2;
	printf("c=%0x\n",c);//c=1b,����00011011
}

void test_2()
{
	printf("========test_2(),(--y)���ʽ��ֵ�Ǳ仯���y����(y=y-1,y); (y--)���ʽ��ֵ��δ�仯��yֵ�������ʽ��ֵ=y,Ȼ��y=y-1\n");
	int y=10;
	do {
		y--;
		printf("y=%d\n",y);
	} while(--y);  // (--y)���ʽ��ֵ�Ǳ仯���y����(y=y-1,y)
	printf("%d\n",y--); // 0
	printf("y=%d\n",y); // -1

	printf("=========\n");
	int x=3;
	do {
		printf("%3d",x-=2);
	} while(!(--x));
	printf("\n");  // 1  -2
}

void test_2_2()
{
	printf("========test_2_2(),(--y)���ʽ��ֵ�Ǳ仯���y����(y=y-1,y); (y--)���ʽ��ֵ��δ�仯��yֵ�������ʽ��ֵ=y,Ȼ��y=y-1\n");
	int x=0,y=5,z=3;
	while(z-->0&&++x<5) y=y-1;
	printf("%d,%d,%d\n",x,y,z); // 3,2,-1
}

void test_2_3()
{ 
	printf("========test_2_3(),(--y)���ʽ��ֵ�Ǳ仯���y����(y=y-1,y); (y--)���ʽ��ֵ��δ�仯��yֵ�������ʽ��ֵ=y,Ȼ��y=y-1\n");
	int p[8]={11,12,13,14,15,16,17,18},i=0,j=0;
	while(i++<7) 
	{
		printf("%d,",i); // 1,2,3,4,5,6,7
		if(p[i]%2) j+=p[i];  // 13+15+17
	}
	printf("\n%d\n",j); // 45
}


void test_3()
{
	printf("========test_3(),||,��1Ϊ1��&&����0Ϊ0��ֹͣ��������\n");
	int a,b,c;
	a=b=c=1;
	++a||++b&&++c;
	printf("b=%d\n",b); // 1

	printf("%d\n",(a=2)&&(b=-2)); // 1

	int x,a1,b1,c1;
	x=(a1=4,b1=8,c1=12);
	printf("x=%d\n",x);
}

void test_3_2()
{
	printf("=========test_3_2(),||,��1Ϊ1��&&����0Ϊ0��ֹͣ��������\n");
	int a=1,b=2,c=3,d=4,m=2,n=2;
	int k;
	k=(m=a>b)&&(n=c>d);
	printf("m=%d,n=%d,k=%d\n",m,n,k); // 0,2,0
}

void test_4()
{
	printf("==========test_4(),������������Ϊȥ��С��������\n");
	int a=7; float x=2.5,y=4.7;
	printf("%f\n",x+a%3*(int)(x+y)%2/4);  // 2.5

	{
		int a=9,b=2;
		float x=6.6,y=1.1,z;
		z=a/2+b*x/y+1/2;
		printf("z=%5.2f\n",z); // 16.00
	}
}

void test_5()
{
	printf("===========test_5(),switch case ��break��䣬����ִ�������case;\n");
	int i;
	for (i=0;i<3;i++)
		switch(i){
		case 1: printf("%d",i);
		case 2: printf("%d",i);
		default: printf("%d",i);
	}
	printf("\n");  // 011122

	int x=1,a=0,b=0;
	switch(x)
	{
	case 0: b++;
	case 1: a++;
	case 2: a++; b++;
	}
	printf("a=%d,b=%d\n",a,b);  // 2,1
}

void test_6()
{
	printf("=======test_6(),ѭ������break��continue���\n");
	int x,y;
	for(x=1,y=1;y<50;y++){
		if(x>=10) break;
		if(x%2==1) { x += 5; continue;}
		x -= 3;
	}
	printf("x=%d\n",x);  // 10
}

int test_7(int x)
{
	printf("=======test_7()���ݹ����f(x)=x-f(x-2),xx=%d\n",x);
	int p;
	if (x==0 || x==1) return 3;
	else p=x-test_7(x-2);

	printf("x=%d,p=%d\n",x,p);
	return p;
}

void test_8()
{  
	printf("============test_8(),forѭ��\n");
	int k=0,i,s[]={1,-9,7,2,-10,3};
	for (i=0;i<6;i++)
		if (s[i]>s[k]) 
			k=i;
	printf("\n%d\n",k); // 2

	printf("=======��for���ִ��˳�򣬳�ʼ��-->�ж�ѭ������-->ִ��ѭ����-->ѭ������(�ڶ��ֺź�����)-->�ж�ѭ������-->...\n");
	int ii,jj;
	for(ii=0,jj=1;ii<=jj+1;ii+=2,jj--)  
		printf("%d\n",ii); // 0����ִ��һ��
}

void test_9()
{ 
	printf("=======test_9()�ṹ����ĳ�ʼ��\n");
	struct cmplx{ 
		int x; int y; 
	}cnum[2]={1,3,2,7};
	printf("%d\n",cnum[0].y /cnum[0].x * cnum[1].x); // 6
}

void test_10()
{
	printf("========test_10(),�����±���ָ�룬a[10],p=a;a[i]=p[i]=*(p+i);p=a+i-->a[i]; p=&a[2]\n");
	char a[10]={'1','2','3','4','5','6','7','8','9',0},*p;
	int i;
	i=8;
	p=a+i;
	printf ("%s\n",p-3); // 6789
}

static int e_i;
static int e_c;
static float e_f;
void test_11()
{
	printf("===========test_11(),δ��ʼ���ľ�̬������ȫ�ֱ���ֵ��������0����������0.0���ַ��ͣ����ַ�(\'\\0\')\n");
	static int i;
	static int c='\0';
	static float f;
	printf("���ַ�=%c,%d\n",'\0','\0');  // ��֪Ϊ�Σ�vc++��ӡ���ַ���'a'
	printf("i,c,f=%d,%c,%f,%d\n",i,c,f,c);// 0,a,0.0,0
	printf("e_i,e_c,e_f=%d,%c,%f,%d\n",e_i,e_c,e_f,e_c); // 0,a,0.0,0
}

void test_12()
{
	printf("============test_12()\n,���ϣ������壩��ÿ����Ա��ʼ��ַ����ͬ\n");
	union dt {int a;char b;double c;} data;
	data.a=5;
	printf("%d,%lf\n",data.a,data.c); // 5, 0.000
}

void test_13()
{
	printf("=============test_13()�������ĺ궨��\n");
#define f(x) x*x
	int i;
	i=f(4+4)/f(2+2);  // 4+4*4+4/2+2*2+2
	printf("%d\n",i); // 28
}

void test_14()
{
	printf("==========test_14(),do {} while()\n");
	int i=0,s=0;
	do
	{  
		if(i%2){i++;continue;}
		i++;
		s+=i;
	}while(i<7);
	printf(" %d\n ",s); // 16
}

void test_15() 
{	
	printf("==========test_15()���������\n");
	int n1,n2; 
	//scanf("%d",&n2);
	n2=1298;
	while(n2!=0) 
	{ 
		n1=n2%10; 
		n2=n2/10; 
		printf("%d",n1); // 8921
    } 
} 

void test_16()
{
	printf("=========test_16()\n");
	printf("�����������飬�������һ��Ԫ�ؽ�������С�������һ��Ԫ�ؽ�����������顣���鳤��Ϊ10.\n");

	int i;
	//int a[10]={0,1,2,3,4,5,6,7,8,9};
	//int a[10]={9,1,2,3,4,5,6,7,8,0};
	//int a[10]={10,1,2,12,4,5,6,7,8,9};
	int a[10]={10,1,2,12,12,5,6,7,8,9};
	int max,min,m1,m2; // �����СԪ�ؼ����±�
	int tmp;

	//for (i=0;i<10;i++) scanf("%d",a[i]);

	printf("ԭ����:\n");
	for (i=0;i<10;i++) printf((i==9)?"%d\n":"%d,",a[i]);

	min=a[0];max=a[0];
	m1=m2=0;
	for (i=0;i<10;i++) {
		if(a[i]>max) { max=a[i]; m1=i; }
		if(a[i]<min) { min=a[i]; m2=i; }
	}

	printf("�����СԪ�ؼ����±�: max,min=%d,%d,m1,m2=%d,%d\n",max,min,m1,m2);
	
	// ���Ԫ��������ĵ�һ��Ԫ�ؽ���
	tmp=a[0]; a[0]=max; a[m1]=tmp;
	// ��������СԪ���������������ͷ���򲻽��еڶ��ν�����������һ�佻������
	if (!((m1==0&&m2==9) || (m1==9&&m2==0))) {
		printf("��СԪ������������һ��Ԫ�ؽ���\n");
	    tmp=a[9]; a[9]=min; a[m2]=tmp;
	}

	printf("����������:\n");
	for (i=0;i<10;i++) printf((i==9)?"%d\n":"%d,",a[i]);
}

void test_16_1()
{
	printf("=========test_16_1()\n");
	printf("�����������飬�������һ��Ԫ�ؽ�������С�������һ��Ԫ�ؽ�����������顣���鳤��Ϊ10.\n");

	int i;
	//int a[10]={0,1,2,3,4,5,6,7,8,9};
	//int a[10]={9,1,2,3,4,5,6,7,8,0};
	//int a[10]={10,1,2,12,4,5,6,7,8,9};
	int a[10]={10,1,2,12,12,5,6,7,8,9};
	int tmp;

	//for (i=0;i<10;i++) scanf("%d",a[i]);

	printf("ԭ����:\n");
	for (i=0;i<10;i++) printf((i==9)?"%d\n":"%d,",a[i]);

	// a[0]���
	for (i=0;i<10;i++) {
		if(a[0]<a[i]) { tmp=a[0]; a[0]=a[i]; a[i]=tmp; }
	}

	// a[9]��С
	for (i=0;i<10;i++) {
		if(a[9]>a[i]) { tmp=a[9]; a[9]=a[i]; a[i]=tmp; }
	}

	printf("����������:\n");
	for (i=0;i<10;i++) printf((i==9)?"%d\n":"%d,",a[i]);
}

//void test_17(char ss[],char c) // ��Ч����һ��
void test_17(char *ss,char c)
{
	printf("=========test_17()��Чtest_18()\n");
	printf("���ss��ָ�ַ�����ָ���ַ��ĸ����������ش�ֵ�����磬�������ַ���123412132�������ַ�1�������3��\n");
    int i,n;
	for (i=0,n=0;ss[i]!='\0';i++) {
		if (ss[i]==c) 
			n++;
	}
	printf("�ַ�������%d\n",n);
}

void test_18(char *ss,char c)
{
	printf("=========test_17()��Чtest_18()\n");
	printf("���ss��ָ�ַ�����ָ���ַ��ĸ����������ش�ֵ�����磬�������ַ���123412132�������ַ�1�������3��\n");
	int n=0;
	//*,++��������ȼ���Ϊ2����������
	//*ss++ --> *(ss++),��ʹ�ã��������������ʽ��ֵ��*ss��Ȼ��ss++;
	//*++ss --> *(++ss),����������ʹ�ã���ss=ss+1,Ȼ����ʽ��ֵ��*ss
	// '\0'��ASCII����0,'0'��ascii����49

	/***************
	do 
	{
		if (*ss==c) n++;
	} while (*++ss);
	****************/

    // ����
	while (*ss != '\0') // while(*ss)
	{
		if (*ss==c) n++;
		ss++;
	}
	printf("�ַ�������%d\n",n);
}

void test_19(int n)
{
	printf("=========test_19()\n");
	printf("���㲢������ж���ʽֵ��Sn = 1 + 1/1! + 1/2! + 1/3! + 1/4! + ... + 1/n! \n");

	int i,j;
	double sum=1.0,fac;

	for (i=1;i<=n;i++)
	{
		fac = 1.0;
		for(j=1;j<=i;j++) fac *= j;
		sum += 1.0/fac;
	}
	printf("n=%d,sum=%lf\n",n,sum);
}

void test_19_1(int n)
{
	printf("=========test_19_1()\n");
	printf("���㲢������ж���ʽֵ��Sn = 1 + 1/(1+2) + 1/(1+2+3) + ... + 1/(1+2+3+...+n) \n");

	int i,j;
	long fac;
	double sum=0.0;

	for (i=1;i<=n;i++)
	{
		fac = 0;
		for(j=0;j<=i;j++) fac += j;
		sum += 1.0/fac;
	}
	printf("n=%d,sum=%lf\n",n,sum);
}

void test_19_2(int n)
{
	printf("=========test_19_2()\n");
	printf("���㲢������ж���ʽֵ��Sn = 1 + 1/(1+2) + 1/(1+2+3) + ... + 1/(1+2+3+...+n) \n");

	int i;
	long fac;
	double sum=0.0;

	fac=0;
	for (i=1;i<=n;i++)
	{
		fac += i;
		sum += 1.0/fac;
	}
	printf("n=%d,sum=%lf\n",n,sum);

}

void test_19_3(int n)
{
	printf("=========test_19_3()\n");
	printf("���㲢������ж���ʽֵ��Sn = 1 + 1/(1+2) + 1/(1+2+3) + ... + 1/(1+2+3+...+n) \n");

	int i;
	double sum=0.0,fac;

	for (i=1;i<=n;i++)
	{
		fac = (1+i)*i/2.0;
		sum += 1.0/fac;
	}
	printf("n=%d,sum=%lf\n",n,sum);

}

int func(int a,int b)
{
	int c;
	c=a+b;
	return c;
}

void test_20()
{
	printf("========test_20(),(x--,y++,x+y)���ʽ��x,y��ʹ��,���������ʽ��ֵ��δ�仯��x+y\n");
	int x=6,y=7,z=8,r;
	r=func((x--,y++,x+y),z--);  //func(6+7,8)
	printf("%d\n",r); // 21
}

void test_21()
{
	printf("=========test_21(),ָ�������鼰++��--\n");
	int a[]={1,2,3,4},y,*p=&a[1];
	y=(*--p)++;
	printf("%d\n",y); // a[0]
	
	int i;
	for(i=0;i<4;i++) printf((i==3)?"%3d\n":"%3d",a[i]); // 2 2 3 4
}

void test_22()
{
	printf("=========test_22(),ָ��������\n");
	int a[]={2,4,6,8,10},y=1,x,*p=&a[1];
	for(x=0;x<3;x++) y+=*(p+x);
	printf("%d\n",y); // 1+a[1]+a[2]+a[3] = 19
}

void test_23()
{
	printf("=========test_23(),contine�������ò㡿ѭ��\n");
	int i,n=0;
	for (i=2;i<5;i++)
	{
		do 
		{
			if(i%3) continue;
			n++;
		} while (!i);
		n++;
	}
	printf("n=%d\n",n);
}

void sum(int *a)
{
	a[0]=a[1];
}

void test_24()
{
	printf("============test_24(),ָ������������,ָ��������ĵ�Чʹ��\n");
	int a[10]={1,2,3,4,5,6,7,8,9,10},i;
	for(i=2;i>=0;i--) sum(&a[i]);
	for(i=0;i<=4;i++) printf(i==4?"%d\n":"%d,",a[i]); //4,4,4,4,5
}

void test_25(int n)
{
	printf("=====test_25(int n)\n");
	printf("��дһ������������nΪż��ʱ�����ú�����1/2+1/4+...+1/n,������nΪ����ʱ�����ú���1/1+1/3+...+1/n;\n");
	int i;
	double sum=0.0;
	if (!(n%2)) {
		for (i=2;i<=n;i+=2) {
			sum += 1.0/i;  // �м�1.0/i��������1/i
		}
		printf("ż��n=%d��sum=%lf\n",n,sum);
	}
	else {
		for (i=1;i<=n;i+=2) {
			sum += 1.0/i; // �м�1.0/i��������1/i
		}
		printf("����n=%d��sum=%lf\n",n,sum);
	}
}

void test()
{
	test_1();
	test_1_1();
	test_2();
	test_2_2();
	test_2_3();
	test_3();
	test_3_2();
	test_4();
	test_5();
	test_6();
	printf("%d\n",test_7(9)); // 7
	test_8();
	test_9();
	test_10();
	test_11();
	test_12();
	test_13();
	test_14();
	test_15();
	test_16();
	test_16_1();
	test_17("123412132",'1');
	test_18("123412132",'1');
	test_19(1);  //2.0
	test_19(2);  //2.5
	test_19(3);  //2.67
	test_19_1(1); //1.0
	test_19_1(2); //1.33
	test_19_1(3); //1.5
	test_19_2(1); //1.0
	test_19_2(2); //1.33
	test_19_2(3); //1.5
	test_19_3(1); //1.0
	test_19_3(2); //1.33
	test_19_3(3); //1.5
	test_20();
	test_21();
	test_22();
	test_23();
	test_24();
	test_25(1);
	test_25(2);
	test_25(3);
	test_25(4);
}