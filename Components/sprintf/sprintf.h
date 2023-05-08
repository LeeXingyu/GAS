#ifndef  __SPRINTF_H__
#define  __SPRINTF_H__

#include "stm8l15x.h"
#include <string.h>

char *myitoa(int num,char *str,int radix);
void num2char(char *str, double number, uint8_t g, uint8_t l);
long myatoi( char *str);
#endif 


