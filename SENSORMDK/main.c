#include "stm8l15x.h"
#include "stm8l15x_it.h"
#include "stm8l15x_conf.h"
//Sys
#include "delay.h"
#include "rtc.h"
//Comp
#include "usart.h"
#include "ETC.h"
#include "SX1212.h"
#include "HX712.h"
#include "activehalt.h"
#include "SPI.h"
#include "MyTypeDef.h"
#include "data.h"
#include "timer.h"
#include "Rfm64.h"
#include "data.h"

/*============================================================================
*Function:  main
*Input   :  None
*Output  :  None
*Declare :	Sx1212 Pin Config		
                NSS_CFG		= PD5,	(Output)	Sx1212 SPI CONFIG enable	low lever  on
                NSS_DATA	= PD4,	(Output)	Sx1212 SPI DATA enable		low lever on
                IRQ0 		= PB4,	(Input)		Sx1212 Interrupt output 
                IRQ1 		= PB3,	(Input)		Sx1212 Interrupt output 
                MISO 		= PB7,	(Input)		Sx1212 SPI data output
                MOSI 		= PB6,	(Output)	Sx1212 SPI data input 
                SCK		= PB5,	(Output)	Sx1212 SPI clock input 
============================================================================*/

INT8U  Check_flag = 0;

INT8U  Function = 0;
extern INT8U Gas_check_Times;

void HardWare_Init(void); 
void LowPowerConsumption_Cmd(void);
void StandyFun_Cmd(void);



int main(void)
{
      disableInterrupts();
      PWR_FastWakeUpCmd(ENABLE);  //���ٻ���ʹ��	
      RTC_Config();//�����͹���      
      GPIO_Init_Colse();  //�ر�GPIO
      
      /*���Դ�������������*/
      //CLK_Config();//��ʼ��ϵͳʱ��         
      HardWare_Init();       
     
      enableInterrupts(); //�������ж�  
      PWR_UltraLowPowerCmd(ENABLE);//���͹���
         
      while(1)
      { 
        if(Function)
        {
          StandyFun_Cmd();
        }
        else
        {
          LowPowerConsumption_Cmd();
        }
      }
}

void HardWare_Init(void)
{
      //TEST
//    Usart_Init();//��ʼ������  
//    Usart1_clear_init();//����BUF��ʼ��  
      TEV_GPIO_INIT();//��ʼ����ŷ����Ϳ���  
      HX712_GPIO_INIT();
     
      TIM3_Init();
      SPI_Initial();
      GPIO_Initial();//spi_gpio_init
  
      VoltageValueinit();//ADC��ʼ��        
	//��λCSx1212 
      SX1212_Init( );
      SX1212_EnterReceiveMode(  );//����ʹ��
        
//    GPIO_Init(GPIOD, GPIO_Pin_7, GPIO_Mode_In_PU_No_IT);//LED  TEST 
}

void SX1212_SEND(void)
{
        HardWare_Init();//ϵͳ��ʼ��
        TEV_STATUS();//�����ѹ       
        ReadVoltageValue();//��ȡ��Դ��ѹ          
        tx_ReadCount();
}

//�͹��������,�ر�hx711,����sleepģʽ
void LowPowerConsumption_Cmd(void)
{
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM3,DISABLE);
    SX1212_SetMode( MODE_SLEEP );
    HX712_CLK_H();
    delay_ms(1);
    //�ر�RTC����   �����ⲿ�жϻ���
    RTC_ITConfig(RTC_IT_WUT, DISABLE);
    RTC_WakeUpCmd(DISABLE); 

    halt();   

}

void StandyFun_Cmd(void)
{
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM3,DISABLE);
  if(Check_flag)    
  {   
    //10s ͨ�ż��
    SX1212_EnterReceiveMode();    
    HX712_CLK_L();
    Read_Init_Mode(ReadVoltage_Mode);
    delay_ms(10);
    
    //������  ��ȡ��ص�ѹ
    tx_ReadVoltage();
    //�����������͹رշ��ŵ�ָ��
    recv_sx1212_data();
    
    if(!Gas_check_Times)
    {
      //����Ƿ�����ѹ
      if(AIN_Level())
      {
        //����ѹ 10s���һ�� ���ϴ�
        Gas_check_Times = 1;
        tx_ReadCount();
      }
      else
      {
        //����ѹ 60s���һ��
        Gas_check_Times = 6;       
      }
    } 
    //ReadVoltageValue();
  }
 
  SX1212_SetMode( MODE_SLEEP );
  HX712_CLK_H();
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM3,ENABLE);
}

/*--------------------------------------------------------------------------------------------------------
                   							                  0ooo
                   								ooo0     (   )
                								(   )     ) /
                								 \ (     (_/
                								  \_)
----------------------------------------------------------------------------------------------------------*/
