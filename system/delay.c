/**
  ******************************************************************************
  * @file    delay.c
  * @author  Microcontroller Division
  * @version V1.2.0
  * @date    09/2010
  * @brief   delay functions
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */

/* Includes ------------------------------------------------------------------*/


#include "delay.h"

/**********************************************************************************************************
*   函 数 名: delay_us
*   功能说明: 微秒延时程序
*   形    参：nCount要延时的微秒数
*   返 回 值: 无
**********************************************************************************************************/
void delay_us(u16 nCount)  //us不准   请用ms 实际us大3倍
{
    //与时钟频率有关  4M时钟频率  对应nop
    while(nCount--)
    {
      asm("nop"); //一个asm("nop")函数经过示波器测试代表100ns
      asm("nop");
      asm("nop");
      asm("nop");
    }
}

/**********************************************************************************************************
*   函 数 名: delay_ms
*   功能说明: 毫秒延时程序
*   形    参：nCount要延时的毫米数
*   返 回 值: 无
**********************************************************************************************************/
void delay_ms(u16 nCount)
{
//4MHz，大概1ms
    unsigned int i;
    while(nCount--)  
    for(i=530;i>0;i--)
    delay_us(1);
}
/*******************************************************************************
****入口参数：无
****出口参数：无
****函数备注：非精确延时函数
****版权信息：蓝旗嵌入式系统
*******************************************************************************/

void Delay(__IO uint16_t nCount)
{
    // Decrement nCount value 
    while (nCount != 0)
    {
        nCount--;
    }
}


/*--------------------------------------------------------------------------------------------------------
                   							                  0ooo
                   								ooo0     (   )
                								(   )     ) /
                								 \ (     (_/
                								  \_)
----------------------------------------------------------------------------------------------------------*/
