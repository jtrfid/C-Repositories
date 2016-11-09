#include <stdio.h>
#include <conio.h>
#include <math.h> 

/**********************************
 ch5,p106, 1. 计算：1+1/3+1/5+...+1/51 
 **********************************/
void ch5_1()
{
	printf("ch5_1(),计算: 1+1/3+1/5+...+1/51\n");
	int i;
	float sum = 0; 
    for(i=1; i<=51; i+=2) 
    {
        sum += 1.0/i;  // 注意1.0/i,而不是1/i 
        //printf("i=%d,sum=%f\n",i,sum);  // 验证   
    } 
    printf("sum=%f\n",sum);
} 

/**********************************
 ch5,p106, 2. 计算1!+2!+3!+...+10!
 外层循环，累加；内层循环求阶乘 
 **********************************/
void ch5_2()
{
	printf("ch5_2(),计算: 1!+2!+3!+...+10!\n");
    int i,j;
	long int fac;     // 阶乘
	long int sum=0;   // sum：总和

	for (i=1;i<=10;i++)
	{
		fac=1;
		for (j=1;j<=i;j++) fac *= j;
		sum += fac;
	}
	printf("sum=%ld\n",sum);
}

/**********************************
 ch5,p106, 3. 读入一系列整数，统计出正整数和负整数个数，读入0则结束。 
 **********************************/
void ch5_3()
{
	printf("ch5_3(),统计正负整数。\n");
	int d; // 读入的整数 
	int m = 0, n = 0; // 正负整数个数 
    printf("输入一系列整数，空格隔开，0，退出。\n");
    while(1) 
    {
        scanf("%d",&d);
        if (d > 0)      m++;  // 正整数计数 
        else if (d < 0) n++;  // 负整数计数
        else break; // d=0,结束 
    } 
    printf("正整数个数:%d,负整数个数%d\n",m,n);     
} 

/**********************************
 ch5,p106, 4. 求满足不等式的最小正整数N:
           (1) 1+2+3+...+N  >=1000;
           (2) 1+1/2+1/3+...+1/N >= 2. 
 **********************************/
void ch5_4()
{
	printf("ch5_4(),求最小正整数。\n");
	
	int N;
	float sum;
	
	// (1) 1+2+3+...+N  >=1000;
	for(N=1,sum=0;;N++) 
    {
       sum += N;
       if (sum>=1000) break;
    }
	printf("(1)最小正整数:%d\n",N);
	
	// (2) 1+1/2+1/3+...+1/N >= 2. 
	for(N=1,sum=0;;N++) 
    {
       sum += 1.0/N; // 注意1.0/N，而非1/N 
       if (sum>=2) break;
    }
	printf("(2)最小正整数:%d\n",N);
	
	// 二者均满足
	float sum1,sum2; 
    for(N=1,sum1=0,sum2=0;;N++) 
    {
       sum1 += N;                         
       sum2 += 1.0/N; // 注意1.0/N，而非1/N 
       if (sum1 >=1000 && sum2>=2) break;
    } 
	printf("(1)、(2)均满足的最小正整数:%d\n",N);
} 

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

/**
 *递归调用的辗除法,求两个整数的最大公约数.
  欧几里德算法又称辗转相除法，用于计算两个正整数a，b的最大公约数
  两个整数的最大公约数等于其中较小的那个数和两数相除余数的最大公约数。
  最大公约数（greatest common divisor）缩写为gcd。
  gcd(a,b) = gcd(b,a mod b) (不妨设a>b 且r=a mod b ,r不为0)
  
  证: 
  a可以表示成 a = kb + r（a，b，k，r皆为正整数），则r = a mod b
  假设d是a,b的一个公约数，记作d|a,d|b，即a和b都可以被d整除。
  而r = a - kb，两边同时除以d，r/d=a/d-kb/d=m，等式左边可知m为整数，因此d|r
  因此d也是（b,a mod b）的公约数
  因此（a,b）和（b,a mod b）的公约数是一样的，其最大公约数也必然相等，得证。
 */
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

// 最小公倍数=两整数的乘积/最大公约数
int multiple (int a,int b)  
{
	printf("最小公倍数=两整数的乘积÷最大公约数\n");
	return(a*b/gcd1(a,b));
}

/*********************************************
 * ch5, p106, 5. 输入两个正整数m和n，求其最大公约数和最小公倍数 
 *********************************************/ 
