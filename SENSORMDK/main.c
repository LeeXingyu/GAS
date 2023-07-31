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
extern unsigned char Bat_Check_state;
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
      PWR_FastWakeUpCmd(ENABLE);  //快速唤醒使能	
      GPIO_Init_Colse();  //关闭GPIO
      
      /*测试打开以下两个函数*/
      //只使用了内部晶振
      CLK_Config(CLK_SYSCLKSource_HSI);//初始化系统时钟 HSI   16
      HardWare_Init(); 
      //第一次上电对码
      FirstPower_CheckService();
      
      Power_PreState = 0;
      Power_CurState = READ_Level();
      enableInterrupts();   
      
      while(1)
      { 
          if(Power_CurState == 0)
          {
              RTC_Config();
              LowPowerStart();
              while(Power_CurState == 0)
              {
                Power_PreState = Power_CurState;
                RTC_WakeUpCmd(ENABLE);
                enableInterrupts(); //开启总中断  
                PWR_UltraLowPowerCmd(ENABLE);//超低功耗
                halt();
                Power_PreState = 0;
              }
          }
          else
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
                 Check_flag = 0;
                 Rfm_Timer = 0; 

                 Rcv_MasterDataParse();                
                 //检测服务
                 Slave_Service(); 
                  
                  if(SlaveGasCTRL == 1)
                  {
                     //接收气体阀门关闭指令
                      Cooker_SendGas_CTRL();
                  }
                  else
                  {    
                      //发送气体
                      Slave_Send_GasState();
                      //发送电池
                      if(Bat_Check_state == GAS_BAT_LOW)
                      {
                        delay_ms(500);
                        Slave_Send_BatState();
                      }
                  }
                
                  SetRFMode( RF_SLEEP );
                  delay_ms(8);
                  HX712_CLK_H();
                  delay_ms(10);
                  CLK_PeripheralClockConfig(CLK_Peripheral_TIM3,ENABLE);
               }
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
