/*=============================================================================
------------------------Low level driver for SX1212---------------------------
==============================================================================*/
#include "SX1212.H"

/*============================================================================
*Function:  SX1212_Init( ) => Initialize the SX1212 device
*Input   :  None
*Output  :  None
============================================================================*/
//主配置寄存器表
INT8U ConfigTable[] =
{   
// Write  register add时  只有中间五位有效   
    0x30,   
    0xA4,    
    0x03,  
    0x63,    
    0x1F,    
    0xC6,    
    143, //433MHz   
    56,    
    55,   
    0x77,    
    0x2F, 
    0x19,
    0xCF, 0xCC,
    0x09, 
    0x00, 
    0xA, 0x38, 0x28, 0x0C, 
    0x00, 
    0x69, 0x81, 0x7E, 0x96, 
    0x70,
    0x3C,
    0x40, 0x00, 
    // 可变长度 1byte前导文 off whitening
    // 开启CRC 
    0x88, 
    0x00,
    
		/*
		0x30, 0xA4, 0x07, 0x07, 0x1F, 0xC6, 
		107, //434MHz
		42, 
		30, 
		0x77, 0x2F, 0x19,
		0xCF, 0x0C, 0x09, 0x00, 0xA3, 0x38, 0x28, 0x0C, 0x00, 
		0x2D, 0xD4, 0x7E,0x96, 
		0x72, 0x3C, 0x40,	0x00, 0xC8, 0x00
		*/
};

void SX1212_Init( void )
{
    INT8U i;
    for( i = 0; (i+1) <= 0x1F; i++ )
    {
        if( i < 0x14 )	{ SX1212_WriteReg( i, ConfigTable[i] ); }
        else    { SX1212_WriteReg( i+1, ConfigTable[i] ); }
    }
}
/*============================================================================
*Function:  SX1212_SetFreqBand( ) => Set the frequency band of SX1212
*Input   :  freq_band,
        	FB_300_330 = 0x00,          300 - 330MHz
        	FB_320_350 = 0x01,          320 - 350MHz
        	FB_350_390 = 0x02,          350 - 390MHz
        	FB_390_430 = 0x03,          390 - 430MHz
        	FB_430_470 = 0x04,          430 - 470MHz
        	FB_470_510 = 0x05           470 - 510MHz
============================================================================*/
void SX1212_SetFreqBand( INT8U  freq_band  )
{
    INT8U tmp = SX1212_ReadReg( 0x00 );		//read the REG_MCPARAM1
    tmp &= ~( ( 1<<4 ) | ( 1<<3 ) | ( 1<<2 ) );		//Clear bit 4-2
    tmp |= ( freq_band << 2 );						        //replace bit 4-2
    SX1212_WriteReg( 0x00, tmp ); 			  //rewrite the register
}

/*============================================================================
*Function:  SX1212_WriteReg( ) => Write a register of the device
*Input   :  addr, the address of the register
            value, the value to be written
*Output  :  None
============================================================================*/
void SX1212_WriteReg( INT8U addr, INT8U value )
{
    SX1212_CFG_LOW( );
    SPI_ExchangeByte( (addr<<1) & 0x3E );
    SPI_ExchangeByte( value );
    SX1212_CFG_HIGH( );
}

/*============================================================================
*Function:  SX1212_ReadReg( ) => Read a register of the device
*Input   :  addr, the address of the register
*Output  :  the read value
=============================================================================*/
INT8U SX1212_ReadReg( INT8U addr )
{
    INT8U btmp;
    
    SX1212_CFG_LOW( );
    SPI_ExchangeByte( ( (addr<<1) & 0x7E ) | 0x40 );
    btmp = SPI_ExchangeByte( 0 );
    SX1212_CFG_HIGH( );
    
    return btmp;
}

