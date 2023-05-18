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
      GPIO_Initial();//SPI_GPIO_INIT       

      //RFM64
      InitSX1212();
      SetRFMode(RF_SLEEP);
      delay_ms(10);        
}

//�����͹���
void LowPowerStart(void)
{  
    //ģ��͹���
    SetRFMode( RF_SLEEP );
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
    //��ʼ����ʱ�� 
    TIM3_Init();  
    //Air_detection_Init();//ADC��ʼ��      
    SetRFMode( RF_SLEEP );//תΪsleepģʽ   
    delay_ms(8);
}

//����ģʽ ����ʵ��
void StandyFun(void)
{    
     Rfm_Times = 40;  //��ʱ��10s����״̬λ
     Check_flag = 0;
         
    // ������
     if(!Gas_check_Times)
    {
      //Hx712 ��ʼ״̬�趨
      HX712_CLK_L();
      HX712_Init_Mode(ReadCount_Mode);
      delay_ms(10);
      //����Ƿ�����ѹ      
      if(tx_ReadCount() != GAS_BAT_LOW)
      {
        //����ѹ 10s���һ�� ���ϴ�
        Gas_check_Times = 1;       
      }
      else
      {
        //����ѹ 60s���һ��
        Gas_check_Times = 6; 
      }
      tx_ReadVoltage();
    }

}
