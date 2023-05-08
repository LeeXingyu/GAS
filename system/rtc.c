#include "rtc.h"


/*******************************************************************************
****入口参数：无
****出口参数：无
****函数备注：时钟配置函数函数
****版权信息：蓝旗嵌入式系统
*******************************************************************************/
void CLK_Config(char SYSCLKSource)
{

    CLK_DeInit();
    CLK_HSICmd(ENABLE);//开始内部高频RC 16M
    CLK_SYSCLKSourceSwitchCmd(ENABLE);//使能时钟切换
    CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);//1分频，即8M
    CLK_SYSCLKSourceConfig(SYSCLKSource);//HSI为系统时钟
  while (CLK_GetSYSCLKSource() != SYSCLKSource)//等待时钟稳定
  {}

  /*
    // Select HSE as system clock source 
  CLK_SYSCLKSourceSwitchCmd(ENABLE);//使能时钟切换
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_LSI);//选择外部低速时钟作为时钟源
  //system clock prescaler: 1
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);//设置系统时钟分频
  while (CLK_GetSYSCLKSource() != CLK_SYSCLKSource_LSI)//等待时钟稳定
  {}
*/
}

/*******************************************************************************
****入口参数：无
****出口参数：无
****函数备注：时钟配置函数函数
****详细说明：
              RTC_SetWakeUpCounter(47500);//设置唤醒时间20s
              CLK_RTCCLKDiv_1  唤醒时间为：20s  勿用
              CLK_RTCCLKDiv_2  唤醒时间为：40s 
             CLK_RTCCLKDiv_4  唤醒时间为：70s 
             CLK_RTCCLKDiv_8  唤醒时间为：155s  2分35
            CLK_RTCCLKDiv_16  唤醒时间为：307s 5分07
            CLK_RTCCLKDiv_64  唤醒时间为：20min---25min
*******************************************************************************/

void RTC_Config(void)
{
  
  
        CLK_LSICmd(ENABLE);//使能LSI
        CLK_RTCClockConfig(CLK_RTCCLKSource_LSI, CLK_RTCCLKDiv_2);//38K
       
        while (CLK_GetFlagStatus(CLK_FLAG_LSIRDY) == RESET);
        
        RTC_WakeUpCmd(DISABLE);
        
        CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);//RTC时钟门控使能
        RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);//38K/16/2=2375HZ t=0.842Ms
        delay_ms(500);//延时
    
        RTC_ITConfig(RTC_IT_WUT, ENABLE);//唤醒定时器中断使能      
        RTC_SetWakeUpCounter(11876);//设置唤醒时间10s 11876*(1/(38K/16/2)
        RTC_WakeUpCmd(ENABLE);//RTC唤醒使能
        
        enableInterrupts();
       
  /*
        CLK_LSEConfig(CLK_LSE_ON);
        while (CLK_GetFlagStatus(CLK_FLAG_LSERDY) == RESET);
        CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_1);//38K
        
        RTC_WakeUpCmd(DISABLE);
        CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);//RTC时钟门控使能
        RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);//38K/16=2375HZ t=0.421ms
        
        Delay(0xffff);//延时 一定要加
    
        RTC_ITConfig(RTC_IT_WUT, ENABLE);//唤醒定时器中断使能
        RTC_SetWakeUpCounter(3000);
        RTC_WakeUpCmd(ENABLE);//RTC唤醒使能

//      ITC_SetSoftwarePriority(RTC_IRQn,ITC_PriorityLevel_3);//优先级
   */     
}



/*--------------------------------------------------------------------------------------------------------
                   							                  0ooo
                   								ooo0     (   )
                								(   )     ) /
                								 \ (     (_/
                								  \_)
----------------------------------------------------------------------------------------------------------*/
