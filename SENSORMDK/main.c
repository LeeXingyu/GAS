#include "program.h"
#include "delay.h"

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

extern INT8U  State;




int main(void)
{

      PWR_FastWakeUpCmd(ENABLE);  //快速唤醒使能	
      GPIO_Init_Colse();  //关闭GPIO
      
      /*测试打开以下两个函数*/
      //只使用了内部晶振
      CLK_Config(CLK_SYSCLKSource_HSI);//初始化系统时钟 LSI      
      HardWare_Init(); 
      
      //关闭功能的系统时钟
      //USART_SendStr("init ok\n"); 
     // GPIO_Init_Colse();  //关闭GPIO
     // Active_Halt_Colse();     
      //State = 1;
      
      while(1)
      { 
        if(State)
        {  
          SX1212_SetMode(MODE_STDBY);
          //   SX1212_EnterReceiveMode(  );//接收使能     
          //StandyFun();
          //Gas_CheckFun();
        }
        else
        {
           SX1212_SetMode( MODE_SLEEP );//转为sleep模式
//          //QA_PowerH();
//          PWR_UltraLowPowerCmd(ENABLE);//超低功耗
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
