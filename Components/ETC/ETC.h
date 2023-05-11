#ifndef  __ETC_H__
#define  __ETC_H__

#include "stm8l15x.h"
//#include "stm8l15x_it.h"
#include "delay.h"
#include "usart.h"

#include <string.h>
#include <stdio.h>
#include "sprintf.h"
#include "mytypedef.h"
#include "SX1212.H"
//The electromagnetic valve
#define TEV_GPIO_PORT  GPIOB
#define TEV_READ_PINS  GPIO_Pin_1
#define EXTI_TEV_READ_PINS EXTI_Pin_1
#define TEV_CTRL_PINS  GPIO_Pin_0


#define QA_PowerH()   GPIO_SetBits(TEV_GPIO_PORT,TEV_CTRL_PINS)// 信号为高电平
#define QA_PowerL()   GPIO_ResetBits(TEV_GPIO_PORT,TEV_CTRL_PINS)// 信号为高电平

int READ_Level(void);
void TEV_CTRL_INIT(void);
void TEV_GPIO_INIT (void);
void TEV_STATUS(void);
#endif
