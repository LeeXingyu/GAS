#include "program.h"

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


INT8U  State = 0;


int main(void)
{
      disableInterrupts();
      PWR_FastWakeUpCmd(ENABLE);  //快速唤醒使能	
      GPIO_Init_Colse();  //关闭GPIO
      
      /*测试打开以下两个函数*/
      //只使用了内部晶振
      CLK_Config(CLK_SYSCLKSource_LSI);//初始化系统时钟 LSI      
      HardWare_Init(); 
      
      Active_Halt_Colse();
      enableInterrupts(); //开启总中断  
      
         
      while(1)
      { 
        if(State)
        {
          CLK_PeripheralClockConfig(CLK_Peripheral_TIM3,DISABLE);
          StandyFun();
          Gas_CheckFun();
          CLK_PeripheralClockConfig(CLK_Peripheral_TIM3,ENABLE);
        }
        else
        {
          PWR_UltraLowPowerCmd(ENABLE);//超低功耗
          LowPowerStart();
          LowPowerStop();
        }
      }
}


/*--------------------------------------------------------------------------------------------------------
                   							                  0ooo
                   								ooo0     (   )
                								(   )     ) /
                								 \ (     (_/
                								  \_)
----------------------------------------------------------------------------------------------------------*/
