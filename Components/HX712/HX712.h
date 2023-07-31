#ifndef  __HX712_H__
#define  __HX712_H__

#include "stm8l15x.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "sprintf.h"
#include "mytypedef.h"
#include "data.h"
#include "SX1212.h"
/*************************************
*定义引脚//PD2--CLK
*          PD3--DOUT
***************************************/
 
#define Bat_threshold  910000    //大于910000 即电压值大于3.3

#define GAS_thresholdH  14000000    //大于14000000 即高气压
#define GAS_thresholdL  6000000    //小于13000000 即低气压

#define GAS_BAT_HIGH   (0XAA)
#define GAS_NORMAL (0XCC)
#define GAS_BAT_LOW  (0X55)

#define HX712_GPIO_PORT  GPIOD
#define HX712_CLK_PINS  GPIO_Pin_3
#define HX712_AIN_PINS  GPIO_Pin_1
#define HX712_DOUT_PINS  GPIO_Pin_2

#define HX712_CLK_L()       GPIO_ResetBits(HX712_GPIO_PORT,HX712_CLK_PINS)//CLK 信号为低电平
#define HX712_CLK_H()       GPIO_SetBits(HX712_GPIO_PORT,HX712_CLK_PINS)//CLK 信号为高电平

#define DOUT_Level()  GPIO_ReadInputDataBit(HX712_GPIO_PORT,HX712_DOUT_PINS)  //读取数据
#define AIN_Level()  GPIO_ReadInputDataBit(HX712_GPIO_PORT,HX712_AIN_PINS)  //读取数据

#define ReadVoltage_Mode 26
#define ReadCount_Mode 25

void HX712_GPIO_INIT (void);
unsigned char tx_ReadCount(void);
unsigned char tx_ReadVoltage(void);
void HX712_Init_Mode(INT8U Mode);
#endif




