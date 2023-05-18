#ifndef  __DATA_H__
#define  __DATA_H__

#include "stm8l15x.h"
#include "stm8l15x_gpio.h"
#include "stm8l15x_flash.h"
#include "delay.h"
#include <string.h>
#include <stdarg.h>
#include "SX1212.H"
#include "Rfm64.h"
#include "ETC.h"

#include <stdio.h>
#include "USART.h"
#include "Crc.h"
#include "Extdata.h"

/* �Զ��峣����ͱ���ʽ�� */
#define RxBuffer 10
#define TxBuffer 10

#ifdef    COOKER_PARSE_MODULE
#define   COOKER_PARSE_EXT
#else
#define   COOKER_PARSE_EXT  extern
#endif

#define COOKER_PARSE_HEADER			'C'
#define COOKER_PARSE_ADDR_LEN			12
#define COOKER_PARSE_PAYLOAD_LEN		30

#define COOKER_PARSE_TRUE				(0xAA)
#define COOKER_PARSE_FALSE				(0x55)

typedef struct tag_data{

	unsigned char rx_data[RxBuffer];
	unsigned char tx_data[TxBuffer];
        unsigned char rx_idx_start;
        unsigned char rx_len;

}data_buf;
INT8U recv_sx1212_data(void);
void Read_UID(void);
extern data_buf data;

typedef struct _Cooker_Parse_t
{
	unsigned char addr[COOKER_PARSE_ADDR_LEN];
	unsigned char length;
	unsigned char cmd;
	unsigned char payload[COOKER_PARSE_PAYLOAD_LEN];
}Cooker_Parse_t;

typedef enum _Cooker_Cmd_e
{
	eCOOKER_SET_SYS_ID = 0,
	eCOOKER_FIRE_STATE = 1,
	eCOOKER_CTRL_Gas  = 2,
        eCOOKER_STATE_Gas = 3,
        eCOOKER_STATE_Bat = 4,
	//eCOOKER_RSP = 0xEE,
}Cooker_Cmd_e;

void Rcv_MasterDataParse(void);
void Master_data_Prase(int c);
void GetMasterId(unsigned char *id);

unsigned int Slave_Load(Cooker_Parse_t *entity);
void Slave_WirelessSendLoad(char *load, unsigned int len);
//发送数据
void Gas_State_Load(unsigned char gas_state);
void Bat_State_Load(unsigned char gas_state);
//实现主机要求的功能函数
void Cooker_AFN_Handle(Cooker_Parse_t *entity);

/*
void Cooker_Parse(int c);

*/


#endif 


