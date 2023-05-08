#include "ETC.h"
#include "stm8l15x_exti.h"
#include "stm8l15x_itc.h"

extern INT8U  Function;
//��ʼ����ŷ����Ϳ���                                         
void TEV_GPIO_INIT (void)
{
   GPIO_DeInit(GPIOB);
   GPIO_Init(TEV_GPIO_PORT, TEV_READ_PINS, GPIO_Mode_In_FL_IT);//����������������͵�ƽ
   EXTI_SetPinSensitivity(EXTI_TEV_READ_PINS, EXTI_Trigger_Rising_Falling);
   //GPIO_Init(TEV_GPIO_PORT, TEV_READ_PINS, GPIO_Mode_In_FL_IT);
   ITC_SetSoftwarePriority(EXTI1_IRQn, ITC_PriorityLevel_1);
   
   enableInterrupts();                                                 //ʹ��ȫ���ж�                                                                                               
   disableInterrupts();                                                //�ر��ж�
                                                                    //�����¼�
   EXTI_ClearITPendingBit(EXTI_IT_Pin1);                               //����жϱ�־λ                                               
   enableInterrupts();  

   GPIO_Init(TEV_GPIO_PORT, TEV_CTRL_PINS, GPIO_Mode_Out_PP_Low_Slow);//����������������͵�ƽ
}
//�����⵽����ѹ����رյ�ŷ�
 void TEV_STATUS(void)
 {
   INT8U txBuffer[5] = {0x43,0x01,0x01,0x02,0x00};//����ѹ
   if(READ_Level())       //1Ϊ��ͨ  0Ϊ�ر�
   {
     QA_PowerH();
     SX1212_SendPacket_Var(txBuffer,5);
   }
 }



/*--------------------------------------------------------------------------------------------------------
                   							                  0ooo
                   								ooo0     (   )
                								(   )     ) /
                								 \ (     (_/
                								  \_)
----------------------------------------------------------------------------------------------------------*/






