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
      PWR_FastWakeUpCmd(ENABLE);  //快速唤醒使能	
      RTC_Config();//启动低功耗      
      GPIO_Init_Colse();  //关闭GPIO
      
      /*测试打开以下两个函数*/
      //CLK_Config();//初始化系统时钟         
      HardWare_Init();       
     
      enableInterrupts(); //开启总中断  
      PWR_UltraLowPowerCmd(ENABLE);//超低功耗
         
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
//    Usart_Init();//初始化串口  
//    Usart1_clear_init();//串口BUF初始化  
      TEV_GPIO_INIT();//初始化电磁阀检测和开关  
      HX712_GPIO_INIT();
     
      TIM3_Init();
      SPI_Initial();
      GPIO_Initial();//spi_gpio_init
  
      VoltageValueinit();//ADC初始化        
	//复位CSx1212 
      SX1212_Init( );
      SX1212_EnterReceiveMode(  );//接收使能
        
//    GPIO_Init(GPIOD, GPIO_Pin_7, GPIO_Mode_In_PU_No_IT);//LED  TEST 
}

void SX1212_SEND(void)
{
        HardWare_Init();//系统初始化
        TEV_STATUS();//检测气压       
        ReadVoltageValue();//读取电源电压          
        tx_ReadCount();
}

//低功耗情况下,关闭hx711,设置sleep模式
void LowPowerConsumption_Cmd(void)
{
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM3,DISABLE);
    SX1212_SetMode( MODE_SLEEP );
    HX712_CLK_H();
    delay_ms(1);
    //关闭RTC唤醒   利用外部中断唤醒
    RTC_ITConfig(RTC_IT_WUT, DISABLE);
    RTC_WakeUpCmd(DISABLE); 

    halt();   

}

void StandyFun_Cmd(void)
{
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM3,DISABLE);
  if(Check_flag)    
  {   
    //10s 通信检测
    SX1212_EnterReceiveMode();    
    HX712_CLK_L();
    Read_Init_Mode(ReadVoltage_Mode);
    delay_ms(10);
    
    //待测试  读取电池电压
    tx_ReadVoltage();
    //接受主机发送关闭阀门的指令
    recv_sx1212_data();
    
    if(!Gas_check_Times)
    {
      //检测是否有气压
      if(AIN_Level())
      {
        //有气压 10s检测一次 并上传
        Gas_check_Times = 1;
        tx_ReadCount();
      }
      else
      {
        //无气压 60s检测一次
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
