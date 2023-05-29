#ifndef  __USART_H__
#define  __USART_H__

/* ����ϵͳͷ�ļ� */
#include "stm8l15x.h"
#include "stm8l15x_gpio.h"

#include "delay.h"
#include <string.h>
#include <stdarg.h>

//#include <stdlib.h>
#include <stdio.h>

/*���崮��*/
#define USART_GPIO_PORT  GPIOC  
#define TXD_GPIO_PINS  GPIO_Pin_3  
#define RXD_GPIO_PINS  GPIO_Pin_2

/* �Զ��峣����ͱ��ʽ�� */
#define countof(a)   (sizeof(a) / sizeof(*(a)))
#define RxBufferSize 50
#define TxBufferSize 120
#define BAUDRATE 115200
#define SIZE 128
typedef struct tag_uart{

	unsigned char rx_data[RxBufferSize];
	unsigned char tx_data[TxBufferSize];
        unsigned char rx_idx_start;

}uart;

extern uart usart1;

 
/*����һ���ṹ�壬��Ŵ�����ر���*/
typedef struct uart_rxbuf_s
{
  uint8_t               buf[SIZE];  /* ���ڽ������ݵ�buffer*/
  uint32_t              len;      /* ���ݵĳ��� */
}uart_rxbuf_t;
 
extern uart_rxbuf_t  g_uart_rxbuf;  /*����һ���ṹ�����g_uart_rxbuf�������Է��ʽṹ����ĳ�Ա*/

/* �������ⲿʹ�õı��� */

/* �������ⲿʹ�õĺ��� */

/*******************************************************************************
 * ����: Uart_Init
 * ����: UART1��ʼ������
 * �β�: ��
 * ����: ��
 * ˵��: �� 
 ******************************************************************************/
void Usart_Init(void);

/*******************************************************************************
 * ����: Uart_DMA_Init
 * ����: UART1��ʼ������
 * �β�: ��
 * ����: ��
 * ˵��: �� 
 ******************************************************************************/

//�Զ��ϴ�


//��ʼ������
void Usart1_clear_init (void);
/*******************************************************************************
 * ����: UART1_SendByte
 * ����: UART1����һ���ֽ�
 * �β�: data -> Ҫ���͵��ֽ�
 * ����: ��
 * ˵��: �� 
 ******************************************************************************/
void USART_SendByte(u8 data);

/*******************************************************************************
 * ����: UART1_SendString
 * ����: UART1����len���ַ�
 * �β�: data -> ָ��Ҫ���͵��ַ���
 *       len -> Ҫ���͵��ֽ���
 * ����: ��
 * ˵��: �� 
 ******************************************************************************/
void USART_SendString(u8* Data,u16 len);
void USART_SendStr(unsigned char *Str);
/*******************************************************************************
 * ����: UART1_ReceiveByte
 * ����: UART1����һ���ַ�
 * �β�: ��
 * ����: ���յ����ַ�
 * ˵��: �� 
 ******************************************************************************/
//u8 USART_ReceiveByte(void);

extern int uart_getchar(void);/*����*/
/*******************************************************************************
 * ����: fputc
 * ����: �ض���c�⺯��printf��UART1
 * �β�: ��
 * ����: Ҫ��ӡ���ַ�
 * ˵��: ��printf���� 
 ******************************************************************************/
 //#ifdef _IAR_
//int fputc(int ch, FILE *f);
//#endif
/********************************************************
 * ��������itoa
 * ����  ������������ת�����ַ���
 * ����  ��-radix =10 ��ʾ10���ƣ��������Ϊ0
 *         -value Ҫת����������
 *         -buf ת������ַ���
 *         -radix = 10
 * ���  ����
 * ����  ����
 * ����  ����USART1_printf()����
 *******************************************************/
char *itoa(int value, char *string, int radix);

/************************************************************************
 * ��������USART1_printf
 * ����  ����ʽ�������������C���е�printf��������û���õ�C��
 * ����  ��-USARTx ����ͨ��������ֻ�õ��˴���1����USART1
 *		     -Data   Ҫ���͵����ڵ����ݵ�ָ��
 *			   -...    ��������
 * ���  ����
 * ����  ���� 
 * ����  ���ⲿ����
 *         ����Ӧ��	 USART1_printf( "\r\n this is a demo \r\n" );
 *            		 USART1_printf(  "\r\n %d \r\n", i );
 *            		 USART1_printf(  "\r\n %s \r\n", j );
 ***************************************************************************/
void USART_printf( uint8_t *Data,...);
void mprintf(char *Data, ...);


#endif 


