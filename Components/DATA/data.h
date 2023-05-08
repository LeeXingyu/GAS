#ifndef  __DATA_H__
#define  __DATA_H__

/* ����ϵͳͷ�ļ� */
#include "stm8l15x.h"
#include "stm8l15x_gpio.h"
#include "delay.h"
#include <string.h>
#include <stdarg.h>
#include "SX1212.H"
#include "ETC.h"
//#include <stdlib.h>
#include <stdio.h>
#include "USART.h"

/* �Զ��峣����ͱ���ʽ�� */
#define RxBuffer 10
#define TxBuffer 10
typedef struct tag_data{

	unsigned char rx_data[RxBuffer];
	unsigned char tx_data[TxBuffer];
        unsigned char rx_idx_start;
        unsigned char rx_len;

}data_buf;
INT8U recv_sx1212_data(void);
extern data_buf data;



#endif 


