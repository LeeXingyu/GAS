#ifndef __CRC_H__
#define __CRC_H__

#include "MyTypeDef.h"

#ifdef    UTILITY_MODULE
#define   UTILITY_EXT
#else
#define   UTILITY_EXT  extern
#endif

#define     CRC_INITIAL                 0xFFFF                  //Initial FCS value
#define     CHECK_SUM                   0xF0B8                  //Good final FCS value

unsigned int crc16_ccitt_byte(unsigned int check, unsigned char dat);
unsigned int crc16_ccitt_block(unsigned char *buf, unsigned int check, unsigned int len);

BOOLEAN cmp_buf(const char * const soc, const char *const dest, INT32U len);

#endif /* __CRC_H__ */
