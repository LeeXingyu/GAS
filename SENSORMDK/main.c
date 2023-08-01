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


extern int Rfm_Timer;
extern int Timer_times;

extern unsigned short Power_PreState;
extern unsigned short  Power_CurState;

extern unsigned char Bat_Check_state;
extern unsigned char  Gas_check_flag;

extern unsigned short SlaveGasCTRL;
uint8_t rst_data = 0;

enum
{
    n_IDTaskSend,    
    n_CtrlGasTask,
    n_SystemTask,
    n_GasTransmit,    
    n_BatTransmit
};

static volatile unsigned char D_SystemRun = 0;

int main(void)
{  
      PWR_FastWakeUpCmd(ENABLE);  //���ٻ���ʹ��	
      GPIO_Init_Colse();  //�ر�GPIO
      
      /*���Դ�������������*/
      //ֻʹ�����ڲ�����
      CLK_Config(CLK_SYSCLKSource_HSI);//��ʼ��ϵͳʱ�� HSI   16
      HardWare_Init(); 
      //��һ���ϵ����
      FirstPower_CheckService();
      
      Power_PreState = 0;
      Power_CurState = READ_Level();
      enableInterrupts();   
      
      while(1)
      { 
        switch(Power_CurState)
        {
          case 0 :
          {
            if(Power_CurState == 0)
            {            
                LowPowerStart();
                RTC_Config();
                while(Power_CurState == 0)
                {
                  Power_PreState = Power_CurState;
                  RTC_WakeUpCmd(ENABLE);
                  enableInterrupts(); //�������ж�  
                  PWR_UltraLowPowerCmd(ENABLE);//���͹���
                  halt();
                  Power_PreState = 0;
                }
            }
          }
          break;
          case 1:
          {
              if(Power_PreState != Power_CurState)
              {
                LowPowerStop();
                TIM3_Init(); 
                Rfm_Timer = 4; 
              }
              
              Power_PreState = Power_CurState;
              if((Rfm_Timer >= 4))
              {             
                  CLK_PeripheralClockConfig(CLK_Peripheral_TIM3,DISABLE);
                  Timer_times = 8;
                  Rfm_Timer = 0; 

                  Rcv_MasterDataParse();                
                  //������
                  Slave_Service(); 
                  
                  if(SlaveGasCTRL == 1)
                  {
                     //�������巧�Źر�ָ��
                      Cooker_SendGas_CTRL();
                  }
                  else
                  { 
                      //δ���յ��ر�����ָ�� �ر������ϱ�
                      Power_PreState = Power_CurState;
                      Power_CurState = READ_Level();        
                      if(Power_PreState != Power_CurState  && Power_PreState == 1)
                      {
                         Power_PreState = Power_CurState;
                         delay_ms(20);
                         if(!READ_Level())
                         {//���͹ر�������ָ��
                            Cooker_Parse_t entity;
                            entity.cmd	= eCOOKER_CTRL_Gas;
                            entity.payload[0]	= COOKER_PARSE_FALSE;
                            entity.length		= 1;
                            Slave_Load(&entity);             
                         }
                      }
                      //δ�ر�����  ������������Ϣ
                      if(Gas_check_flag == 0 && Power_CurState == 1)
                      {
                        Slave_GasBat_SendService();
                      }
                  }
                
                  SetRFMode( RF_SLEEP );
                  delay_ms(8);
                  HX712_CLK_H();
                  delay_ms(10);
                  CLK_PeripheralClockConfig(CLK_Peripheral_TIM3,ENABLE);
               }
           }  
          break;
        default:break;
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
