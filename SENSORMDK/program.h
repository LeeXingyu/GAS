#ifndef __PROGRAM_H__
#define __PROGRAM_H__

#include "stm8l15x.h"
#include "stm8l15x_it.h"
#include "stm8l15x_conf.h"
#include "stm8l15x_flash.h"
#include <string.h>
//Sys
#include "delay.h"
#include "rtc.h"
//Comp
#include "usart.h"
#include "adc.h"
#include "ETC.h"
//#include "SX1212.h"
#include "HX712.h"
#include "activehalt.h"
#include "SPI.h"
#include "MyTypeDef.h"
#include "timer.h"
#include "Rfm64.h"
#include "data.h"
#include "Extdata.h"

void HardWare_Init(void); 
void LowPowerStart(void);
void LowPowerStop(void);
void Slave_Service(void);
void FirstPower_CheckService(void);
void Slave_GasBat_SendService(void);
void Shutdown_GasSend(void);
#endif