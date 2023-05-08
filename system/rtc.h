#ifndef __RTC_H__
#define __RTC_H__

#include "stm8l15x.h"
#include "stm8l15x_itc.h"
#include "usart.h"


void CLK_Config(char SYSCLKSource);
void RTC_Config(void);

#endif 
