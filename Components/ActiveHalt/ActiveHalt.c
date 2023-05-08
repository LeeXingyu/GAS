#include "activehalt.h"

/*********************************************************
函数功能：关闭串口，SPI，DMA时钟,定时器3时钟
函数名称：Active_Halt_Colse
**********************************************************/
void Active_Halt_Colse(void)
{
  
  USART_DeInit(USART1); //重新将USART1设置为缺省值的USART1
  CLK_PeripheralClockConfig (CLK_Peripheral_USART1,DISABLE);//关闭USART时钟 

  //   DMA_DeInit(DMA1_Channel2);  //将DMA的通道1寄存器重设为缺省值
  //   CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, DISABLE);    //关闭时钟，很重要   

  SPI_DeInit(SPI1);
  CLK_PeripheralClockConfig (CLK_Peripheral_SPI1,DISABLE);//关闭SPI时钟

  ADC_DeInit(ADC1);//
  CLK_PeripheralClockConfig (CLK_Peripheral_ADC1,DISABLE);//开启ADC时钟

  TIM3_DeInit(); 
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM3,DISABLE); 

   
}
/*********************************************************
函数功能：打开串口，SPI，ADC
函数名称：Active_Halt_Open
**********************************************************/
void Active_Halt_Open(void)
{ 
   CLK_PeripheralClockConfig (CLK_Peripheral_USART1,DISABLE);//关闭USART时钟   
   USART_DeInit(USART1); //重新将USART1设置为缺省值的USART1

   CLK_PeripheralClockConfig (CLK_Peripheral_SPI1,DISABLE);//关闭SPI时钟
   SPI_DeInit(SPI1);
   
   CLK_PeripheralClockConfig (CLK_Peripheral_ADC1,DISABLE);//开启ADC时钟
   ADC_DeInit(ADC1);//  
   
   CLK_PeripheralClockConfig(CLK_Peripheral_TIM3,DISABLE);
   TIM3_DeInit(); 
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
  //ReadVoltageValue();
}


void Air_detection_Init(void)
{
   
       CLK_PeripheralClockConfig (CLK_Peripheral_ADC1,ENABLE);//开启ADC时钟
       ADC_DeInit(ADC1);
       ADC_Init (ADC1,ADC_ConversionMode_Single,ADC_Resolution_12Bit,ADC_Prescaler_1);//ADC1，单次采样，12位，1分频
  
       ADC_SamplingTimeConfig(ADC1,ADC_Group_FastChannels,ADC_SamplingTime_384Cycles);//采样周期设置
       
       ADC_ChannelCmd(ADC1, ADC_Channel_21, ENABLE);
       //
       ADC_Cmd(ADC1,ENABLE);//ADC1使能 
       ADC_SoftwareStartConv(ADC1);
}

INT8U Air_detection(void)//读取是否有气体
{
    u16 u16_adc1_value = 0;  
    //float VoltageValue = 0;

    ADC_SoftwareStartConv (ADC1);//开启软件转换
         
    while(!ADC_GetFlagStatus (ADC1,ADC_FLAG_EOC));//等待转换结束
    ADC_ClearFlag (ADC1,ADC_FLAG_EOC);//清除相关标识

    delay_ms(10);
    u16_adc1_value=ADC_GetConversionValue (ADC1);//获取转换值
    //VoltageValue=u16_adc1_value*3300UL/4095UL;

    //VoltageValue = (3300* (u16_adc1_value))/4096;//获取实际电压值

    if(u16_adc1_value < 1490)   //小于1490 无气压
    {
     return 0;
    }
    else return 1;       
}

/*--------------------------------------------------------------------------------------------------------
                   							                  0ooo
                   								ooo0     (   )
                								(   )     ) /
                								 \ (     (_/
                								  \_)
----------------------------------------------------------------------------------------------------------*/
