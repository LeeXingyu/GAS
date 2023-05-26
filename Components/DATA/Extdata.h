#ifndef __EXTDATA_H_
#define __EXTDATA_H_

#include "stm8l15x.h"
#include "stm8l15x_flash.h"
#include "data.h"
#include "string.h"

#define EEP_ADDRESS_START   0xBF70	                   
#define EEP_ADDRESS_END     0xFFFF	
#define EEPROMPAGESIZE      32

#define FlASH_OPER_SIZE     (COOKER_PARSE_ADDR_LEN + 20)
#define PARA_START_INDEX    0xBF70

void FLASH_ReadNByte(unsigned char* pBuffer, uint32_t ReadAddr, uint8_t nByte);
void FLASH_WriteNByte(uint8_t* pBuffer, uint32_t WriteAddr, uint8_t nByte);
void Cooker_SendSetIdResult(void);
void Cooker_SendGas_CTRL(void);
#endif //__EXTDATA_H_
