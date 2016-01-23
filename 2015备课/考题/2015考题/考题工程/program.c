/************************
 * 4. 编程题 
 ************************/
 
 /////////// A4_1 ///////////////////////
 // 下编写C程序，从键盘输入10个元素的整型数组，计算所有元素值为偶数的元素的和，并打印输出之。 
 #include <stdio.h>
 #define N 10
 void A4_1_______( ) // 试题中用主函数 
 {
       int a[N],i; 
       float sum = 0.0; 
       
       printf("input array a,%d elements:\n",N); 
       for(i=0;i<N;i++)
       { 
         scanf("%d",&a[i]);
         if(a[i]%2==0) sum += a[i]; 
       }
       
       printf("sum=%f\n",sum);  
 } 
 
 
 #include <stdio.h>
 #define MAXlength 100
 // 编写C程序，从键盘输入n个元素的整型数组，不计一个最大值，不计一个最小值，求其余(n-2)个元素的平均值，并打印输出之。n也有键盘输入，使用宏定义数组的最大长度MAXlength为100。 
  void A4_1( ) // 试题中用主函数 
 {
       int a[MAXlength],i,n;
       int max,min; // 最大最小值 
       int sum = 0; // n-2个元素的和 
       float average; // n-2个元素的平均值 
       
       printf("元素个数(小于%d):\n",MAXlength);
       scanf("%d",&n); 
       
       for(i=0;i<n;i++)
       { 
         scanf("%d",&a[i]);
         if(i==0) max=min=a[0]; 
         if(a[i]>max) max=a[i];
         if(a[i]<min) min=a[i]; 
         sum += a[i]; 
       }
       sum = sum-max-min;
       average = (float)sum/(n-2); 
       printf("average=%f\n",average);  
 } 
 
 ////////// A4_2 ////////////////////// 
 struct time { int hour,minute,second; };
 
 struct time TimeTranslate(long totalSeconds)
 {
      struct time t;
      t.hour = totalSeconds/60/60%60; // 或 t.hour = totalSeconds/60/60;
      t.minute = totalSeconds/60%60;
      t.second = totalSeconds%60;
      return t;
 }
 
 /////////// A4_3 ///////////////////////
 
 /**********************************************
 * ch7,p156, 10. 编写函数factor(n),求n!
 * 再利用它求出表达式m!/(n!*(m-n)!)的值
 * 两种方法求n!:
 * 1. 循环语句， 2.递归调用
 * 下列两个函数均可 
 **********************************************/ 
long factor(int n)
{
     int i;
     long int fac = 1L;  // 如果参数n=0,下列循环语句不执行，将返回1L，符合阶乘的定义 
     for (i=1;i<=n;i++) fac *= i; 
     return fac;
} 

long factor1(int n)
{
     if (n == 0 || n == 1) return 1;
     return n*factor(n-1);
} 

void A4_3( )
{
    int m,n; 
    float f;
    
    printf("please input m,n:\n");
    scanf("%d%d",&m,&n);
    f = (float)factor1(m)/(factor(n)*factor(m-n)); // 注意，一定要强制类型转换，否则失去精度。 
    printf("表达式的值=%f\n",f); 
} 

 
 /////////// A4_4 ///////////////////////
 
/******************************************************
 * ch8,p192, 3. 编写一个函数，由实参传来一个字符串，统计此字符串中的字母、数字个空格的个数，并输出。
 * ch7,p155, 第5题用字符数组实现。
 *  指针参数：Letter，Number，Space，Other传递字母、数字、空格和其它字符的个数 
 ******************************************************/ 
void strCount(char *s, int *Letter, int *Number, int *Space, int *Other)
{    
     char c;
     *Letter = 0, *Number = 0, *Space = 0 , *Other = 0; 
     while((c = *s++) != 0){ // c = *s++相当于c = *s，s++ 
         if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) (*Letter)++;
         else if ('0' <= c && c <= '9') (*Number)++;
         else if (c == ' ') (*Space)++;
         else (*Other)++;
     } 
} 

