#ifndef  __TIMER_H__
#define  __TIMER_H__

#include "stm8l15x.h"
#include "stm8l15x_gpio.h"
#include "delay.h"
#include <string.h>
#include <stdarg.h>
#include "mytypedef.h"
#include <stdio.h>

void TIM3_Init(void);
void TIM3_Handle(void);

#endif