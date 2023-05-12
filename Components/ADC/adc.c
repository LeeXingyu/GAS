#include "adc.h"
#include "HX712.h"

void Air_detection_Init(void)
{
       GPIO_Init(HX712_GPIO_PORT, HX712_AIN_PINS, GPIO_Mode_In_FL_No_IT);//带上拉，推挽输出低电平
       CLK_PeripheralClockConfig (CLK_Peripheral_ADC1,ENABLE);//开启ADC时钟
       ADC_DeInit(ADC1);
       ADC_Init (ADC1,ADC_ConversionMode_Single,ADC_Resolution_12Bit,ADC_Prescaler_1);//ADC1，单次采样，12位，1分频
  
       ADC_SamplingTimeConfig(ADC1,ADC_Group_FastChannels,ADC_SamplingTime_384Cycles);//采样周期设置
       
       ADC_Cmd(ADC1,ENABLE);//ADC1使能 
       ADC_ChannelCmd(ADC1, ADC_Channel_21, ENABLE);
       //
       
       ADC_SoftwareStartConv(ADC1);
}

static INT16U Get_adc()
{

    ADC_SoftwareStartConv (ADC1);//开启软件转换
         
    while(!ADC_GetFlagStatus (ADC1,ADC_FLAG_EOC));//等待转换结束
    ADC_ClearFlag (ADC1,ADC_FLAG_EOC);//清除相关标识

    return (ADC_GetConversionValue (ADC1));//获取转换值

}

/*********************************************************
函数功能：ADC中值滤波
函数名称：GetMedianNum
**********************************************************/
static u16 GetMedianNum(u8 Value_Len)
{
  
	int i,j;// 循环变量
	u16 bTemp;
        u16 bArray[100] = {0};
        
	for (j = 0; j < Value_Len - 1; j ++)
	{
          bArray[j] = Get_adc();

	}
                
	// 用冒泡法对数组进行排序
	for (j = 0; j < Value_Len - 1; j ++)
	{
		for (i = 0; i < Value_Len - j - 1; i ++)
		{
			if (bArray[i] > bArray[i + 1])
			{
				// 互换
				bTemp = bArray[i];
				bArray[i] = bArray[i + 1];
				bArray[i + 1] = bTemp;
			}
		}
	}	
	// 计算中值
	// 数组有偶数个元素，返回中间两个元素平均值
	bTemp = (bArray[Value_Len / 2] + bArray[Value_Len / 2 + 1]) / 2;
	return bTemp;
}

INT8U Air_detection(void)//读取是否有气体
{
    u16 u16_adc1_value = 0;
    u16_adc1_value = GetMedianNum(50);
    
    if(u16_adc1_value < Gas_ADC_Threshold)   //小于无气压
    {
     return 0;
    }
    else return 1;       
}
