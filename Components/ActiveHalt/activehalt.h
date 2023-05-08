#ifndef __ACTIVEHALT_H__
#define __ACTIVEHALT_H__


#include "stm8l15x.h"
#include "stm8l15x_it.h"
#include "delay.h"
#include "data.h"


extern void Active_Halt_Colse(void);
extern void Active_Halt_Open(void);

extern void GPIO_Init_Colse(void);


void LED_Run(void);
void VoltageValueinit(void);
void ReadVoltageValue(void);//读取电源电压

#endif 

