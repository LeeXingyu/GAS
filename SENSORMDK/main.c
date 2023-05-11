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

int main(void)
{
      extern INT8U  State;
      
      disableInterrupts();
      PWR_FastWakeUpCmd(ENABLE);  //快速唤醒使能	
      GPIO_Init_Colse();  //关闭GPIO
      
      /*测试打开以下两个函数*/
      //只使用了内部晶振
      CLK_Config(CLK_SYSCLKSource_HSI);//初始化系统时钟 LSI      
      HardWare_Init(); 
      //关闭功能的系统时钟
      //USART_SendStr("init ok\n"); 
      Active_Halt_Colse();
      enableInterrupts(); //开启总中断  
      
      //State = 1;
      
      while(1)
      { 
        //QA_PowerH();//外接指示灯进行判断
        //delay_ms(3000);
        //read =  GPIOB->IDR & 0x02;
        if(State)
        {
//          QA_PowerH();//外接指示灯进行判断
//          delay_ms(50);
//          QA_PowerL();
//          delay_ms(50);
          //Hx712 初始状态设定
//          HX712_CLK_L();
//          Read_Init_Mode(ReadVoltage_Mode);
//          delay_ms(10);  
//          //待测试  读取电池电压并发送
//          tx_ReadVoltage();
//          USART_SendStr("tx_ReadVoltage1 ok\n");
//          HX712_CLK_L();
//
//                //检测是否有气压
//          if(Air_detection())
//          {           
//            //有气压 10s检测一次 并上传            
//            tx_ReadCount();
//            USART_SendStr("tx_ReadCount1 ok\n");
//          }
//          else USART_SendStr("tx_ReadCount2 ok\n");
 
          //CLK_PeripheralClockConfig(CLK_Peripheral_TIM3,DISABLE);
          //StandyFun();
          //Gas_CheckFun();
          //CLK_PeripheralClockConfig(CLK_Peripheral_TIM3,ENABLE);

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