void ch5_5()
{
    printf("====ch5_5()\n");
	int m,n,gcd;
    printf("请输入两个正整数，求其最大公约数和最小公倍数。\n");
    scanf("%d%d",&m,&n);
    gcd = gcd1(m,n); 
    printf("%d,%d的最大公约数=%d,",m,n,gcd);
    printf("最小公倍数=%d\n",m*n/gcd); 
    /******************************************** 
    以下为各种方法的验证 
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
	printf("319,377最小公倍数：%d\n",multiple (319,377)); 
    **********************************************/ 
}

/*********************************************
 * ch5, p106, 6. 输出图案 
 *        分析：每行按照空格和星号个数打印输出 
 *********************************************/ 
void ch5_6()
{
     printf("ch5_6()，输出图案\n");
     char sp = ' ', star  = '*';
     int spNum,starNum,i;
     // 上半部分 
     for(spNum = 3,starNum = 1; starNum <= 7; spNum--,starNum += 2) {                        
       for(i=0;i < spNum;i++)   printf("%2c",sp);
       for(i=0;i < starNum;i++) printf("%2c",star); 
       for(i=0;i < spNum;i++)   printf("%2c",sp); 
       putchar('\n'); 
     }
     // 下半部分 
     for(spNum=1,starNum=5; starNum >= 1; spNum++,starNum -= 2) {                        
       for(i=0;i<spNum;i++)   printf("%2c",sp);
       for(i=0;i<starNum;i++) printf("%2c",star); 
       for(i=0;i<spNum;i++)   printf("%2c",sp);
       putchar('\n'); 
     }
}

/*********************************************
 * ch5, p107, 7. 用pi/2=2/1*2/3*4/3*4/5*6/5*6/7*...前100项之积计算pi。
 *      分析：递推公式： t=1; t = t*[2n/(2n-1)]*[2n/(2n+1)], n=1..50       
 *********************************************/ 
void ch5_7()
{
     printf("ch5_7(),计算pi.\n");
     int n;
     float pi,t;
     for(n=1,t=1; n <= 50; n++)
     {
        t = t*(4*n*n)/((2*n-1)*(2*n+1));  // 分子自动转换为float 
        // 注意，如果写成 t *= (4*n*n)/((2*n-1)*(2*n+1));
        // 相当于：t= t* [4*n*n/((2*n-1)*(2*n+1))];  []内是整数/整数，舍掉了小数; 因此必须：
        // t *= (float)(4*n*n)/((2*n-1)*(2*n+1)) 
         
     } 
     pi = 2 * t;
     printf("pi=%f\n",pi);
}

/*********************************************
 * ch5, p107, 8. 输出三角形式的九九乘法表。
 *      分析： 双重循环，外层循环进行"行"的遍历，内层循环进行"列"的遍历      
 *********************************************/ 
void ch5_8()
{
     printf("ch5_8(),九九乘法表.\n");
     int i,j;
     
     // 打印标题行 
     for(i=1; i <= 9; i++) printf("%-3d",i);         putchar('\n');  
     for(i=1; i <= 9; i++) printf(i==9?"-":"---");   putchar('\n');
     
     // 行、列
     for(i=1; i <= 9; i++) // 行 
     {
        // 最左端标题列 
        printf("%-3d",i);      
        if (i == 1) { putchar('\n' ); continue; } // 如果是第一行，输出"1\n" 
        
        // 行列相乘结果列 
        for(j=2; j <= i; j++)   
        {
           printf("%-3d",i*j); 
        }
        putchar('\n'); 
     } 
}

/*********************************************
 * ch5, p107, 9. 两队乒乓球比赛，各出三人。甲队为A、B、C，乙队为X、Y、Z，
 *  抽签决定比赛名单，已知A不和X比，C不和X、Z比。请编程找出对阵名单。
 *  分析：双重循环，行列组成的"格子"，即行列相逢即对阵   
 *********************************************/
void ch5_9()
{
    printf("ch5_9(),对阵名单.\n");
    char team1,team2;
    // 行列相逢即对阵 
    for(team1='A';team1 <= 'C'; team1++) 
    {
       for(team2='X';team2 <= 'Z'; team2++)
       {
          if (team1 == 'A' && team2 == 'X') { printf("轮空\t"); continue;} 
          if (team1 == 'C' && team2 == 'X') { printf("轮空\t"); continue;} 
          if (team1 == 'C' && team2 == 'Z') { printf("轮空\t"); continue;} 
          printf("%c对阵%c\t",team1,team2);
       }
       putchar('\n'); 
    }
}