void A4_4()
{
     char s[80];
     int Letter, Number, Space, Other;
     printf("please input string:\n");
     gets(s); // 可以接收含有空格的字符串
     strCount(s,&Letter,&Number,&Space,&Other);
     printf("\"%s\"中字母、数字、空格和其它字符的个数分别为:\n%d,%d,%d,%d\n",s,Letter,Number,Space,Other);
} 

/////////////// B4_2
struct student 
  {
       char name[10];
       int  num;
       int  age;
  } stu[10];
  
void inputStudent(struct student stu[])
{
     int i;
     for(i=0;i<10;i++) 
     {
        printf("请输入第%d个学生的姓名、学号和年龄:\n",i+1);
        scanf("%s%d%d",stu[i].name,&stu[i].num,&stu[i].age); 
     } 
}

/////////////// B4_3
// 计算：1-1/2+1/3-1/4+...+1/99-1/100
double sum(int n)
{
	int i=1,s = 1; // i:循环变量； s：正负号;
	double sum=0.0,k;   // sum：和; k: 代表每一项

	while(i<=n)
	{ 
		k=s*1.0/i;
		sum = sum+k;
		i++;
		s = -s; 
	}
	return sum;
}

void B4_3()
{
	int n;
    printf("请输入项数n:\n");
	scanf("%d",&n);
	printf("计算结果:%lf\n",sum(n));
}

///////////////////////// B4_4
// （1）不用库函数，求字符串长度，（2）统计字符串中含有特定字符的个数。
// 参数：
// char *s: 
int B4_4(char *s, char ch, int *Number)
{    
     char c;
     int len = 0; 
     *Number = 0;
     while((c = *s++) != 0){ // c = *s++相当于c = *s，s++ 
         len++;
         if(c == ch) (*Number)++; 
     } 
     return len;
} 

// 编程候选
/**************************
   来源于tc help tcstudy.chm  69题 【同学习指导，p73 (50)】 
   有n个人围成一圈，顺序排号。从第一个人开始报数（从1到3报数），凡报到3的人退出圈子，问最后留下的一位是原来第几号的那位。
   分析: 设置一数组，存放人员编号1~n；
         循环报数，退出的人，将其编号置0；
         循环终止条件：置0元素的个数，最大值是n-1个 
****************************/
#define MAX 100
void p1(int n)
{
     int person[MAX]; // 数组存放每个人的编号,1,2,3,...,n; 报数退出，相应元素置0 
     int i,k,m;  // i: 数组person下标，0~n-1; k:报数1~3，m:置0元素个数，最大值为n-1,作为循环终止条件 
     
     // 数组存放人员编号1~n；
     for(i=0;i<n;i++) person[i] = i+1; 
     i=m=k=0;
     // 循环报数 
     do 
     {
        if(person[i]!=0) k++;
        if(k==3) 
        {
            person[i] = 0;
            k = 0;
            m++;
        } 
        i++; 
        if(i==n) i=0;  // 最后一人接第一人 
     }while(m<n-1);
     
     // 最后只留一位 
     for(i=0;i<n;i++) 
        if(person[i]!=0) { 
          printf("%d ",person[i]);  
          break;
        }
     printf("\n");
}

/******
  来源于tc help tcstudy.chm  80题 
  海滩上有一堆桃子，五只猴子来分。
  第一只猴子把这堆桃子平均分为五份，多了一个，这只猴子把多的一个扔入海中，拿走了一份。
  第二只猴子把剩下的桃子又平均分成五份，又多了一个，它同样把多的一个扔入海中，拿走了一份，
  第三、第四、第五只猴子都是这样做的，
  问海滩上原来最少有多少个桃子？
  方法一：假设n个桃子，n=1~n, 从n=1开始试探计算，每次剩下4/5份           
*******/
void p2_1()
{
     int n=1,i; // n为桃子的个数,i=0~4,代表第1到第5个猴子分桃子 
     int m; // 分桃子前，将n暂存为m 
     while(1)
     {      
        for(i=0,m=n;i<5;i++) // 5个猴子分m个桃子 
        {
           if (m<6) break; // 不够分                   
           if(m%5==1) m=4*m/5; // 拿走m的1/5，剩下(4/5)*m,自动丢掉一个桃子(整数相除得整数),为了消除4/5=0,===> (m/5)*4 或 ==>4*m/5 
           else break; // 如果不能均分5份余1，则此时的m不符合题意 
        }
        if(i==5) // 找到n
        {
          printf("最少有%d个桃子.\n",n); // 3121
          break; 
        } 
        n++;
     }
     
}

