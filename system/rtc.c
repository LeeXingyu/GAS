#include "rtc.h"


/*******************************************************************************
****��ڲ�������
****���ڲ�������
****������ע��ʱ�����ú�������
****��Ȩ��Ϣ������Ƕ��ʽϵͳ
*******************************************************************************/
void CLK_Config(char SYSCLKSource)
{

    CLK_DeInit();
    CLK_HSICmd(ENABLE);//��ʼ�ڲ���ƵRC 16M
    CLK_SYSCLKSourceSwitchCmd(ENABLE);//ʹ��ʱ���л�
    CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);//1��Ƶ����8M
    CLK_SYSCLKSourceConfig(SYSCLKSource);//HSIΪϵͳʱ��
  while (CLK_GetSYSCLKSource() != SYSCLKSource)//�ȴ�ʱ���ȶ�
  {}

  /*
    // Select HSE as system clock source 
  CLK_SYSCLKSourceSwitchCmd(ENABLE);//ʹ��ʱ���л�
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_LSI);//ѡ���ⲿ����ʱ����Ϊʱ��Դ
  //system clock prescaler: 1
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);//����ϵͳʱ�ӷ�Ƶ
  while (CLK_GetSYSCLKSource() != CLK_SYSCLKSource_LSI)//�ȴ�ʱ���ȶ�
  {}
*/
}

/*******************************************************************************
****��ڲ�������
****���ڲ�������
****������ע��ʱ�����ú�������
****��ϸ˵����
              RTC_SetWakeUpCounter(47500);//���û���ʱ��20s
              CLK_RTCCLKDiv_1  ����ʱ��Ϊ��20s  ����
              CLK_RTCCLKDiv_2  ����ʱ��Ϊ��40s 
             CLK_RTCCLKDiv_4  ����ʱ��Ϊ��70s 
             CLK_RTCCLKDiv_8  ����ʱ��Ϊ��155s  2��35
            CLK_RTCCLKDiv_16  ����ʱ��Ϊ��307s 5��07
            CLK_RTCCLKDiv_64  ����ʱ��Ϊ��20min---25min
*******************************************************************************/

void RTC_Config(void)
{
  
  
        CLK_LSICmd(ENABLE);//ʹ��LSI
        CLK_RTCClockConfig(CLK_RTCCLKSource_LSI, CLK_RTCCLKDiv_2);//38K
       
        while (CLK_GetFlagStatus(CLK_FLAG_LSIRDY) == RESET);
        
        RTC_WakeUpCmd(DISABLE);
        
        CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);//RTCʱ���ſ�ʹ��
        RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);//38K/16/2=2375HZ t=0.842Ms
        delay_ms(500);//��ʱ
    
        RTC_ITConfig(RTC_IT_WUT, ENABLE);//���Ѷ�ʱ���ж�ʹ��      
        RTC_SetWakeUpCounter(11876);//���û���ʱ��10s 11876*(1/(38K/16/2)
        RTC_WakeUpCmd(ENABLE);//RTC����ʹ��
        
        enableInterrupts();
       
  /*
        CLK_LSEConfig(CLK_LSE_ON);
        while (CLK_GetFlagStatus(CLK_FLAG_LSERDY) == RESET);
        CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_1);//38K
        
        RTC_WakeUpCmd(DISABLE);
        CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);//RTCʱ���ſ�ʹ��
        RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);//38K/16=2375HZ t=0.421ms
        
        Delay(0xffff);//��ʱ һ��Ҫ��
    
        RTC_ITConfig(RTC_IT_WUT, ENABLE);//���Ѷ�ʱ���ж�ʹ��
        RTC_SetWakeUpCounter(3000);
        RTC_WakeUpCmd(ENABLE);//RTC����ʹ��

//      ITC_SetSoftwarePriority(RTC_IRQn,ITC_PriorityLevel_3);//���ȼ�
   */     
}



/*--------------------------------------------------------------------------------------------------------
                   							                  0ooo
                   								ooo0     (   )
                								(   )     ) /
                								 \ (     (_/
                								  \_)
----------------------------------------------------------------------------------------------------------*/
