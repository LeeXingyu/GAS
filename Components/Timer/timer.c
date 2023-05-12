#include "timer.h"

INT8U Rfm_Times = 40;  //0.25*40 = 10s
INT8U Gas_check_Times = 1;
extern INT8U  Check_flag;
/*********************************************************
函数功能：定时器3初始化
函数名称：TIM3_Init
          预分频 128 重装载值 31250 16M/128/31250
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
函数功能：定时器3定时处理
函数名称：TIM3_Handle
          根据Times达到定时时长，改变标志位
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


