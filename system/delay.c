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
*   �� �� ��: delay_us
*   ����˵��: ΢����ʱ����
*   ��    �Σ�nCountҪ��ʱ��΢����
*   �� �� ֵ: ��
**********************************************************************************************************/
void delay_us(u16 nCount)  //us��׼   ����ms ʵ��us��3��
{
    //��ʱ��Ƶ���й�  4Mʱ��Ƶ��  ��Ӧnop
    while(nCount--)
    {
      asm("nop"); //һ��asm("nop")��������ʾ�������Դ���100ns
      asm("nop");
      asm("nop");
      asm("nop");
    }
}

/**********************************************************************************************************
*   �� �� ��: delay_ms
*   ����˵��: ������ʱ����
*   ��    �Σ�nCountҪ��ʱ�ĺ�����
*   �� �� ֵ: ��
**********************************************************************************************************/
void delay_ms(u16 nCount)
{
//4MHz�����1ms
    unsigned int i;
    while(nCount--)  
    for(i=530;i>0;i--)
    delay_us(1);
}
/*******************************************************************************
****��ڲ�������
****���ڲ�������
****������ע���Ǿ�ȷ��ʱ����
****��Ȩ��Ϣ������Ƕ��ʽϵͳ
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
