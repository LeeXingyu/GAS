#include "nvic.h"


/*******************************************************
  函数名称：NVIC_Configuration
	功    能：设置TIM6，USART1,USART4等优先级
	参    数：
	返    回：
	说    明：
	时    间：2017-07-10
*******************************************************/

void NVIC_Configuration(void) 
{	
  /*
  NVIC_InitTypeDef NVIC_InitStructure;

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);	

	//TIM4-系统心跳定时器
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;//TIM4_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
*/
}
