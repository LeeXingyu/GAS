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
      PWR_FastWakeUpCmd(ENABLE);  //���ٻ���ʹ��	
      GPIO_Init_Colse();  //�ر�GPIO
      
      /*���Դ�������������*/
      //ֻʹ�����ڲ�����
      CLK_Config(CLK_SYSCLKSource_HSI);//��ʼ��ϵͳʱ�� LSI      
      HardWare_Init(); 
      //�رչ��ܵ�ϵͳʱ��
      //USART_SendStr("init ok\n"); 
      Active_Halt_Colse();
      enableInterrupts(); //�������ж�  
      
      //State = 1;
      
      while(1)
      { 
        //QA_PowerH();//���ָʾ�ƽ����ж�
        //delay_ms(3000);
        //read =  GPIOB->IDR & 0x02;
        if(State)
        {
//          QA_PowerH();//���ָʾ�ƽ����ж�
//          delay_ms(50);
//          QA_PowerL();
//          delay_ms(50);
          //Hx712 ��ʼ״̬�趨
//          HX712_CLK_L();
//          Read_Init_Mode(ReadVoltage_Mode);
//          delay_ms(10);  
//          //������  ��ȡ��ص�ѹ������
//          tx_ReadVoltage();
//          USART_SendStr("tx_ReadVoltage1 ok\n");
//          HX712_CLK_L();
//
//                //����Ƿ�����ѹ
//          if(Air_detection())
//          {           
//            //����ѹ 10s���һ�� ���ϴ�            
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
          PWR_UltraLowPowerCmd(ENABLE);//���͹���
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
