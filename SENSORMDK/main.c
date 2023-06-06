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
    n_IDTaskSend,    
    n_CtrlGasTask,
    n_SystemTask,
    n_GasTransmit,    
    n_BatTransmit
};

static volatile unsigned char D_SystemRun = 0;

int main(void)
{  
      PWR_FastWakeUpCmd(ENABLE);  //快速唤醒使能	
      GPIO_Init_Colse();  //关闭GPIO
      
      /*测试打开以下两个函数*/
      //只使用了内部晶振
      CLK_Config(CLK_SYSCLKSource_HSI);//初始化系统时钟 HSI   4分频   4M
      HardWare_Init();      
      FirstPower_CheckService();
      enableInterrupts(); 
      //TIM3_Init();       
      while(1)
      {   

        if(Power_CurState)
        {   
          Power_PreState = Power_CurState;
          //不关闭定时器 9s定时进入          
          if((Rfm_Timer >= 1))
          {             
             CLK_PeripheralClockConfig(CLK_Peripheral_TIM3,DISABLE);
             Timer_times = 8;
             Check_flag = 0;
             Rfm_Timer = 0; 

             //printf("\n times\n");
             Rcv_MasterDataParse();                
             
             switch(D_SystemRun)
             {               
                case n_IDTaskSend:
                {
                   // Cooker_SendSetIdResult();
                    D_SystemRun = n_CtrlGasTask;
                }
                case n_CtrlGasTask:
                {
                  //接收关闭指令时 发送
                    Cooker_SendGas_CTRL();
                    D_SystemRun = n_SystemTask;
                } 
                case n_SystemTask: // 检测服务
                {   
                   Slave_Service();
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
                    delay_ms(500);
                    Slave_Send_BatState();
                   }
                    D_SystemRun = n_IDTaskSend;                  
                } 
                break;
                default: // 系统时间异常
                    D_SystemRun = n_IDTaskSend;
                    break;
              }   
             //printf("\n Timerend \n");
              //模块低功耗
              SetRFMode( RF_SLEEP );
              delay_ms(8);
              HX712_CLK_H();
              delay_ms(10);
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
