

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


    
    
char* Int2String(int num,char *str)//10���� 
{
	int i = 0;//ָʾ���str 
	if(num<0)//���numΪ��������num���� 
	{
		num = -num;
		str[i++] = '-';
	} 
	//ת�� 
	do
	{
		str[i++] = num%10+48;//ȡnum���λ �ַ�0~9��ASCII����48~57������˵����0+48=48��ASCII���Ӧ�ַ�'0' 
		num /= 10;//ȥ�����λ	
	}while(num);//num��Ϊ0����ѭ��
	
	str[i] = '\0';
	
	//ȷ����ʼ������λ�� 
	int j = 0;
	if(str[0]=='-')//����и��ţ����Ų��õ��� 
	{
		j = 1;//�ӵڶ�λ��ʼ���� 
		++i;//�����и��ţ����Խ����ĶԳ���ҲҪ����1λ 
	}
	//�Գƽ��� 
	for(;j<i/2;j++)
	{
		//�Գƽ������˵�ֵ ��ʵ����ʡ���м��������a+b��ֵ��a=a+b;b=a-b;a=a-b; 
		str[j] = str[j] + str[i-1-j];
		str[i-1-j] = str[j] - str[i-1-j];
		str[j] = str[j] - str[i-1-j];
	} 
	
	return str;//����ת�����ֵ 

}

/* ʵ��itoa������Դ�� */ 
char *myitoa(int num,char *str,int radix) 
{  
	/* ������ */ 
	 char index[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"; 
	unsigned  unum; /* �м���� */ 
	int i=0,j,k; 
	/* ȷ��unum��ֵ */ 
	if(radix==10&&num<0) /* ʮ���Ƹ��� */ 
	{ 
		unum=(unsigned)-num; 
		str[i++]='-'; 
	} 
	else unum=(unsigned)num; /* ������� */ 
	/* ���� */ 
	do  
	{ 
		str[i++]=index[unum%(unsigned)radix]; 
		unum/=radix; 
	}while(unum); 
	str[i]='\0'; 
	/* ת�� */ 
	if(str[0]=='-') k=1; /* ʮ���Ƹ��� */ 
	else k=0; 
	/* ��ԭ���ġ�/2����Ϊ��/2.0������֤��num��16~255֮�䣬radix����16ʱ��Ҳ�ܵõ���ȷ��� */ 
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
	// �������ִ���
	my_itoa(int_part,str_int,10);
	// С�����ִ���
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