//#include "stdafx.h"
#include <stdio.h>

// 循环语句的辗除法,求两个整数的最大公约数
int gcd1(int a,int b){
	printf("循环语句的辗除法,求两个整数的最大公约数,gcd1()\n");
	int temp;
	if(a<b){/*交换两个数，使大数放在a上*/
		temp=a; a=b; b=temp;
	}

	while(b!=0){/*利用辗除法，直到b为0为止*/
		temp=a%b;
		a=b;
		b=temp;
	}
	return a;
}

// 循环语句的辗除法,求两个整数的最大公约数
int gcd2(int a,int b){
	printf("循环语句的辗除法,求两个整数的最大公约数,gcd2()\n");
	int temp;
	if(a<b){/*交换两个数，使大数放在a上*/
		temp=a;
		a=b;
		b=temp;
	}

	do {
		temp=a%b;
		if (temp == 0) break; 
		a=b;
		b=temp;
	} while (temp != 0);

	return b;
}

// 递归调用的辗除法,求两个整数的最大公约数
int gcd3(int a,int b){
	printf("递归调用的辗除法,求两个整数的最大公约数gcd3(),a=%d,b=%d\n",a,b);
	// 不用比较a,b大小，如果b>a, a%b=a,第二次调用，自然将a,b调换，即大数a,小数b
	if (b==0) return a;
	else return gcd3(b,a%b);
	
}

// 递归调用的辗除法,求两个整数的最大公约数
int gcd4(int a,int b){
	printf("递归调用的辗除法,求两个整数的最大公约数gcd4(),a=%d,b=%d\n",a,b);
	// 不用比较a,b大小，如果b>a, a%b=a,第二次调用，自然将a,b调换，即大数a,小数b
	if (a%b==0) return b;
	else {
		return gcd4(b,a%b);
	}
}

// 递归调用的辗除法,求两个整数的最大公约数
int gcd5(int a,int b){
	printf("递归调用的辗除法,求两个整数的最大公约数gcd5(),a=%d,b=%d\n",a,b);
	// 不用比较a,b大小，如果b>a, a%b=a,第二次调用，自然将a,b调换，即大数a,小数b
	int temp;
    
	temp = a%b;
	if (temp==0) return b;
	else {
		return gcd5(b,temp);
	}
}


// 最小公倍数=两整数的乘积÷最大公约数
int multiple (int a,int b)  
{
	printf("最小公倍数=两整数的乘积÷最大公约数\n");
	return(a*b/gcd1(a,b));
}

void ch1()
{
	// 循环语句的辗除法,求两个整数的最大公约数
	printf("319,377最大公约数：%d\n",gcd1(319,377)); //29

	// 循环语句的辗除法,求两个整数的最大公约数
	printf("319,377最大公约数：%d\n",gcd2(319,377)); //29

	// 递归调用的辗除法,求两个整数的最大公约数
	printf("377,319最大公约数：%d\n",gcd3(377,319)); //29
	printf("319,377最大公约数：%d\n",gcd3(319,377)); //29
	printf("377,319最大公约数：%d\n",gcd4(377,319)); //29
	printf("319,377最大公约数：%d\n",gcd4(319,377)); //29
	printf("377,319最大公约数：%d\n",gcd5(377,319)); //29
	printf("319,377最大公约数：%d\n",gcd5(319,377)); //29

	// 最小公倍数=两整数的乘积÷最大公约数
	//printf("319,377最小公倍数：%d\n",multiple (319,377));  
}
