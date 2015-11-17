#include <stdio.h>
#include <conio.h>


void test_for()
{
  printf("====test_for()\n");
  int row; char ch;
  for (row = 0; row < 5; row++) {
     for (ch = 'A'; ch < ('A' + 5); ch++)
       printf("%c",ch);
     printf("\n");
  }
  for (row = 0; row < 5; row++) {
     for (ch = 'A'; ch < ('A' + row); ch++)
       printf("%c",ch);
     printf("\n");
  }
}

// p106,计算1!+2!+3!+...+10!
void p106_2()
{
	printf("p106,计算1!+2!+3!+...+10!\n");
    int i,j;
	long int fac;     // 阶乘
	long int sum=0; // sum：总和

	int n=10;
	for (i=1;i<=n;i++)
	{
		fac=1;
		for (j=1;j<=i;j++) fac *= j;
		sum += fac;
	}
	printf("sum=%ld\n",sum);
}

void ch5()
{
    test_for(); 
	// p106,计算1!+2!+3!+...+10!
	p106_2();
	//getchar();
}