// p2()的第二种解法
void p2_2()
{
  int i,m,j,k,count; // i:分一次桃前桃子的数量，j:每次分桃后桃子个数 
  for(i=4;i<10000;i+=4)
  {
    count=0;
    m=i; // 暂存i 
    for(k=0;k<5;k++)
    {
      // 分一次桃子后桃子的数量(i)=分一次桃子前桃子的数量(j)*4/5; //一个猴子拿走1/5，扔掉一个 
      // 因此，j = i/4*5 + 1 
      j=i/4*5+1; 
      i=j;
      if(j%4==0)
        count++;
      else
        break;
    }
    i=m;
    if(count==4)
    {
      printf("最少有%d个桃子.\n",j); //3121
      break;
    }
  }
}

// 递归分配桃子，n为待分配的桃子，k=1~5代表5个猴子的编号
// 返回值：0：不能按规则分配，1:每个猴子均按规则分配了桃子 
int p2_3_1(int n,int k)
{
    //printf("%d,%d\n",n,k); // 调试 
    if(n%5!=1)return 0;
    if(k==5 && n%5==1) return 1; 
    if(k<5) {
      k++;
      return p2_3_1(4*n/5,k); // 分配一次桃子后，剩下待分配的桃子是原来的4/5 
      // return p2_3_1(4*n/5,k++); // 不能在参数中使用k++，导致参数值为k=1, 
    }
    else return 0; 
} 
// 第三种解法，递归调用
void p2_3()
{
     int n = 0;
     while(1)
     {
        n++; 
        if(!p2_3_1(n,1)) continue; // 不能成功分配 
        else break; // 成功分配 
     }
     printf("最少有%d个桃子.\n",n); //3121
}

/**********************
 来源于tc help tcstudy.chm  81题
 809*??=800*??+9*??+1 其中??代表的两位数,8*??的结果为两位数，9*??的结果为3位数。
 求??代表的两位数，及809*??后的结果。
 **********************/
void p3_1()
{
     int a,b,e;//表示??的十位和个位，e(??)=a*10+b
     int c,d,ok;
     for(a=1;a<=9;a++)
     {
       ok = 0; // 未找出a,b 
       for(b=0;b<=9;b++)
       {
          e = a*10+b;
          c = 8*e;  // c是二位数 
          d = 9*e;  // c是三位数
          if((c>10 && c<=99) && (d>100 && d<=999))
           { ok = 1; break; } // 找出a,b 
       }
       if(ok) break; // 找出a,b 
     }
     if(ok) {
       printf("a,b=%d,%d,??=%d,809*??=%d\n",a,b,e,809*e); // a,b=1,2,??=12,809*??=9708
       printf("800*??+9*??+1=%d\n",800*e+9*e+1);  // 800*??+9*??+1=9709
     }
     else printf("error!!");    
} 
// p3_1的另一种解法 
void p3_2()
{
  long int a,b,i;
  a=809;
  for(i=10;i<100;i++)
  {
    b=i*a+1;
    if(b>=1000&&b<=10000&&8*i<100&&9*i>=100)
    printf("%ld/%ld=809*%ld+%ld\n",b,i,i,b%i); // 9709/12=809*12+1
  }
}

// 来源于tc help tcstudy.chm  82题
// 八进制转换为十进制
void p4()
{
  char *p,s[6];int n;
  p=s;
  puts("输入8进制数，0结束");
  while (1)
  { 
    gets(p);
    if(!strcmp(p,"0")) return; 
    n=0;
    while(*(p)!='\0')
    {
      n=n*8+*p-'0';
      p++;
    }
    printf("对应的十进制数=%d\n",n);
  }
} 