/************************************************
 * ch5, p107, 10. 求100~150之间的素数。
 *  判一个数m是否为素数的方法: 
 *  用2,3,...,sqrt(m)的整数去除它时均不能除尽，即为素数。 
 *************************************************/ 
 void ch5_10()
{
    printf("ch5_10(),求100~150之间的素数.\n");
    int m, k, i, n=0;  
    for (m=101; m<=150; m=m+2)
    {     
        k=sqrt(m);
        for (i=2; i<=k;i++)
          if (m%i==0) break;  // 不是素数 
          
        if (i>=k+1) {  // 是素数 
          printf("%5d",m);
          n=n+1;
          if ( n%5==0) printf("\n"); // 满5，换行 
        }
    }
} 

/************************************************
 * ch5, p107, 11. 用迭代法求x=sqrt(a).求平方根的迭代公式为
 *                Xn+1 = (Xn+a/Xn)/2,
 *          要求前后两次求出的x的差的绝对值小于10的-5次方。
 * 迭代公式来源于牛顿迭代法，y=f(x0)+(df(x0)/dx)*(x-x0),令y=0，得迭代公式：x1=x0-f(x)/(df(x0)/dx)
 * df(x0)/dx 表示f(x)的导数在x=x0时的值。 
 * 算法：
 * 1.先自定一个初值x0，作为a的平方根值。利用迭代公式求出一个x1。此值与真正的a的平方根值相比，误差很大。
 * 2.把新求得的x1代入x0中，准备用此新的x0再去求出一个新的x1.
 * 3.利用迭代公式再求出一个新的x1的值，也就是用新的x0又求出一个新的平方根值x1，此值将更趋近于真正的平方根值。
 * 4.比较前后两次求得的平方根值x0和x1，如果它们的差值小于我们指定的值，即达到我们要求的精度，则认为x1就是a的平方根值，去执行步骤5；否则执行步骤2，即循环进行迭代。
 *
 * 牛顿迭代法参考：
 * (1) 书p102，例5.12
 * (2) 书p138，例7.6  
 * 初值很关键，取得不恰当，有可能不收敛。
 见《二分法和牛顿迭代法求解非线性方程的比较及应用.pdf 》 
 *************************************************/ 
 void ch5_11()
{
    printf("ch5_11(),迭代法求a的平方根.\n");
    float x0,x,a;
    printf("请输入a\n");
    scanf("%f",&a);
    x = a; // 0 < 初值x0 < a 
    do{
        x0 = x;  
        x = (x0+a/x0)/2;
        printf("迭代过程,x0=%f,x=%f\n",x0,x); // 调试时观察迭代过程 
    }while(fabs(x-x0) >= 1E-5); 
    printf("%.2f的平方根=%f,迭代求得: %f\n",a,sqrt(a),x);
}

 
// 学习指导，p33,3.编程题 (32)
// 计算：1-1/2+1/3-1/4+...+1/99-1/100
void guide_p33()
{
    printf("===========guide_p33()\n");
	int i,n,s=1;   // i:循环变量； n： 总项数； s：正负号;
	float sum,k;   // sum：和; k: 代表每一项

	i=1; sum=0.0;
	printf("求n个数的和,输入n\n");
	scanf("%d",&n);
	while(i<=n)
	{ 
		k=(float)s/i;
		//k=s/i;  // 第一项，k=1；第二项k=0.5，两个整数相除，自动截断，k=0
		sum=sum+k;
		s=-s;
		i++;
	}

	printf("sum=%f\n",sum);
}

void ch5()
{   
    ch5_1(); // 计算: 1+1/3+1/5+...+1/51
    ch5_2(); // 计算: 1!+2!+3!+...+10!
    ch5_3(); // 统计正负整数。
    ch5_4(); // 求最小正整数
    ch5_5(); // 求最大公约数和最小公倍数  
    ch5_6(); // 输出图案
    ch5_7(); // 计算pi
    ch5_8(); // 九九乘法表
    ch5_9(); // 对阵名单
    ch5_10(); // 求100~150之间的素数
    ch5_11(); // 迭代法求a的平方根
    
    guide_p33(); 
}
