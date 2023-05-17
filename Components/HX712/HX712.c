#include "HX712.h"
#include "usart.h"  

//static int TimeOut = 20000; //20ms
bool Time_flag = FALSE;

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
   
}
//��ȡ��ѹ
static unsigned long ReadCount(void)
{
  unsigned long Count;
  unsigned char i;
  
  HX712_CLK_L();
  Count = 0;
  //��ȡ�����Ƿ�����ѹ
  while(DOUT_Level()){};
  
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
  delay_us(1);
  delay_us(1);
  HX712_CLK_H();
  delay_us(1);
  delay_us(1);
  HX712_CLK_L();
  return(Count);
}
//��ȡ��Դ��ѹ
//���㷽ʽ����ǰ��ص��� * 3.2/(56 + 3.2) ��оƬ�ɼ�����AD_Valueֵ
//          CountVol =   AD_Value/(3.3/2)*(long int)(0x7FFFFF) 
static unsigned long ReadVoltage(void)
{
  unsigned long CountVol;
  unsigned char i;
  
  HX712_CLK_L();
  CountVol = 0;
  //��ȡBATת������
while(DOUT_Level()){};
  
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

void HX712_Init_Mode(INT8U Mode)
{
  unsigned char i;
  
  HX712_CLK_L();
  
  //ADת���Ƿ����
  while(DOUT_Level()){};
  
  for (i=0;i<Mode;i++)
  {
     HX712_CLK_H();
     delay_us(1);
     delay_us(1);
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
   //float Bat_Vol;
    CountVol = ReadVoltage();
    //С��3.3V��adֵʱ��������
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