/***** 
 学习指导，p56, (46)
 输入整数n,k,输出n中从右端开始的第k个数字的值，将求n中右端第k个数字定义成函数。
 例如digit(2456,3)=4
 *****/
void p5_1(int n,int k)
{
     int i,m;
     for(i=1,m=n;i<=k-1;i++) // 把第k个数字保留为m的最后一位 
     {
        m = m/10;
     }
     printf("%d的右端第%d个数字=%d\n",n,k,m%10); 
} 

// p5_1()的递归解法
void p5_2(int n,int k)
{
     if(k==1)  printf("右端第k个数字是%d\n",n%10);
     else { k--; p5_2(n/10,k); }
}

/*********************************
 学习指导，p181,模拟填空题
 韩信点兵。韩信有对兵，他想知道有多少人，便让士兵排队报数；
 按从1至5报数，最末一个士兵报的数为1；
 按从1至6报数，最末一个士兵报的数为5；
 按从1至7报数，最末一个士兵报的数为4；
 按从1至11报数，最末一个士兵报的数为10； 
 计算韩信至少有多少兵。 
********************************/
void p6()
{
     int x = 1;
     int find = 0;
     for(x=1;find==0;x++)
     {
        if(x%5==1 && x%6==5 && x%7==4 && x%11==10)
        {
           printf("x=%d\n",x); // 2111
           find = 1; 
        }
     } 
} 

/***********************************
  学习指导p33
  求s=a+aa+aaa+...aa...a的值，其中a是一个数字。例如2+22+222+2222(此时n=4),n和a均由键盘输入。 
  **********************************/
void p7()
{
     int a,n,i,s,t;
     printf("input a,n:\n");
     scanf("%d%d",&a,&n);
     s=0; t=0;
     for(i=1;i<=n;i++)
     {
        t = t*10 + a;
        s += t;
     }
     printf("s=%d\n",s); 
} 

/**************************************
 学习指导，p33
 求水仙花数。如果一个三位数的个位数、十位数和百位数的立方和等于该数自身，则称该数为水仙花数。
 编程求出所有的水仙花数。 
 **************************************/
 void p8()
 {
      int i,j,k;// 百、十、个位
      int n=0; 
      for(i=1;i<=9;i++)
        for(j=0;j<=9;j++)
          for(k=0;k<=9;k++)
            if(i*100+j*10+k == i*i*i+j*j*j+k*k*k)
            { 
              n++;
              printf((n%5==0)?"%d\n":"%d ",i*100+j*10+k); // 水仙花数              
            } 
      printf("\nn=%d\n",n); // 4
 }

/****************************************** 
 来源于tc help tcstudy.chm  1题
 有1、2、3、4个数字，能组成多少个互不相同且无重复数字的三位数？都是多少？
 程序分析：可填在百位、十位、个位的数字都是1、2、3、4。组成所有的排列后再去
　　　　　　掉不满足条件的排列。 
*******************************************/
void p9()
{
  int i,j,k,count=0;
  
  for(i=1;i<=4;i++)   /*以下为三重循环*/
    for(j=1;j<=4;j++)
      for (k=1;k<=4;k++)
      {
        if (i!=k&&i!=j&&j!=k) /*确保i、j、k三位互不相同*/
        {
          count++;
          printf("%d,%d,%d\n",i,j,k);
        }  
      }
  printf("count=%d\n",count); // 24
}

/*****************************************
 来源于tc help tcstudy.chm  11题
 题目：古典问题：有一对兔子，从出生后第3个月起每个月都生一对兔子，小兔子长到第三个月
　　　后每个月又生一对兔子，假如兔子都不死，问每个月的兔子总数为多少？
 分析：兔子的规律为数列1,1,2,3,5,8,13,21....
 f1=f1+f2; f2=f1+f2;
 *****************************************/
void p10()
{
     int i;
     long f1,f2;
     f1=f2=1;
     for(i=1;i<=20;i++)
     {
        printf("%12ld%12ld",f1,f2); 
        if(i%2==0) printf("\n");
        f1=f1+f2;
        f2=f1+f2;
     } 
}

