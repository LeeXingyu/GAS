#include "activehalt.h"

/*********************************************************
�������ܣ��رմ��ڣ�SPI��DMAʱ��
�������ƣ�Active_Halt_Colse
**********************************************************/
void Active_Halt_Colse(void)
{
  
//   USART_DeInit(USART1); //���½�USART1����Ϊȱʡֵ��USART1
//   CLK_PeripheralClockConfig (CLK_Peripheral_USART1,DISABLE);//�ر�USARTʱ�� 
   
//   DMA_DeInit(DMA1_Channel2);  //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
//   CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, DISABLE);    //�ر�ʱ�ӣ�����Ҫ   
   
   SPI_DeInit(SPI1);
   CLK_PeripheralClockConfig (CLK_Peripheral_SPI1,DISABLE);//�ر�SPIʱ��
   
  ADC_DeInit(ADC1);//�ڲ���׼��ѹʹ��
  CLK_PeripheralClockConfig (CLK_Peripheral_ADC1,DISABLE);//����ADCʱ��
    
   
   
}
/*********************************************************
�������ܣ��򿪴��ڣ�SPI��DMAʱ��
�������ƣ�Active_Halt_Open
**********************************************************/
void Active_Halt_Open(void)
{ 
//   CLK_PeripheralClockConfig (CLK_Peripheral_USART1,DISABLE);//�ر�USARTʱ��   
//   USART_DeInit(USART1); //���½�USART1����Ϊȱʡֵ��USART1

   CLK_PeripheralClockConfig (CLK_Peripheral_SPI1,DISABLE);//�ر�SPIʱ��
   SPI_DeInit(SPI1);
      
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
      GPIO_Init(GPIOC, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT);//����GPIOC  
      GPIO_Init(GPIOB, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT);//����GPIOC  
      GPIO_Init(GPIOA, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT);//����GPIOC  
      GPIO_Init(GPIOD, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT);//����GPIOC  

}

//test LED running
void LED_Run(void)
{
  delay_ms(1000);
  GPIO_ToggleBits(GPIOD,GPIO_Pin_3);//�ϵ�ָʾ���е�
  ReadVoltageValue();
}

//ʹ���ڲ���׼��ѹ����ʵ�ʵ�ѹֵ
void VoltageValueinit(void)
{
   
       CLK_PeripheralClockConfig (CLK_Peripheral_ADC1,ENABLE);//����ADCʱ��
       ADC_DeInit(ADC1);
       ADC_Init (ADC1,ADC_ConversionMode_Single,ADC_Resolution_12Bit,ADC_Prescaler_1);//ADC1�����β�����12λ��1��Ƶ
  
       ADC_SamplingTimeConfig(ADC1,ADC_Group_FastChannels,ADC_SamplingTime_384Cycles);//������������
       ADC_VrefintCmd(ENABLE);//�ڲ���׼��ѹʹ��
       ADC_ChannelCmd (ADC1,ADC_Channel_Vrefint,ENABLE);//ʹ���ڲ���׼��ѹ����ͨ��
       ADC_Cmd(ADC1,ENABLE);//ADC1ʹ�� 
}
void ReadVoltageValue(void)//��ȡ��Դ��ѹ
{
    u16 u16_adc1_value = 0;  
    float VoltageValue = 0;
  
       ADC_SoftwareStartConv (ADC1);//�������ת��
             
       while(!ADC_GetFlagStatus (ADC1,ADC_FLAG_EOC));//�ȴ�ת������
       ADC_ClearFlag (ADC1,ADC_FLAG_EOC);//�����ر�ʶ
       
       delay_ms(10);
       u16_adc1_value=ADC_GetConversionValue (ADC1);//��ȡת��ֵ
       //VoltageValue=u16_adc1_value*3300UL/4095UL;
       VoltageValue=(1.225* (u16_adc1_value))/4096;//���VDD��ѹ
 //      USART_SendData8(USART1,VoltageValue);
       
       	data.tx_data[0] = 0x43;
	data.tx_data[1] = 0x01;	   
	data.tx_data[2] = 0x01;
	data.tx_data[3] = 0x03;
        data.tx_data[4] = (INT8U) &VoltageValue;
        SX1212_SendPacket_Var(data.tx_data,5);
       
}

/*--------------------------------------------------------------------------------------------------------
                   							                  0ooo
                   								ooo0     (   )
                								(   )     ) /
                								 \ (     (_/
                								  \_)
----------------------------------------------------------------------------------------------------------*/
