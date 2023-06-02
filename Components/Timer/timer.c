#include "timer.h"

int Timer_times = 8;  //0.25*4
int  Check_flag = 0;
int Rfm_Timer = 0;
static volatile unsigned int    usSystemTick = 0;
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
  if(Timer_times != 0)
  {
    Timer_times-- ;
  }
  else 
  {
    Check_flag++;
    Timer_times = 8;
    Rfm_Timer = 1;
    usSystemTick++;
  }
}

uint16_t Cooker_TimeGet(void)
{
	return usSystemTick;
}

