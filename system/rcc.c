#include "rtc.h"


/*******************************************************************************
****入口参数：无
****出口参数：无
****函数备注：非精确延时函数
****版权信息：蓝旗嵌入式系统
*******************************************************************************/
void Delay(__IO uint16_t nCount)
{
    /* Decrement nCount value */
    while (nCount != 0)
    {
        nCount--;
    }
}
/*******************************************************************************
****入口参数：无
****出口参数：无
****函数备注：时钟配置函数函数
****版权信息：蓝旗嵌入式系统
*******************************************************************************/
void CLK_Config(void)
{
  
  /* Select HSE as system clock source */
  CLK_SYSCLKSourceSwitchCmd(ENABLE);//使能时钟切换
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_LSI);//选择外部低速时钟作为时钟源
  /* system clock prescaler: 1*/
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);//设置系统时钟分频
  while (CLK_GetSYSCLKSource() != CLK_SYSCLKSource_LSI)//等待时钟稳定
  {}
}
void RTC_Config(void)
{
        CLK_LSEConfig(CLK_LSE_ON);
        while (CLK_GetFlagStatus(CLK_FLAG_LSERDY) == RESET);
        CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_1);//38K
        
        RTC_WakeUpCmd(DISABLE);
        CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);//RTC时钟门控使能
        RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);//38K/16=2375HZ t=0.421ms
        
        Delay(0xffff);//延时 一定要加
    
        RTC_ITConfig(RTC_IT_WUT, ENABLE);//唤醒定时器中断使能
        
        RTC_SetWakeUpCounter(3000);//设置唤醒时间
        RTC_WakeUpCmd(ENABLE);//RTC唤醒使能

        //ITC_SetSoftwarePriority(RTC_IRQn, ITC_PriorityLevel_3);//优先级
        
}
