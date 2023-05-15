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

      PWR_FastWakeUpCmd(ENABLE);  //���ٻ���ʹ��	
      GPIO_Init_Colse();  //�ر�GPIO
      
      /*���Դ�������������*/
      //ֻʹ�����ڲ�����
      CLK_Config(CLK_SYSCLKSource_HSI);//��ʼ��ϵͳʱ�� LSI      
      HardWare_Init(); 
      
      //�رչ��ܵ�ϵͳʱ��
      //USART_SendStr("init ok\n"); 
     // GPIO_Init_Colse();  //�ر�GPIO
     // Active_Halt_Colse();     
      //State = 1;
      
      while(1)
      { 
        if(State)
        {  
          SX1212_SetMode(MODE_STDBY);
          //   SX1212_EnterReceiveMode(  );//����ʹ��     
          //StandyFun();
          //Gas_CheckFun();
        }
        else
        {
           SX1212_SetMode( MODE_SLEEP );//תΪsleepģʽ
//          //QA_PowerH();
//          PWR_UltraLowPowerCmd(ENABLE);//���͹���
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
