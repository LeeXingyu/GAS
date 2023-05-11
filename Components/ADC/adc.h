#ifndef __ADC_H__
#define __ADC_H__

#include "stm8l15x.h"
#include "stm8l15x_it.h"
#include "delay.h"
#include "data.h"

#define Gas_ADC_Threshold 1400 

void Air_detection_Init(void);
INT8U Air_detection(void);

#endif