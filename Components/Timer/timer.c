#include "timer.h"

INT8U Rfm_Times = 40;  //0.25*40 = 10s
INT8U Gas_check_Times = 1;
extern INT8U  Check_flag;
/*********************************************************
�������ܣ���ʱ��3��ʼ��
�������ƣ�TIM3_Init
          Ԥ��Ƶ 128 ��װ��ֵ 31250 16M/128/31250
**********************************************************/
void TIM3_Init(void)
{
   CLK_PeripheralClockConfig(CLK_Peripheral_TIM3,ENABLE); 
   TIM3_DeInit(); 
   //16M/128/31250 = 0.5s
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
  if(Rfm_Times != 0x00)
  {
    Rfm_Times-- ;
    Check_flag = 0;
  }
  else 
  {
    Check_flag = 1;
    Gas_check_Times --;
    if(Gas_check_Times <= 0)
      Gas_check_Times = 0;
  }
}


