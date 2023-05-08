
/*==============================================================================
------------------------Low level driver for SX1212-----------------------------
==============================================================================*/

#ifndef _SX1212_H_
#define _SX1212_H_

#include "SPI.h"

/*==============================================================================
-------------------------IMPORT FUNCTIONS FOR DRIVER----------------------------
==============================================================================*/
#define  SX1212_CFG_HIGH( )   GPIO_SetBits( GPIOD, GPIO_Pin_5 )
#define  SX1212_CFG_LOW( )		GPIO_ResetBits( GPIOD, GPIO_Pin_5 )

#define  SX1212_DATA_HIGH( )  GPIO_SetBits( GPIOD, GPIO_Pin_4 )
#define  SX1212_DATA_LOW( )  	GPIO_ResetBits( GPIOD, GPIO_Pin_4 )

INT8U SPI_ExchangeByte( INT8U input );
#define SX1212_READ_IRQ_0( )  ( GPIO_ReadInputDataBit( GPIOB, GPIO_Pin_4 ) )
#define SX1212_READ_IRQ_1( )  ( GPIO_ReadInputDataBit( GPIOB, GPIO_Pin_3 ) )

/*
================================================================================
--------------------------------MACRO DIFINITIONS-------------------------------
================================================================================
*/
typedef enum
{
    MODE_SLEEP              =   0x00,       //sleep mode
    MODE_STDBY              =   0x01,       //standby mode
    MODE_FS                 =   0x02,       //frequency synthesizer mode
    MODE_RX                 =   0x03,       //receive mode
    MODE_TX                 =   0x04	    //transmit mode
}CHIP_MODE;
/*
================================================================================
-------------------------EXPORT FUNCTIONS OF DRIVER-----------------------------
================================================================================
*/

/*Write a register of the device*/
void SX1212_WriteReg( INT8U addr, INT8U value );

/*Read a register of the device*/
INT8U SX1212_ReadReg( INT8U addr );

/*Initialize the SX1212 device*/
void SX1212_Init( void );

/*Put the SX1212 into another work mode*/
void SX1212_SetMode( CHIP_MODE chip_mode );

/*Write data into fifo*/
void SX1212_WriteFifo( INT8U *pbuff, INT8U size );

/*Send packet via the SX1212, Variable length*/
void SX1212_SendPacket_Var( INT8U *pbuff, INT8U size );

/*Read a packet from SX1212*/
INT8U SX1212_ReceivePacket( INT8U *buffer );

/*Enter the Receive mode*/
void SX1212_EnterReceiveMode( void );

#endif//_SX1212_H_

/*
================================================================================
-----------------------------------------End Of File----------------------------
================================================================================
*/
