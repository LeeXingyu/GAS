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
void tx_ReadCount(void);
void tx_ReadVoltage(void);
void Read_Init_Mode(INT8U Mode);
#endif




