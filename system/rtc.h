#ifndef __RTC_H__
#define __RTC_H__

#include "stm8l15x.h"
#include "stm8l15x_itc.h"
#include "usart.h"

#define time60s  (34000)
#define time5s  (2500)

void CLK_Config(char SYSCLKSource);
void RTC_Config(int time);

#endif 
