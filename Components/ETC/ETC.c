#include "ETC.h"
#include "stm8l15x_exti.h"
#include "stm8l15x_itc.h"

extern INT8U  Function;
//初始化电磁阀检测和开关                                         
void TEV_GPIO_INIT (void)
{
   GPIO_DeInit(GPIOB);
   disableInterrupts(); 
   GPIO_Init(TEV_GPIO_PORT, TEV_READ_PINS, GPIO_Mode_In_FL_No_IT);//带上拉，推挽输出低电平
   
   EXTI_SetPinSensitivity(EXTI_TEV_READ_PINS, EXTI_Trigger_Rising_Falling);
   GPIO_Init(TEV_GPIO_PORT, TEV_READ_PINS, GPIO_Mode_In_FL_IT);
   //GPIO_Init(TEV_GPIO_PORT, TEV_READ_PINS, GPIO_Mode_In_FL_IT);
   //ITC_SetSoftwarePriority(EXTI1_IRQn, ITC_PriorityLevel_1);
   
   enableInterrupts();                                                 //使能全局中断                                                                                               
   disableInterrupts();                                                //关闭中断
                                                                    //处理事件
   EXTI_ClearITPendingBit(EXTI_IT_Pin1);                               //清除中断标志位                                               
   enableInterrupts();  
   
   
  
}

void TEV_CTRL_INIT(void)
{
   //GPIO_DeInit(GPIOB);
   GPIO_Init(TEV_GPIO_PORT, TEV_CTRL_PINS, GPIO_Mode_Out_PP_Low_Slow);//带上拉，推挽输出低电平
}


int READ_Level(void)
{
  if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1) != 0)
  {
        return 1;
  }
  else return  0;
}

//如果检测到有气压，则关闭电磁阀
 void TEV_STATUS(void)
 {
   INT8U txBuffer[5] = {0x43,0x01,0x01,0x02,0x00};//有气压
   if(READ_Level())       //1为导通  0为关闭
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






