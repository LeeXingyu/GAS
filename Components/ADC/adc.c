#include "adc.h"
#include "HX712.h"

void Air_detection_Init(void)
{
       GPIO_Init(HX712_GPIO_PORT, HX712_AIN_PINS, GPIO_Mode_In_FL_No_IT);//����������������͵�ƽ
       CLK_PeripheralClockConfig (CLK_Peripheral_ADC1,ENABLE);//����ADCʱ��
       ADC_DeInit(ADC1);
       ADC_Init (ADC1,ADC_ConversionMode_Single,ADC_Resolution_12Bit,ADC_Prescaler_1);//ADC1�����β�����12λ��1��Ƶ
  
       ADC_SamplingTimeConfig(ADC1,ADC_Group_FastChannels,ADC_SamplingTime_384Cycles);//������������
       
       ADC_Cmd(ADC1,ENABLE);//ADC1ʹ�� 
       ADC_ChannelCmd(ADC1, ADC_Channel_21, ENABLE);
       //
       
       ADC_SoftwareStartConv(ADC1);
}

static INT16U Get_adc()
{

    ADC_SoftwareStartConv (ADC1);//�������ת��
         
    while(!ADC_GetFlagStatus (ADC1,ADC_FLAG_EOC));//�ȴ�ת������
    ADC_ClearFlag (ADC1,ADC_FLAG_EOC);//�����ر�ʶ

    return (ADC_GetConversionValue (ADC1));//��ȡת��ֵ

}

/*********************************************************
�������ܣ�ADC��ֵ�˲�
�������ƣ�GetMedianNum
**********************************************************/
static u16 GetMedianNum(u8 Value_Len)
{
  
	int i,j;// ѭ������
	u16 bTemp;
        u16 bArray[100] = {0};
        
	for (j = 0; j < Value_Len - 1; j ++)
	{
          bArray[j] = Get_adc();

	}
                
	// ��ð�ݷ��������������
	for (j = 0; j < Value_Len - 1; j ++)
	{
		for (i = 0; i < Value_Len - j - 1; i ++)
		{
			if (bArray[i] > bArray[i + 1])
			{
				// ����
				bTemp = bArray[i];
				bArray[i] = bArray[i + 1];
				bArray[i + 1] = bTemp;
			}
		}
	}	
	// ������ֵ
	// ������ż����Ԫ�أ������м�����Ԫ��ƽ��ֵ
	bTemp = (bArray[Value_Len / 2] + bArray[Value_Len / 2 + 1]) / 2;
	return bTemp;
}

INT8U Air_detection(void)//��ȡ�Ƿ�������
{
    u16 u16_adc1_value = 0;
    u16_adc1_value = GetMedianNum(50);
    
    if(u16_adc1_value < Gas_ADC_Threshold)   //С������ѹ
    {
     return 0;
    }
    else return 1;       
}
