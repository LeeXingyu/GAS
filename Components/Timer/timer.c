#include "timer.h"

int Rfm_Times = 8;  //0.25*4
int  Check_flag = 0;
/*********************************************************
�������ܣ���ʱ��3��ʼ��
�������ƣ�TIM3_Init
          Ԥ��Ƶ 128 ��װ��ֵ 31250 16M/128/31250
**********************************************************/
void TIM3_Init(void)
{
   CLK_PeripheralClockConfig(CLK_Peripheral_TIM3,ENABLE); 
   TIM3_DeInit(); 
   //16M/128/62500 = 0.5s
   TIM3_TimeBaseInit(TIM3_Prescaler_128,TIM3_CounterMode_Up,31250);  
   TIM3_ARRPreloadConfig(ENABLE);  
   TIM3_ITConfig(TIM3_IT_Update, ENABLE);  
   TIM3_Cmd(ENABLE);
}

/*********************************************************
�������ܣ���ʱ��3��ʱ����
�������ƣ�TIM3_Handle
          ����Times�ﵽ��ʱʱ�����ı��־λ
**********************************************************/
void TIM3_Handle(void)
{
  if(Rfm_Times != 0)
  {
    Rfm_Times-- ;
  }
  else 
  {
    Check_flag ++;
    Rfm_Times = 8;
  }
}


