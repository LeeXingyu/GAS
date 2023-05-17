#include "activehalt.h"

/*********************************************************
�������ܣ��رմ��ڣ�SPI��DMAʱ��,��ʱ��3ʱ��
�������ƣ�Active_Halt_Colse
**********************************************************/
void Active_Halt_Colse(void)
{
  
  USART_DeInit(USART1); //���½�USART1����Ϊȱʡֵ��USART1
  CLK_PeripheralClockConfig (CLK_Peripheral_USART1,DISABLE);//�ر�USARTʱ��   

//  SPI_DeInit(SPI1);
//  CLK_PeripheralClockConfig (CLK_Peripheral_SPI1,DISABLE);//�ر�SPIʱ��

  ADC_DeInit(ADC1);//
  CLK_PeripheralClockConfig (CLK_Peripheral_ADC1,DISABLE);//����ADCʱ��

  TIM3_DeInit(); 
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM3,DISABLE); 
  
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM1,DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM2,DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4,DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_I2C1,DISABLE);
}
/*********************************************************
�������ܣ��򿪴��ڣ�SPI��DMAʱ��,��ʱ��3ʱ��
�������ƣ�Active_Halt_Open
**********************************************************/
void Active_Halt_Open(void)
{ 
  
   CLK_PeripheralClockConfig (CLK_Peripheral_USART1,ENABLE);//�ر�USARTʱ��   
   //USART_DeInit(USART1); //���½�USART1����Ϊȱʡֵ��USART1

//   CLK_PeripheralClockConfig (CLK_Peripheral_SPI1,ENABLE);//�ر�SPIʱ��
//   SPI_DeInit(SPI1);
   
   CLK_PeripheralClockConfig (CLK_Peripheral_ADC1,ENABLE);//����ADCʱ��
   ADC_DeInit(ADC1);//  
   
   CLK_PeripheralClockConfig(CLK_Peripheral_TIM3,ENABLE);
   TIM3_DeInit(); 

}

/*********************************************************
�������ܣ��ر�GPIO,����Ӳ����·������IO���ó���͹���ģʽ
�������ƣ�GPIO_Init_Colse
**********************************************************/
void GPIO_Init_Colse(void)
{
     //PC0��PC1�ӵ��Ǹߵ�ƽ����PC_ALL��
  GPIO_DeInit(GPIOB);
  GPIO_DeInit(GPIOA);
  GPIO_DeInit(GPIOC);
  GPIO_DeInit(GPIOD);
  
  GPIO_Init(GPIOA,GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow); 
  GPIO_Init(GPIOB,GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOC,GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOD,GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow);
  //GPIO_SetBits(GPIOD,GPIO_Pin_3);
  //GPIO_WriteBit
}

//test LED running
void LED_Run(void)
{
  delay_ms(1000);
  GPIO_ToggleBits(GPIOD,GPIO_Pin_3);//�ϵ�ָʾ���е�
  //ReadVoltageValue();
}



/*--------------------------------------------------------------------------------------------------------
                   							                  0ooo
                   								ooo0     (   )
                								(   )     ) /
                								 \ (     (_/
                								  \_)
----------------------------------------------------------------------------------------------------------*/
