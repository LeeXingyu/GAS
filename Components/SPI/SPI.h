
#ifndef _SPI_H_
#define _SPI_H_

#include "mytypedef.h"
#include "stm8l15x.h"
/*Exchange a byte via the SPI bus*/
INT8U SPI_ExchangeByte( INT8U input );

/*Initialize the SPI bus*/
void SPI_Initial( void );

/*Initialize the other GPIOs of the board*/
void GPIO_Initial( void );


#endif //_BOARD_H_

/*==============================================================================
------------------------------------End of FILE--------------------------------
==============================================================================*/
