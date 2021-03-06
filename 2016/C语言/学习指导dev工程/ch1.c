//#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

// ch1,p3,3. 算法题
/********************************
 * 百钱百鸡,已知公鸡5个钱1只，母鸡3个钱1只，小鸡1个钱3只，用100个钱买了100只鸡，
 * 问公鸡、母鸡、小鸡各几只? 
 *******************************/ 
void ch1_1()
{
     int x,y,z; // 公鸡、母鸡、小鸡个数
     int flag = 0;
     for(x=0;x<=100;x++) {
        for(y=0;y<=100;y++) {
          for(z=0;z<=100;z++) {
            if(5*x+3*y+z/3 == 100 && x+y+z == 100 && z%3 == 0) {
              printf("公鸡=%d、母鸡=%d、小鸡=%d\n",x,y,z);
              flag = 1;
            }
          }
        } 
     } 
     if(flag == 0) 
       printf("无解！\n");
} 

/*******************
 * 同作业： 
 * ch5, p106, 5. 输入两个正整数m和n，求其最大公约数和最小公倍数
  递归调用的辗除法,求两个整数的最大公约数.
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
 *******************/
int ch1_2(int a,int b)
{
     // 不用比较a,b大小，如果b>a, a%b=a,第二次调用，自然将a,b调换，即大数a,小数b
	int temp; 
	temp = a%b;
	if (temp==0) return b;
	else {
		return ch1_2(b,temp);
	}
}

/******************* 
 * 用pi/4=1-1/3+1/5-1/7+...公式，求pi的近似值，直到最后一项的绝对值小于10的-6次方为止。
 * 本题同书p100,例5.10 
 *******************/
void ch1_3()
{
     int i = 1, sign = 1;
     float item = 1.0, sum = 0.0, pi;
     do {
         item = 1.0/i;
         sum += sign*item;
         i += 2; 
         sign = - sign;
     }while(item > 1.0E-6); // 可以不用fabs(item),因为item>0 
     pi = sum*4.0;
     printf("pi = %f, i = %d\n",pi,i-2); 
}

/******************
 * 用迭代法求一元方程f(x)=0的根。 
 * 牛顿迭代法，y=f(x0)+(df(x0)/dx)*(x-x0),令y=0，得迭代公式x1=x0-f(x)/(df(x0)/dx)
 * df(x0)/dx 表示f(x)的导数在x=x0时的值。 
 * 初值很关键，取得不恰当，有可能不收敛。
 见《二分法和牛顿迭代法求解非线性方程的比较及应用.pdf 》 
 * 算法：
 * 1.先自定一个初值x0，利用迭代公式求出一个x1。此值与真正的a的平方根值相比，误差很大。
 * 2.把新求得的x1代入x0中，准备用此新的x0再去求出一个新的x1.
 * 3.利用迭代公式再求出一个新的x1的值，也就是用新的x0又求出一个新值x1，此值将更趋近于真正的方程的解。
 * 4.比较前后两次求得的根值x0和x1，如果它们的差值小于我们指定的值，即达到我们要求的精度，则认为x1就是a的平方根值，去执行步骤5；否则执行步骤2，即循环进行迭代。 
 * 
 * 参考：
 * (1) 书p102，例5.12
 * (2) 书p138，例7.6 
 * (3) 作业，ch5, p107, 11. 用迭代法求x=sqrt(a).求平方根的迭代公式为
 *                Xn+1 = (Xn+a/Xn)/2,
 *          要求前后两次求出的x的差的绝对值小于10的-5次方。 
 */
void ch1_4()
{
} 

//////////////////////////////////////////////////
// 其它排序法见课件及书p119,
// 排序法和迭代法是经典的算法，可以在互联网中查找算法，后续课程会涉及。 

