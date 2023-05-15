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
      HX712_CLK_H();
      delay_ms(1);
//
      SPI_Initial();
      GPIO_Initial();//SPI_GPIO_INIT       
//    ��λCSx1212 
      SX1212_Init( );
      SX1212_EnterReceiveMode(  );//����ʹ��
      SX1212_SetMode( MODE_SLEEP );//תΪsleepģʽ

      delay_ms(10);
    
      

        
}

//�����͹���
void LowPowerStart(void)
{  
    //ģ��͹���
    SX1212_SetMode( MODE_SLEEP );
    QA_PowerL();
    HX712_CLK_H();
    //�ر�ʱ��
    Active_Halt_Colse();
    //״̬λ��ʼ��
    Gas_check_Times = 1;
    Check_flag = 0;
    
    enableInterrupts(); //�������ж�  
    PWR_UltraLowPowerCmd(ENABLE);//���͹���
    halt(); 
}

//�˳��͹���
void LowPowerStop(void)
{
    //����ʱ��
    Active_Halt_Open();
 
    //��ʼ����ʱ�� sx1212 adc
    TIM3_Init();
//  
    Air_detection_Init();//ADC��ʼ��      
//    
    SPI_Initial();
    GPIO_Initial();//SPI_GPIO_INIT   
//  ��λCSx1212 
    SX1212_Init( );
    SX1212_EnterReceiveMode(  );//����ʹ��
//  SX1212_SetMode( MODE_SLEEP );//תΪsleepģʽ   
    delay_ms(8);
//  SX1212_SetMode( MODE_SLEEP );

}

//����ģʽ�� ������ͨѶ����
void StandyFun(void)
{
  if(Check_flag)    
  {  
     CLK_PeripheralClockConfig(CLK_Peripheral_TIM3,DISABLE);
     
     Rfm_Times = 40;  //��ʱ��10s����״̬λ
     Check_flag = 0;

     //10s ͨ�ż��
    //SX1212_EnterReceiveMode();    
    //Hx712 ��ʼ״̬�趨
    HX712_CLK_L();
    HX712_Init_Mode(ReadVoltage_Mode);
    delay_ms(10);  
    //��ȡ��ص�ѹ������
    tx_ReadVoltage();
    
    //�����������͹رշ��ŵ�ָ��
   // recv_sx1212_data();
  }
}
//����״̬���������趨
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
  
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM3,ENABLE);  
  SX1212_SetMode( MODE_SLEEP );
  HX712_CLK_H(); 
}