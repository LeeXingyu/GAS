#include "activehalt.h"

/*********************************************************
�������ܣ��رմ��ڣ�SPI��DMAʱ��,��ʱ��3ʱ��
�������ƣ�Active_Halt_Colse
**********************************************************/
void Active_Halt_Colse(void)
{
  
  USART_DeInit(USART1); //���½�USART1����Ϊȱʡֵ��USART1
  CLK_PeripheralClockConfig (CLK_Peripheral_USART1,DISABLE);//�ر�USARTʱ�� 

  //   DMA_DeInit(DMA1_Channel2);  //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
  //   CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, DISABLE);    //�ر�ʱ�ӣ�����Ҫ   

  SPI_DeInit(SPI1);
  CLK_PeripheralClockConfig (CLK_Peripheral_SPI1,DISABLE);//�ر�SPIʱ��

  ADC_DeInit(ADC1);//
  CLK_PeripheralClockConfig (CLK_Peripheral_ADC1,DISABLE);//����ADCʱ��

  TIM3_DeInit(); 
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM3,DISABLE); 

}
/*********************************************************
�������ܣ��򿪴��ڣ�SPI��DMAʱ��,��ʱ��3ʱ��
�������ƣ�Active_Halt_Open
**********************************************************/
void Active_Halt_Open(void)
{ 
   CLK_PeripheralClockConfig (CLK_Peripheral_USART1,DISABLE);//�ر�USARTʱ��   
   USART_DeInit(USART1); //���½�USART1����Ϊȱʡֵ��USART1

   CLK_PeripheralClockConfig (CLK_Peripheral_SPI1,DISABLE);//�ر�SPIʱ��
   SPI_DeInit(SPI1);
   
   CLK_PeripheralClockConfig (CLK_Peripheral_ADC1,DISABLE);//����ADCʱ��
   ADC_DeInit(ADC1);//  
   
   CLK_PeripheralClockConfig(CLK_Peripheral_TIM3,DISABLE);
   TIM3_DeInit(); 
//   CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, DISABLE);    //�ر�ʱ�ӣ�����Ҫ  
//   DMA_DeInit(DMA1_Channel2);  //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
}

/*********************************************************
�������ܣ��ر�GPIO,����Ӳ����·������IO���ó���͹���ģʽ
�������ƣ�GPIO_Init_Colse
**********************************************************/
void GPIO_Init_Colse(void)
{
     //PC0��PC1�ӵ��Ǹߵ�ƽ����PC_ALL��
      GPIO_Init(GPIOC, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Fast);//����GPIOC  
      GPIO_Init(GPIOB, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Fast);//����GPIOC  
      GPIO_Init(GPIOA, GPIO_Pin_All, GPIO_Mode_In_FL_No_IT);//����GPIOC  
      GPIO_Init(GPIOD, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Fast);//����GPIOC  
      
      GPIO_SetBits(GPIOD,GPIO_Pin_3);

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
