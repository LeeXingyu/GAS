#include "HX712.h"
#include "usart.h"  

//static int TimeOut = 20000; //20ms
bool Time_flag = FALSE;

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
   
}
//读取气压
static unsigned long ReadCount(void)
{
  unsigned long Count;
  unsigned char i;
  
  HX712_CLK_L();
  Count = 0;
  //读取气体是否有气压
  while(DOUT_Level())
  {
   /* TimeOut--;
    delay_us();
    if(!TimeOut) 
    {
      USART_SendStr("ReadCount timeout\n");
      Time_flag = FALSE;
      return (Count);
    }
    else Time_flag = TRUE;*/
    
  };
  
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
//读取电源电压
//计算方式：当前电池电量 * 3.2/(56 + 3.2) 是芯片采集到的AD_Value值
//          CountVol =   AD_Value/(3.3/2)*(long int)(0x7FFFFF) 
static unsigned long ReadVoltage(void)
{
  unsigned long CountVol;
  unsigned char i;
  
  HX712_CLK_L();
  CountVol = 0;
  //读取BAT转换数据
  while(DOUT_Level())
  {
/*    TimeOut--;
    delay_us();
    if(!TimeOut) 
    {
      USART_SendStr("ReadVoltage timeout\n");
      Time_flag = FALSE;
      return (CountVol);
    }
    else Time_flag = TRUE;*/
  };
  
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
  
  //AD转换是否完成
    while(DOUT_Level())
  {
   /* TimeOut--;
    delay_us();
    if(!TimeOut) 
    {
      USART_SendStr("Read_Init_Mode timeout\n");
      Time_flag = FALSE;
    }
    else Time_flag = TRUE;*/
  };
  
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
    if(Time_flag == TRUE)
    {
      data.tx_data[0] = 0x43;
      data.tx_data[1] = 0x01;	   
      data.tx_data[2] = 0x01;
      data.tx_data[3] = 0x03;
      data.tx_data[4] = (INT8U)&Count;   
      delay_ms(10);
      //SX1212_SendPacket_Var(data.tx_data,5);
    }                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     (data.tx_data,5);
}

void tx_ReadVoltage(void)
{
   unsigned long  CountVol;
   float Bat_Vol;
    CountVol = ReadVoltage();
    //小于3.3V的ad值时发送数据
    //Bat_Vol = (float)(((uint32_t)CountVol)/8388607*(3.3/2)/3.2*(56+3.2));
    if(CountVol <= Bat_threshold)
    {

      data.tx_data[0] = 0x43;
      data.tx_data[1] = 0x01;	   
      data.tx_data[2] = 0x01;
      data.tx_data[3] = 0x03;
      data.tx_data[4] = (INT8U)&CountVol;
      delay_ms(10);
     // SX1212_SendPacket_Var(data.tx_data,5);
    }                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                (data.tx_data,5);
}




/*--------------------------------------------------------------------------------------------------------
                   							                  0ooo
                   								ooo0     (   )
                								(   )     ) /
                								 \ (     (_/
                								  \_)
----------------------------------------------------------------------------------------------------------*/






