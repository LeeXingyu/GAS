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
extern INT8U  Check_flag;


int main(void)
{ 

      PWR_FastWakeUpCmd(ENABLE);  //快速唤醒使能	
      GPIO_Init_Colse();  //关闭GPIO
      
      /*测试打开以下两个函数*/
      //只使用了内部晶振
      CLK_Config(CLK_SYSCLKSource_HSI);//初始化系统时钟 LSI      
      HardWare_Init(); 
      unsigned char test_id[12];   
      unsigned char test_id2[12]; 
      while(1)
      { 
        unsigned char rc = ERROR;
        
//        Read_UID();
//       
//        if(State)
//        {       
//            if(Check_flag)    
//            {  
//              CLK_PeripheralClockConfig(CLK_Peripheral_TIM3,DISABLE);
//              StandyFun();
//              CLK_PeripheralClockConfig(CLK_Peripheral_TIM3,ENABLE); 
//              SX1212_SetMode( MODE_SLEEP );
//              HX712_CLK_H(); 
//            }   
//       ReceiveRfFrame((unsigned char *)(&RF_Pkt), sizeof(RF_Pkt), &rc);
       Rcv_MasterDataParse();
//	unsigned int count;
//        GetMasterId(test_id);
//	ReceiveRfFrame((unsigned char *)(&RF_Pkt), sizeof(RF_Pkt), &rc);
//	if(rc == OK)
//	{
//           GetMasterId(test_id2);
//		count = 0;
//		while (count < RF_Pkt.key)
//		{
//			Master_data_Prase(RF_Pkt.fill[count]);
//			count++;
//		}
//	}
        QA_PowerH();
        delay_ms(100);
        QA_PowerL();
        delay_ms(100); 
        
//
//       }
//        else
//        {
//          LowPowerStart();
//          LowPowerStop();
//        }

        
      }

      
}


/*--------------------------------------------------------------------------------------------------------
                   							                  0ooo
                   								ooo0     (   )
                								(   )     ) /
                								 \ (     (_/
                								  \_)
----------------------------------------------------------------------------------------------------------*/
