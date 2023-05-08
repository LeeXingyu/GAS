#include "HX712.h"
                                                

/*********************************************************************************/
/*
函数名：iic_init
功  能：配置SDA 和SCL端口
参  数：无
返回值：无
*/
//初始化HX712

void HX712_GPIO_INIT (void)
{
   GPIO_Init(HX712_GPIO_PORT, HX712_CLK_PINS, GPIO_Mode_Out_PP_Low_Slow);//带上拉，推挽输出低电平
   GPIO_Init(HX712_GPIO_PORT, HX712_DOUT_PINS, GPIO_Mode_In_FL_No_IT);//带上拉，推挽输出低电平
   GPIO_Init(HX712_GPIO_PORT, HX712_AIN_PINS, GPIO_Mode_In_FL_No_IT);//带上拉，推挽输出低电平
}
//读取气压
unsigned long ReadCount(void)
{
  unsigned long Count;
  unsigned char i;
  
  HX712_CLK_L();
  Count = 0;
  //读取气体是否有气压
  while(DOUT_Level());
  
  for (i=0;i<24;i++)
  {
  HX712_CLK_H();
   Count=Count<<1;
   HX712_CLK_L();
   if(DOUT_Level()) Count++;
  }
  //第25个
  HX712_CLK_H();
  //按位异或  将高位MSB置一
  Count=Count^0x800000;
  HX712_CLK_L();
  // 转成 BAT
  delay_us();
  delay_us();
  HX712_CLK_H();
  delay_us();
  delay_us();
  HX712_CLK_L();
  return(Count);
}
//读取电压
unsigned long ReadVoltage(void)
{
  unsigned long CountVol;
  unsigned char i;
  
  HX712_CLK_L();
  CountVol = 0;
  //读取气体是否有气压
  while(DOUT_Level());
  
  for (i=0;i<24;i++)
  {
    HX712_CLK_H();
     CountVol=CountVol<<1;
     HX712_CLK_L();
     if(DOUT_Level()) CountVol++;
  }
  //第25个  转成差分输入
  HX712_CLK_H();
  //保证数据
  CountVol = CountVol & 0x7FFFFF;
  HX712_CLK_L();
  return(CountVol);
}

void Read_Init_Mode(INT8U Mode)
{
  unsigned char i;
  
  HX712_CLK_L();
  
  //读取气体是否有气压
  while(DOUT_Level());
  
  for (i=0;i<Mode;i++)
  {
     HX712_CLK_H();
     delay_us();
     delay_us();
     HX712_CLK_L();
   
  }
  HX712_CLK_L();
}

void tx_ReadCount(void)
{
  unsigned long  Count;
    
    Count = ReadCount();
    
    data.tx_data[0] = 0x43;
    data.tx_data[1] = 0x01;	   
    data.tx_data[2] = 0x01;
    data.tx_data[3] = 0x03;
    data.tx_data[4] = (INT8U)&Count;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        (data.tx_data,5);
}

void tx_ReadVoltage(void)
{
  unsigned long  CountVol;
    
    CountVol = ReadVoltage();

    data.tx_data[0] = 0x43;
    data.tx_data[1] = 0x01;	   
    data.tx_data[2] = 0x01;
    data.tx_data[3] = 0x03;
    data.tx_data[4] = (INT8U)&CountVol;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        (data.tx_data,5);
}




/*--------------------------------------------------------------------------------------------------------
                   							                  0ooo
                   								ooo0     (   )
                								(   )     ) /
                								 \ (     (_/
                								  \_)
----------------------------------------------------------------------------------------------------------*/






