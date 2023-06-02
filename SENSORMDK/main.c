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


extern int  Check_flag;
extern int Rfm_Timer;
extern int Timer_times;
extern unsigned short Power_PreState;
extern unsigned short  Power_CurState;
extern unsigned int  BatCheck_Flag;
extern unsigned short  SlaveGasCTRL;
extern unsigned short SlaveSendSetIdResult;
uint8_t rst_data = 0;

enum
{
    n_SystemTask,
    n_IDTaskSend,
    n_CtrlGasTask,
    n_GasTransmit,    
    n_BatTransmit
};

static volatile unsigned char D_SystemRun;

int main(void)
{  
      PWR_FastWakeUpCmd(ENABLE);  //���ٻ���ʹ��	
      GPIO_Init_Colse();  //�ر�GPIO
      
      /*���Դ�������������*/
      //ֻʹ�����ڲ�����
      CLK_Config(CLK_SYSCLKSource_HSI);//��ʼ��ϵͳʱ�� LSI      
      HardWare_Init();      
      //FirstPower_CheckService();
      TIM3_Init();  
      while(1)
      { 
          
//        if(Power_CurState)
//        {   
//          Power_PreState = Power_CurState;
          //���رն�ʱ��  0.25s����
          if(Rfm_Timer == 1)
          {
              CLK_PeripheralClockConfig(CLK_Peripheral_TIM3,DISABLE);
              printf("\n times\n");
              
              Rcv_MasterDataParse();                
              Rfm_Timer = 0;             
              CLK_PeripheralClockConfig(CLK_Peripheral_TIM3,ENABLE);
         }
//          //10s��ʱ����  �Լ� ���յ����ݽ���
          if((Check_flag >= 5) || SlaveGasCTRL)
          {             
             CLK_PeripheralClockConfig(CLK_Peripheral_TIM3,DISABLE);
             SpiWriteCfg(REG_BITRATE_MSB,RF_BIRATE_19200_MSB);
	     SpiWriteCfg(REG_BITRATE_LSB, RF_BIRATE_19200_LSB);
             Timer_times = 8;
             Check_flag = 0;
             
             switch(D_SystemRun)
             {
                 case n_SystemTask: // ϵͳ����
                {   
                   Slave_Service();
                    D_SystemRun = n_IDTaskSend;
                }
                
                case n_IDTaskSend:
                {
                   // Cooker_SendSetIdResult();
                    D_SystemRun = n_GasTransmit;
                }
                                
                case n_GasTransmit:
                {
                    Slave_Send_GasState();
                    D_SystemRun = n_BatTransmit;
                } 
                
                case n_BatTransmit:
                {
                  if(BatCheck_Flag == 0)
                  {
                    Slave_Send_BatState();
                   }
                    D_SystemRun = n_CtrlGasTask;                  
                }
                 case n_CtrlGasTask:
                {
                  //���չر�ָ��ʱ ����
                    Cooker_SendGas_CTRL();
                    D_SystemRun = n_SystemTask;
                }
                break;

                default: // ϵͳʱ���쳣
                    D_SystemRun = n_SystemTask;
                    break;
              }   
             printf("\n Timerend \n");

            CLK_PeripheralClockConfig(CLK_Peripheral_TIM3,ENABLE);
         }         
//      }
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
