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

      PWR_FastWakeUpCmd(ENABLE);  //���ٻ���ʹ��	
      GPIO_Init_Colse();  //�ر�GPIO
      
      /*���Դ�������������*/
      //ֻʹ�����ڲ�����
      CLK_Config(CLK_SYSCLKSource_HSI);//��ʼ��ϵͳʱ�� LSI      
      HardWare_Init(); 
      unsigned char rc = ERROR;
      unsigned int count;
      while(1)
      { 
       
       
        if(State)
        {       
            if(Check_flag)    
            {  
              CLK_PeripheralClockConfig(CLK_Peripheral_TIM3,DISABLE);
              ReceiveRfFrame((unsigned char *)(&RF_Pkt), sizeof(RF_Pkt), &rc);
              if(rc == OK)
              {
                  count = 0;
                  while (count < RF_Pkt.key)
                  {
                        Master_data_Prase(RF_Pkt.fill[count]);
                        count++;
                  }
              }
              //���� ��ѹ���
              if(READ_Level())
              {
                delay_ms(6);
                if(READ_Level())
                {
                  StandyFun();
                }
              }
              SetRFMode( RF_SLEEP );
              HX712_CLK_H(); 
              CLK_PeripheralClockConfig(CLK_Peripheral_TIM3,ENABLE); 
            }   

      }
        else
        {
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