/**********************************************
 来源于tc help tcstudy.chm  14题
 将一个正整数分解质因数。例如：输入90,打印出90=2*3*3*5。

程序分析：对n进行分解质因数，应先找到一个最小的质数k，然后按下述步骤完成： 
(1)如果这个质数恰等于n，则说明分解质因数的过程已经结束，打印出即可。
(2)如果n != k，但n能被k整除，则应打印出k的值，并用n除以k的商,作为新的正整数n,
　重复执行第一步。
(3)如果n不能被k整除，则用k+1作为k的值,重复执行第一步。

质数（prime number）又称素数，有无限个。除了1和它本身以外不再有其他的因数；否则称为合数。
根据算术基本定理，每一个比1大的整数，要么本身是一个质数，要么可以写成一系列质数的乘积；
而且如果不考虑这些质数在乘积中的顺序，那么写出来的形式是唯一的。最小的质数是2。
************************************************/
void p11(int n)
{
     int i,k,m;
     int count = 0; // n的质数 
     
     printf("%d=",n); 
     // 求2~n中的质数(素数) 
     for(k=2;k<=n;k++)
     {
       m=sqrt(k);
       for(i=2;i<=m;i++)
         if(k%i==0) break; // 非素数 
       // k是素数
       if(i>=m+1)  
       {
          if(n%k==0) // 找到n的质数 
          { 
            count++; 
            printf(count>1?"*%d":"%d",k);
            
            if(n!=k) 
            { 
              n = n/k; // 新的n 
              k = 1;   // continue后执行k++; 使k=2 从头开始找n的最小质数，
              continue;
            } 
            else break;  // n本身就是一个质数，分解结束 
          }  
       } 
     }
     printf("\n");     
}
 
/************************************
  学习指导，p166,
  有5个人围在一起，问第5个人多大年纪，他说比第4个人大2岁；问第4个人，他说比第3个人大2岁；
问第3个人，他说比第2个人大2岁；问第2个人，他说比第1个人大2岁。第1个人说他自己10岁，问第5个人多大年龄。  
分析：
  设n为人员编号，age(n)=10,n=1时；age(n)=age(n-1)-2,n>1时 
*************************************/
int age(n)
{
    if(n==1) return 10;
    else return age(n-1)-2; 
} 

void program()
{
     /****
     printf("A4_1========\n"); 
     A4_1();
     
     printf("A4_2========\n"); 
     struct time t;
     t=TimeTranslate(37100);
     printf("h:m:s=%d:%d:%d\n",t.hour,t.minute,t.second);
     
     printf("A4_3========\n"); 
     A4_3();
      
     printf("A4_4========\n");  
     A4_4();
     
     printf("B4_2========\n"); 
     //inputStudent(stu);
     
     printf("B4_3========\n"); 
     B4_3();
     
     printf("B4_4========\n"); 
     int n,len;
     len=B4_4("1234 ab ", '4', &n);
     printf("%d,%d\n",len,n);
     
     printf("p1()========\n"); 
     p1(1); // 1
     p1(2); // 2
     p1(3); // 2
     p1(4); // 1
     p1(100); // 91 
     
     printf("p2_1()========\n"); 
     p2_1(); 
     
     printf("p2_2()========\n");
     p2_2();
     
     printf("p2_3()========\n");
     p2_3();  
     
     printf("p3_1()========\n");
     p3_1();  
     
     printf("p3_2()========\n");
     p3_2();
     
     printf("p4()========\n");
     //p4();
     
     printf("p5_1()========\n");
     p5_1(1234,3);
     
     printf("p5_2()========\n");
     p5_2(1234,3);

     printf("p6()========\n");
     p6();
         
     printf("p7()========\n");
     p7();
           
     printf("p8()========\n");
     p8();
     
     printf("age(n)========\n");
     int n;
     for(n=1;n<=5;n++) printf("age(%d)=%d\n",n,age(n));
     ****/
     
     printf("p9()========\n");
     p9();
     
     printf("p10()========\n");
     p10();
     
     printf("p11()========\n");
     //p11(90);  // 90=2*3*3*5
     p11(10);  // 10=2*5 
}