/**********************
 * 输入一个十进制整数，转换成对应的二进制数和十六进制数并输出。
 * 10进制转2进制，除2取余至商为0，倒序排列。
 * 负数转码先将其作为正数，得原码，再转为补码。 
 * 负数是用补码表示的。 
 * 给出正负数在计算机内存中的保存格式的截图，并予以解释。
 * 正数的原码、反码、补码相同；补码是可逆的，即补码再求补得原码。 
 * [－25]原= (10011001)2     [－25]反= (11100110)2 
 * [－25]补 = [－25]反 + 1 = (11100110 +1) 2 = (11100111)2 
 * [－25]原= ([－25]补 )补 = (10011000+1)2= (10011001)2 
 * 
 **********************/ 
void ch1_25() 
{
     int a,b,i;
     char s[100];
     
     printf("请输入一个整数：\n");  
     scanf("%d",&a); 
     b = a;
     if(b < 0) a = -a; // 如果是负数，先将其转为正数， 
     i = 0;
     while(a!=0)
     {        
       s[i++]= a%2 + '0'; 
       a = a/2; 
     }
     s[i] = '\0';
     printf("%d,0x%x,正序=%s\n",b,b,s); // 25,0x19,10011
                                   // -25,0xffffffe7,10011

     // 倒序s
     int len = i; // = strlen(s)
     int temp;
     for(i=0;i<len/2;i++) { 
        temp = s[i];
        s[i] = s[len-i-1];
        s[len-i-1] = temp;
     }
     printf("%d,0x%x,倒序=%s\n",b,b,s); // 25,0x19,11001
                                        // -25,0xffffffe7,11001
     // 正整数到此为止。
                                    
     // 机内负数是补码表示的，以下转换负数的补码表示  
     if(b<0) {
        printf("负数(%d)的补码表示。\n",b); 
        // 负数的反码，符号位不变，其它位按位取反 
        // 最高位是符号位，在补足32位时置1表示负数 
        for(i=0;i<len;i++) {
          s[i] = s[i]=='1'?'0':'1';
        } 
        printf("%d,0x%x,反码=%s\n",b,b,s);
        
        // 补码=反码+1
        int f = 1;   // 进位标志
        int bit;     // 当前位 
        for(i=len-1;i>=0;i--) {                   
          if(s[i] == '1') { // 1
            if(f) { bit = '0'; f = 1; }
            else  { bit = '1'; f = 0; } 
          }
          else { // 0
            if(f) { bit = '1'; f = 0; }
            else  { bit = '0'; f = 0; } 
          } 
          s[i] = bit;
        } 
        printf("%d,0x%x,补码=%s\n",b,b,s);
        
        //补足32位(sizeof(int)*8)的补码，最高位一定是置1，表示负数。 
        char ss[100];
        if(len < sizeof(int)*8) {
          for(i=0;i<sizeof(int)*8;i++) {
             if(i < sizeof(int)*8-len) ss[i] = '1';
             else ss[i] = s[i-(sizeof(int)*8-len)];                
          }
          ss[i]='\0';
        } 
        printf("%d,0x%x,补足%d位的补码=%s\n",b,b,sizeof(int)*8,ss);
        // 每四位显示 
        for(i=0;i<sizeof(int)*8;i++){
          if(i%4 == 0 && i!=0) putchar(' ');
          putchar(ss[i]);
        }
        putchar('\n');
     }
}

/************************
 * 递归输出整数的二进制串 
 * 递归调用前计算余数，递归调用后输出，这样计算出的第一个余数反而在最后一个输出。 
 * ch1_25_1(-25); // 1111 1111 1111 1111 1111 1111 1110 0111
 * ch1_25_1(25);  // 11001
 ************************/
void ch1_25_1(unsigned int number)  // 负数输出其补码
//void ch1_25_1(int number) // 不能输出负数 
{
     int r;
     r = number%2;
     if(number >= 2)
       ch1_25_1(number/2); // 以堆栈结构存放于此 
     putchar(r + '0'); // 从栈中取出输出，即先进后出 
}
void ch1()
{
     printf("ch1()=========\n");
     ch1_1();
     //ch1_25();
     
     ch1_25_1(25);  // 11001
     putchar('\n');
     ch1_25_1(-25); //1111 1111 1111 1111 1111 1111 1110 0111
     putchar('\n'); 
} 
