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
*��������//PD2--CLK
*          PD3--DOUT
***************************************/
 
#define Bat_threshold  910000    //����910000 ����ѹֵ����3.3

#define GAS_thresholdH  14000000    //����14000000 ������ѹ
#define GAS_thresholdL  6000000    //С��13000000 ������ѹ

#define GAS_BAT_HIGH   (0XAA)
#define GAS_NORMAL (0XCC)
#define GAS_BAT_LOW  (0X55)

#define HX712_GPIO_PORT  GPIOD
#define HX712_CLK_PINS  GPIO_Pin_3
#define HX712_AIN_PINS  GPIO_Pin_1
#define HX712_DOUT_PINS  GPIO_Pin_2

#define HX712_CLK_L()       GPIO_ResetBits(HX712_GPIO_PORT,HX712_CLK_PINS)//CLK �ź�Ϊ�͵�ƽ
#define HX712_CLK_H()       GPIO_SetBits(HX712_GPIO_PORT,HX712_CLK_PINS)//CLK �ź�Ϊ�ߵ�ƽ

#define DOUT_Level()  GPIO_ReadInputDataBit(HX712_GPIO_PORT,HX712_DOUT_PINS)  //��ȡ����
#define AIN_Level()  GPIO_ReadInputDataBit(HX712_GPIO_PORT,HX712_AIN_PINS)  //��ȡ����

#define ReadVoltage_Mode 26
#define ReadCount_Mode 25

void HX712_GPIO_INIT (void);
unsigned char tx_ReadCount(void);
unsigned char tx_ReadVoltage(void);
void HX712_Init_Mode(INT8U Mode);
#endif