/*============================================================================
*Function:  SX1212_SetMode( ) => Put the SX1212 into another work mode
*Input   :  chip_mode,
            MODE_SLEEP = 0x00,      sleep mode
            MODE_STDBY = 0x01,      standby mode
            MODE_FS    = 0x02,      frequency synthesizer mode
            MODE_RX    = 0x03,      receive mode
            MODE_TX    = 0x04,      transmit mode
*Output  :  None
============================================================================*/
void SX1212_SetMode( CHIP_MODE chip_mode )
{
    INT8U tmp = SX1212_ReadReg( 0x00 );             //read the REG_MCPARAM1
    tmp &= ~( ( 1<<7 ) | ( 1<<6 ) | ( 1<<5 ) );     //Clear bit 7-5
    tmp |= ( chip_mode << 5 );                      //replace bit 7-5
    SX1212_WriteReg( 0x00, tmp );                   //rewrite the register
}

/*============================================================================
*Function:  SX1212_WriteFifo( ) => Write data into fifo 
*Input   :  pbuff, the buffer stores data
            size, how many bytes should be written
*Output  :  None
============================================================================*/
void SX1212_WriteFifo( INT8U *pbuff, INT8U size )
{
    while( size -- )
    {
        SX1212_DATA_LOW( );
        SPI_ExchangeByte( *pbuff ++ );
        SX1212_DATA_HIGH( );
    }
}

/*============================================================================
*Function:  SX1212_ReceivePacket( ) => Receive a packet from the device
*Input   :  buffer, the buffer stores the data
*Output  :  the size of the received packet
=============================================================================*/
INT8U SX1212_ReceivePacket( INT8U *buffer )
{
    INT8U size, i;
    INT8U tmp = SX1212_ReadReg( 31 );       //read the REG_MCPARAM1
    tmp &= ~( 1<<6 );                       //Clear bit 6
    tmp |= ( 1<<6 );                        //replace bit 6
    SX1212_WriteReg( 31, tmp );             //rewrite the register
          
    SX1212_SetMode( MODE_STDBY );

    SX1212_DATA_LOW( );
    size = SPI_ExchangeByte( 0xFF );
    SX1212_DATA_HIGH( );
    
    for( i = 0; i < size; i ++ )
    {
        SX1212_DATA_LOW( );
        *buffer ++ = SPI_ExchangeByte( 0xFF );
        SX1212_DATA_HIGH( );
    }
    return size;
}

/*============================================================================
*Function:  SX1212_SendPacket( ) => Send packet via the SX1212, Variable length
*Input   :  pbuff, the buffer stores data
            size, how many bytes should be written
*Output  :  None
=============================================================================*/
void SX1212_SendPacket_Var( INT8U *pbuff, INT8U size )
{
    INT8U tmp = SX1212_ReadReg( 1 );        //read the REG_MCPARAM2
    tmp &= ~( ( 1<<7 ) | ( 1<<6 ) );        //Clear bit 7-6
    tmp |= ( 2<<6 );                        //replace bit 7-6
    SX1212_WriteReg( 1, tmp );              //rewrite the register

    tmp = SX1212_ReadReg( 31 );             //read the REG_MCPARAM1
    tmp &= ~( 1<<6 );                       //Clear bit 6 
    SX1212_WriteReg( 31, tmp );             //rewrite the register
    SX1212_SetMode( MODE_STDBY );
    
    SX1212_WriteFifo( &size, 1 );
    SX1212_WriteFifo( pbuff, size );
    SX1212_SetMode( MODE_TX );
    while( !( SX1212_ReadReg( 14 ) & ( 1<<5 ) ) );
}

/*=============================================================================
*Function:  SX1212_EnterReceiveMode( ) => Enter the Receive mode
*Input   :  None
*Output  :  None
=============================================================================*/
void SX1212_EnterReceiveMode( void )
{
    INT8U tmp = SX1212_ReadReg( 0x01 );     //read the REG_MCPARAM2
    tmp &= ~( ( 1<<7 ) | ( 1<<6 ) );        //Clear bit 7.6
    tmp |= ( 2<<6 );                        //replace bit 7.6
    SX1212_WriteReg( 0x01, tmp );           //rewrite the register
    
    SX1212_SetMode( MODE_RX );
}





/*=============================================================================
-----------------------------------------End Of File--------------------------
==============================================================================*/
