#include "HX712.h"
                                                

/*********************************************************************************/
/*
��������iic_init
��  �ܣ�����SDA ��SCL�˿�
��  ������
����ֵ����
*/
//��ʼ��HX712

void HX712_GPIO_INIT (void)
{
   GPIO_Init(HX712_GPIO_PORT, HX712_CLK_PINS, GPIO_Mode_Out_PP_Low_Slow);//����������������͵�ƽ
   GPIO_Init(HX712_GPIO_PORT, HX712_DOUT_PINS, GPIO_Mode_In_FL_No_IT);//����������������͵�ƽ
   GPIO_Init(HX712_GPIO_PORT, HX712_AIN_PINS, GPIO_Mode_In_FL_No_IT);//����������������͵�ƽ
}
//��ȡ��ѹ
unsigned long ReadCount(void)
{
  unsigned long Count;
  unsigned char i;
  
  HX712_CLK_L();
  Count = 0;
  //��ȡ�����Ƿ�����ѹ
  while(DOUT_Level());
  
  for (i=0;i<24;i++)
  {
  HX712_CLK_H();
   Count=Count<<1;
   HX712_CLK_L();
   if(DOUT_Level()) Count++;
  }
  //��25��
  HX712_CLK_H();
  //��λ���  ����λMSB��һ
  Count=Count^0x800000;
  HX712_CLK_L();
  // ת�� BAT
  delay_us();
  delay_us();
  HX712_CLK_H();
  delay_us();
  delay_us();
  HX712_CLK_L();
  return(Count);
}
//��ȡ��ѹ
unsigned long ReadVoltage(void)
{
  unsigned long CountVol;
  unsigned char i;
  
  HX712_CLK_L();
  CountVol = 0;
  //��ȡ�����Ƿ�����ѹ
  while(DOUT_Level());
  
  for (i=0;i<24;i++)
  {
    HX712_CLK_H();
     CountVol=CountVol<<1;
     HX712_CLK_L();
     if(DOUT_Level()) CountVol++;
  }
  //��25��  ת�ɲ������
  HX712_CLK_H();
  //��֤����
  CountVol = CountVol & 0x7FFFFF;
  HX712_CLK_L();
  return(CountVol);
}

void Read_Init_Mode(INT8U Mode)
{
  unsigned char i;
  
  HX712_CLK_L();
  
  //��ȡ�����Ƿ�����ѹ
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






