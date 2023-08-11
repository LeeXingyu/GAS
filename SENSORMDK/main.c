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


extern int Sleep_times;

extern unsigned short Power_PreState;
extern unsigned short  Power_CurState;

extern unsigned char Bat_Check_state;
extern unsigned char  Gas_check_flag;
extern unsigned char Gas_Pressure_state ;
extern unsigned char Gas_Check_Prestate ;

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

struct Fun_status
{
  unsigned char GasBat_status;    //������״̬
  unsigned char Value_status;
  unsigned char Slave_Check;
};

static volatile unsigned char D_SystemRun = 0;
struct Fun_status fun_status;


int main(void)
{ 
      PWR_FastWakeUpCmd(ENABLE);  //���ٻ���ʹ��	
      GPIO_Init_Colse();  //�ر�GPIO
      
      //ֻʹ�����ڲ�����
      CLK_Config(CLK_SYSCLKSource_HSI);//��ʼ��ϵͳʱ�� HSI   16
      //״̬λ��ʼ��
      fun_status.GasBat_status = 0;
      fun_status.Value_status = 0;
      fun_status.Slave_Check = 0;
      
      HardWare_Init(); 
      //��һ���ϵ���� �Լ� ���������
      FirstPower_CheckService();
      Power_PreState = 0;
      Power_CurState = READ_Level();
      
      
      if(Power_CurState == 1)
      {
        fun_status.Slave_Check = 1;
      }
      //����RTCʱ��
      RTC_Config((uint16_t)time60s);
      //enableInterrupts(); 
      
      while(1)
      { 
        //ִ��״̬ 0 halt  1 work
        if(fun_status.Slave_Check == 0)
        {
                LowPowerStart();                
        Sleep:   
                Power_PreState = Power_CurState;
                RTC_WakeUpCmd(ENABLE);
                enableInterrupts(); //�������ж�  

                PWR_UltraLowPowerCmd(ENABLE);//���͹���
                halt();                                            
                //δ���յ��ر�����ָ�� �ر������ϱ�      
                if(Power_PreState != Power_CurState  && Power_PreState == 1)
                {
                  fun_status.Value_status = 1;
                  goto SendingRecv_data;

                }
                
                if(Power_CurState == 0)
                { 
                  //δ�򿪿��أ�ִ��sleep
                  goto Sleep; 
                  
                }            
        }
        
        //�跢������ʱ  ִ�й�������
        SendingRecv_data: 
                //������ִ�е�ָ��
                LowPowerStop(); 
                RTC_WakeUpCmd(DISABLE);
                if(Power_CurState == 1 && Power_PreState == 0)
                {
                   Sleep_times = 0;
                   if(Power_CurState == 1)
                   {//���͹ر�������ָ��
                       Slave_Send_GasCtrl(COOKER_PARSE_TRUE);                                                 
                   } 
                   RTC_Config((uint16_t)time5s);
                }
               
                RTC_WakeUpCmd(DISABLE);                      
                Power_PreState = Power_CurState;
                //���պ���
                Rcv_MasterDataParse();                
                //������
                if(Sleep_times == 0)
                {          
                  Slave_Service(); 
                  fun_status.GasBat_status = 1;
                }
                //�������巧�Źر� ��Ϣ�ϱ�
                if(SlaveGasCTRL == 1)
                {                
                    Cooker_SendGas_CTRL();         
                    if(Power_CurState == 0)
                    {
                          Shutdown_GasSend();
                          //����RTCʱ��
                          RTC_Config((uint16_t)time60s);
                    } 
                    SlaveGasCTRL = 0;    
                }
                else
                { 
                    //�����ֶ��رս����ϱ�    
                    if(fun_status.Value_status == 1 || !READ_Level())
                    {
                       delay_ms(20);                      
                       Sleep_times = 1;
                       if(!READ_Level())
                       {//���͹ر�������ָ��
                         Slave_Send_GasCtrl(COOKER_PARSE_FALSE);
                       } 
                      if(!READ_Level())
                      {
                          Shutdown_GasSend();
                          //����RTCʱ��
                          RTC_Config((uint16_t)time60s);
                      } 
                      fun_status.Value_status = 0;
                    }                  
                }
               
                //δ�ر�����  ������������Ϣ
                if((fun_status.GasBat_status == 1) && (Power_CurState == 1))
                {
                  Slave_GasBat_SendService();
                  fun_status.GasBat_status = 0;
                }

                fun_status.Slave_Check = 0;
      }
}


/*--------------------------------------------------------------------------------------------------------
                   							                  0ooo
                   								ooo0     (   )
                								(   )     ) /
                								 \ (     (_/
                								  \_)
----------------------------------------------------------------------------------------------------------*/
