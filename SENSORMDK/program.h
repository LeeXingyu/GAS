#ifndef __PROGRAM_H__
#define __PROGRAM_H__

#include "stm8l15x.h"
#include "stm8l15x_it.h"
#include "stm8l15x_conf.h"
//Sys
#include "delay.h"
#include "rtc.h"
//Comp
#include "usart.h"
#include "ETC.h"
#include "SX1212.h"
#include "HX712.h"
#include "activehalt.h"
#include "SPI.h"
#include "MyTypeDef.h"
#include "data.h"
#include "timer.h"
//#include "Rfm64.h"
#include "data.h"

void HardWare_Init(void); 
void LowPowerConsumption_Cmd(void);
void StandyFun(void);

#endif