

#include "sprintf.h"


long myatoi( char *str)
{
  char *ps;
  long result = 0x00;

  for (ps = str; *ps; ps ++){
    result *= 10;
    result += *ps - '0';
  }
   return result;
}


    
    
char* Int2String(int num,char *str)//10进制 
{
	int i = 0;//指示填充str 
	if(num<0)//如果num为负数，将num变正 
	{
		num = -num;
		str[i++] = '-';
	} 
	//转换 
	do
	{
		str[i++] = num%10+48;//取num最低位 字符0~9的ASCII码是48~57；简单来说数字0+48=48，ASCII码对应字符'0' 
		num /= 10;//去掉最低位	
	}while(num);//num不为0继续循环
	
	str[i] = '\0';
	
	//确定开始调整的位置 
	int j = 0;
	if(str[0]=='-')//如果有负号，负号不用调整 
	{
		j = 1;//从第二位开始调整 
		++i;//由于有负号，所以交换的对称轴也要后移1位 
	}
	//对称交换 
	for(;j<i/2;j++)
	{
		//对称交换两端的值 其实就是省下中间变量交换a+b的值：a=a+b;b=a-b;a=a-b; 
		str[j] = str[j] + str[i-1-j];
		str[i-1-j] = str[j] - str[i-1-j];
		str[j] = str[j] - str[i-1-j];
	} 
	
	return str;//返回转换后的值 

}

/* 实现itoa函数的源码 */ 
char *myitoa(int num,char *str,int radix) 
{  
	/* 索引表 */ 
	 char index[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"; 
	unsigned  unum; /* 中间变量 */ 
	int i=0,j,k; 
	/* 确定unum的值 */ 
	if(radix==10&&num<0) /* 十进制负数 */ 
	{ 
		unum=(unsigned)-num; 
		str[i++]='-'; 
	} 
	else unum=(unsigned)num; /* 其它情况 */ 
	/* 逆序 */ 
	do  
	{ 
		str[i++]=index[unum%(unsigned)radix]; 
		unum/=radix; 
	}while(unum); 
	str[i]='\0'; 
	/* 转换 */ 
	if(str[0]=='-') k=1; /* 十进制负数 */ 
	else k=0; 
	/* 将原来的“/2”改为“/2.0”，保证当num在16~255之间，radix等于16时，也能得到正确结果 */ 
	char temp; 
	for(j=k;j<=(i-k-1)/2.0;j++) 
	{ 
		temp=str[j]; 
		str[j]=str[i-j-1]; 
		str[i-j-1]=temp; 
	} 
	return str; 
} 

static char table[]={'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
void num2char(char *str, double number, uint8_t g, uint8_t l)
{
    uint8_t i;
    int temp = number/1;
    double t2 = 0.0;
    for (i = 1; i<=g; i++)
    {
        if (temp==0)
            str[g-i] = table[0];
        else
            str[g-i] = table[temp%10];
        temp = temp/10;
    }
    *(str+g) = '.';
    temp = 0;
    t2 = number;
    for(i=1; i<=l; i++)
    {
        temp = t2*10;
        str[g+i] = table[temp%10];
        t2 = t2*10;
    }
    *(str+g+l+1) = '\0';
}
/*
int sprintf(char *buf,const char *fmt,...)
{
  int len;
  va_list va;
  va_start(va,fmt);
  len = vsprintf(buf,fmt,va);
  va_end(va);
  return len;
}
char* my_itoa(int value,char *str,int radix)
{
	int sign = 0;
	//char *s = str;
	char ps[32];
	memset(ps,0,32);
	int i=0;
	if(value < 0)
	{
		sign = -1;
		value = -value;
	}
	do
	{
		if(value%radix>9)
			ps[i] = value%radix +'0'+7;
		else 
			ps[i] = value%radix +'0';
		i++;
	}while((value/=radix)>0);
	if(sign<0)
		ps[i] = '-';
	else
		i--;
	for(int j=i;j>=0;j--)
	{
		str[i-j] = ps[j];
	}
	return str;
} 
char *my_ftoa(double number,int ndigit,char *buf)
{
    long int_part;
	double float_part;
	char str_int[32];
	char str_float[32];
	memset(str_int,0,32);
	memset(str_float,0,32);
	int_part = (long)number;
	float_part = number - int_part;
	// 整数部分处理
	my_itoa(int_part,str_int,10);
	// 小数部分处理
	if(ndigit>0)
	{
		float_part =fabs( pow(10,ndigit)*float_part);
		my_itoa((long)float_part,str_float,10);
	}
	int i = strlen(str_int);
	str_int[i] = '.';
	strcat(str_int,str_float);
	strcpy(buf,str_int);
	return buf;
}
*/



 
/*--------------------------------------------------------------------------------------------------------
                   									   0ooo
                   								ooo0     (   )
                								(   )     ) /
                								 \ (     (_/
                								  \_)
----------------------------------------------------------------------------------------------------------*/