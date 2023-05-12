#include "program.h"

INT8U  Check_flag = 0;
extern INT8U Rfm_Times;
extern INT8U Gas_check_Times ;

//�̼���ʼ��
void HardWare_Init(void)
{
      //TEST
 //     Usart_Init();//��ʼ������  
 //     Usart1_clear_init();//����BUF��ʼ�� 

      TEV_GPIO_INIT();//��ʼ����ŷ����Ϳ���  
      TEV_CTRL_INIT();
//      
      HX712_GPIO_INIT();//hx712&ADC GPIO_init
     
      TIM3_Init();
      
      HX712_CLK_H();
      delay_ms(1);
      QA_PowerL();
    //״̬λ��ʼ��
        
}

void SX1212_SEND(void)
{
        HardWare_Init();//ϵͳ��ʼ��
        TEV_STATUS();//�����ѹ       
        //ReadVoltageValue();//��ȡ��Դ��ѹ          
        tx_ReadCount();
}

//�����͹���
void LowPowerStart(void)
{    
    Active_Halt_Colse();
    Gas_check_Times = 1;
    Check_flag = 0;

    halt(); 
}
//�˳��͹���
void LowPowerStop(void)
{
    QA_PowerH();

    //����ʱ��
    Active_Halt_Open();
    //��ʼ����ʱ�� sx1212 adc
    TIM3_Init();
//      SPI_Initial();
//      GPIO_Initial();//SPI_GPIO_INIT
//  
      Air_detection_Init();//ADC��ʼ��        
//      ��λCSx1212 
//      SX1212_Init( );
//      SX1212_EnterReceiveMode(  );//����ʹ��
//      SX1212_SetMode( MODE_SLEEP );//תΪsleepģʽ
    
    delay_ms(10);
    //SX1212_SetMode( MODE_SLEEP );
    HX712_CLK_H();
    QA_PowerH();//���ָʾ�ƽ����ж�
    delay_ms(50);
    QA_PowerL();
    delay_ms(50);
}

//����ģʽ�� ������ͨѶ����
void StandyFun(void)
{
  if(Check_flag)    
  {  
     CLK_PeripheralClockConfig(CLK_Peripheral_TIM3,DISABLE);
     Rfm_Times = 40;
     Check_flag = 0;
     QA_PowerL();
     delay_ms(100);
    //10s ͨ�ż��
    //SX1212_EnterReceiveMode();    
    //Hx712 ��ʼ״̬�趨
    HX712_CLK_L();
    Read_Init_Mode(ReadVoltage_Mode);
    delay_ms(10);  
    //������  ��ȡ��ص�ѹ������
    tx_ReadVoltage();
    
    //�����������͹رշ��ŵ�ָ��
   // recv_sx1212_data();
  }
}
//����״̬���������趨
void Gas_CheckFun(void)
{
    char i = 0;
    if(!Gas_check_Times)
    {

      //����Ƿ�����ѹ
      if(Air_detection())
      {
        for(i = 0;i<50;i++)
        {
            QA_PowerH();//���ָʾ�ƽ����ж�
            delay_ms(50);
            QA_PowerL();
            delay_ms(10);
        }
        //����ѹ 10s���һ�� ���ϴ�
        Gas_check_Times = 1;
        tx_ReadCount();
      }
      else
      {
        //����ѹ 60s���һ��
        Gas_check_Times = 6; 
        QA_PowerH();//���ָʾ�ƽ����ж�
        delay_ms(50);
        QA_PowerL();
        delay_ms(50);
      }
    }
  
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM3,ENABLE);  
  //SX1212_SetMode( MODE_SLEEP );
  HX712_CLK_H(); 
}