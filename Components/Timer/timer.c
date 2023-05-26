#include "timer.h"

int Rfm_Times = 8;  //0.25*4
int  Check_flag = 0;
/*********************************************************
函数功能：定时器3初始化
函数名称：TIM3_Init
          预分频 128 重装载值 31250 16M/128/31250
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
函数功能：定时器3定时处理
函数名称：TIM3_Handle
          根据Times达到定时时长，改变标志位
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


