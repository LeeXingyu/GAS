#include "activehalt.h"

/*********************************************************
函数功能：关闭串口，SPI，DMA时钟
函数名称：Active_Halt_Colse
**********************************************************/
void Active_Halt_Colse(void)
{
  
//   USART_DeInit(USART1); //重新将USART1设置为缺省值的USART1
//   CLK_PeripheralClockConfig (CLK_Peripheral_USART1,DISABLE);//关闭USART时钟 
   
//   DMA_DeInit(DMA1_Channel2);  //将DMA的通道1寄存器重设为缺省值
//   CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, DISABLE);    //关闭时钟，很重要   
   
   SPI_DeInit(SPI1);
   CLK_PeripheralClockConfig (CLK_Peripheral_SPI1,DISABLE);//关闭SPI时钟
   
  ADC_DeInit(ADC1);//内部基准电压使能
  CLK_PeripheralClockConfig (CLK_Peripheral_ADC1,DISABLE);//开启ADC时钟
    
   
   
}
/*********************************************************
函数功能：打开串口，SPI，DMA时钟
函数名称：Active_Halt_Open
**********************************************************/
void Active_Halt_Open(void)
{ 
//   CLK_PeripheralClockConfig (CLK_Peripheral_USART1,DISABLE);//关闭USART时钟   
//   USART_DeInit(USART1); //重新将USART1设置为缺省值的USART1

   CLK_PeripheralClockConfig (CLK_Peripheral_SPI1,DISABLE);//关闭SPI时钟
   SPI_DeInit(SPI1);
      
//   CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, DISABLE);    //关闭时钟，很重要  
//   DMA_DeInit(DMA1_Channel2);  //将DMA的通道1寄存器重设为缺省值
}

/*********************************************************
函数功能：关闭GPIO,根据硬件电路把所有IO配置成最低功耗模式
函数名称：GPIO_Init_Colse
**********************************************************/
void GPIO_Init_Colse(void)
{
     //PC0和PC1接的是高电平，即PC_ALL高
      GPIO_Init(GPIOC, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT);//拉高GPIOC  
      GPIO_Init(GPIOB, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT);//拉高GPIOC  
      GPIO_Init(GPIOA, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT);//拉高GPIOC  
      GPIO_Init(GPIOD, GPIO_Pin_All, GPIO_Mode_In_PU_No_IT);//拉高GPIOC  

}

//test LED running
void LED_Run(void)
{
  delay_ms(1000);
  GPIO_ToggleBits(GPIOD,GPIO_Pin_3);//上电指示运行灯
  ReadVoltageValue();
}

//使用内部基准电压测量实际电压值
void VoltageValueinit(void)
{
   
       CLK_PeripheralClockConfig (CLK_Peripheral_ADC1,ENABLE);//开启ADC时钟
       ADC_DeInit(ADC1);
       ADC_Init (ADC1,ADC_ConversionMode_Single,ADC_Resolution_12Bit,ADC_Prescaler_1);//ADC1，单次采样，12位，1分频
  
       ADC_SamplingTimeConfig(ADC1,ADC_Group_FastChannels,ADC_SamplingTime_384Cycles);//采样周期设置
       ADC_VrefintCmd(ENABLE);//内部基准电压使能
       ADC_ChannelCmd (ADC1,ADC_Channel_Vrefint,ENABLE);//使能内部基准电压测量通道
       ADC_Cmd(ADC1,ENABLE);//ADC1使能 
}
void ReadVoltageValue(void)//读取电源电压
{
    u16 u16_adc1_value = 0;  
    float VoltageValue = 0;
  
       ADC_SoftwareStartConv (ADC1);//开启软件转换
             
       while(!ADC_GetFlagStatus (ADC1,ADC_FLAG_EOC));//等待转换结束
       ADC_ClearFlag (ADC1,ADC_FLAG_EOC);//清除相关标识
       
       delay_ms(10);
       u16_adc1_value=ADC_GetConversionValue (ADC1);//获取转换值
       //VoltageValue=u16_adc1_value*3300UL/4095UL;
       VoltageValue=(1.225* (u16_adc1_value))/4096;//获得VDD电压
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
