#include "program.h"

INT8U  Check_flag = 0;
extern INT8U Gas_check_Times;


void HardWare_Init(void)
{
      //TEST
      Usart_Init();//��ʼ������  
      Usart1_clear_init();//����BUF��ʼ��  
      TEV_GPIO_INIT();//��ʼ����ŷ����Ϳ���  
      HX712_GPIO_INIT();//hx712&ADC GPIO_init
     
      TIM3_Init();
      SPI_Initial();
      GPIO_Initial();//SPI_GPIO_INIT
  
      Air_detection_Init();//ADC��ʼ��        
      //��λCSx1212 
      SX1212_Init( );
      SX1212_EnterReceiveMode(  );//����ʹ��
      SX1212_SetMode( MODE_SLEEP );
        
}

void SX1212_SEND(void)
{
        HardWare_Init();//ϵͳ��ʼ��
        TEV_STATUS();//�����ѹ       
        //ReadVoltageValue();//��ȡ��Դ��ѹ          
        tx_ReadCount();
}

//�͹��������,�ر�hx711,����sleepģʽ
void LowPowerConsumption_Cmd(void)
{    
    HX712_CLK_H();
    delay_ms(1);    
    //״̬λ��ʼ��
    Gas_check_Times = 1;
    Check_flag = 0;

    halt(); 
    //�ı�ϵͳʱ��Ƶ��
    CLK_Config(CLK_SYSCLKSource_HSI);
}
//����ģʽ�� �̶����ܼ��
void StandyFun(void)
{
  if(Check_flag)    
  {   
    //10s ͨ�ż��
    SX1212_EnterReceiveMode();    
    //Hx712 ��ʼ״̬�趨
    HX712_CLK_L();
    Read_Init_Mode(ReadVoltage_Mode);
    delay_ms(10);  
    //������  ��ȡ��ص�ѹ������
    tx_ReadVoltage();
    
    //�����������͹رշ��ŵ�ָ��
    recv_sx1212_data();
  }
}

void Gas_CheckFun(void)
{
    if(!Gas_check_Times)
    {
      //����Ƿ�����ѹ
      if(Air_detection())
      {
        //����ѹ 10s���һ�� ���ϴ�
        Gas_check_Times = 1;
        tx_ReadCount();
      }
      else
      {
        //����ѹ 60s���һ��
        Gas_check_Times = 6;       
      }
    } 
    
  SX1212_SetMode( MODE_SLEEP );
  HX712_CLK_H();
  
}