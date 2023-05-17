
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



/*******************************************************************
** SX1212 Internal registers Address                              **
*******************************************************************/
#if 1
#define REG_MCPARAM1                     0x00      //0
#define REG_MCPARAM2                     0x01      //1
#define REG_FDEV                         0x02      //2
#define REG_BITRATE_MSB                  0x03      //3
#define REG_BITRATE_LSB                  0x04      //4
#define REG_MCPARAM6                     0x05      //5
#define REG_R1                           0x06      //6
#define REG_P1                           0x07      //7
#define REG_S1                           0x08      //8
#define REG_R2                           0x09      //9
#define REG_P2                           0x0A      //10
#define REG_S2                           0x0B      //11
#define REG_FIFOCONFIG                   0x0C      //12
#define REG_IRQPARAM1                    0x0D      //13
#define REG_IRQPARAM2                    0x0E      //14
#define REG_RSSIIRQTHRESH                0x0F      //15

#define REG_RXPARAM1                     0x10      //16
#define REG_RXPARAM2                     0x11      //17
#define REG_RXPARAM3                     0x12      //18
#define REG_OOKFLOORTHRESH               0x13      //19
#define REG_RSSIVALUE                    0x14      //20
#define REG_RXPARAM6                     0x15      //21
#define REG_SYNCBYTE1                    0x16      //22
#define REG_SYNCBYTE2                    0x17      //23
#define REG_SYNCBYTE3                    0x18      //24
#define REG_SYNCBYTE4                    0x19      //25
#define REG_TXPARAM                      0x1A      //26
#define REG_OSCPARAM                     0x1B      //27

#define REG_PKTPARAM1                    0x1C      //28
#define REG_NODEADRS                     0x1D      //29
#define REG_PKTPARAM3                    0x1E      //30
#define REG_PKTPARAM4                    0x1F      //31


/*******************************************************************
** SX1212 definitions                                             **
*******************************************************************/

/*******************************************************************
** center frequency                                               **
*******************************************************************/
// RPS = 143,56,55 for 433MHz
#define RX_CENTER_FREQ_433MHZ_R     143
#define RX_CENTER_FREQ_433MHZ_P     56
#define RX_CENTER_FREQ_433MHZ_S     55

// RRS = 107,42,30 for 434MHz
#define RX_CENTER_FREQ_434MHZ_R     107
#define RX_CENTER_FREQ_434MHZ_P     42
#define RX_CENTER_FREQ_434MHZ_S     30
#endif

/*******************************************************************
** SX1212 bit control definition                                  **
*******************************************************************/
#if 1
//// REG_MCPARAM1
// Chip operationmode
#define RF_MC1_SLEEP                    0x00
#define RF_MC1_STANDBY                  0x20    // d
#define RF_MC1_SYNTHESIZER              0x40
#define RF_MC1_RECEIVER                 0x60
#define RF_MC1_TRANSMITTER              0x80

// Frequency band
#define RF_MC1_BAND_300_330             0x00
#define RF_MC1_BAND_330_365             0x04
#define RF_MC1_BAND_365_400             0x08
#define RF_MC1_BAND_400_440             0x0C    // d
#define RF_MC1_BAND_440_470             0x10
#define RF_MC1_BAND_470_510             0x14

// sub band
#define RF_MC1_SUB_BAND_1st             0x00
#define RF_MC1_SUB_BAND_2nd             0x01
#define RF_MC1_SUB_BAND_3rd             0x02
#define RF_MC1_SUB_BAND_4th             0x03    // d


//// REG_MCPARAM2
// data operation mode
#define RF_MC2_CONTINUOUS_MODE          0x00    // d
#define RF_MC2_BUFFER_MODE              0x40
#define RF_MC2_PACKET_MODE              0x80

// modulation scheme
#define RF_MC2_OOK                      0x10
#define RF_MC2_FSK                      0x20    // d

// OOK thresh type
#define RF_MC2_FIXED                    0x00
#define RF_MC2_PEAK                     0x04    // d
#define RF_MC2_AVERAGE                  0x08

// IF gain
#define RF_0DB                          0x00    // d
#define RF_M4DB5                        0x01
#define RF_M9DB                         0x02
#define RF_M13DB5                       0x03

//// Frequency deviation (KHz) REG_FDEV (XTAL = 12.8MHz)
#define RF_FDEV_20                      19
#define RF_FDEV_40                      9
#define RF_FDEV_50                      7
#define RF_FDEV_80                      4
#define RF_FDEV_100                     3       // d
#define RF_FDEV_200                     1

//// Bitrate (bit/sec) REG_BITRATE_C & D (XTAL = 12.8MHz)
#define RF_BIRATE_2500_MSB                 0x01
#define RF_BIRATE_2500_LSB                 0x7F

#define RF_BIRATE_4800_MSB                 35
#define RF_BIRATE_4800_LSB                 36

#define RF_BIRATE_19200_MSB                0x02
#define RF_BIRATE_19200_LSB                0x6E


//// REG_MCPARAM6
// PA_RAMP OOK
#define RF_MC6_PARAMP_OOK_3US           0x00
#define RF_MC6_PARAMP_OOK_8US5          0x40
#define RF_MC6_PARAMP_OOK_15US          0x80
#define RF_MC6_PARAMP_OOK_23US          0xC0    // d

// Low power Rx
#define RF_MC6_LOW_PWR_RX               0x20

// Trim band
#define RF_MC6_TRIM_BAND                0x06    // d

// PRS select
#define RF_MC6_PRS1                     0x00    // d
#define RF_MC6_PRS2                     0x01

//// REG_FIFO_CTRL
// FIFO size
#define RF_FIFO_SIZE16                 0x00    // d
#define RF_FIFO_SIZE32                 0x40
#define RF_FIFO_SIZE48                 0x80
#define RF_FIFO_SIZE64                 0xC0

// FIFO thressh (bit5-0)

//// REG_IRQPARAM1
// Rx irq0 source continuous mode

// Rx irq0 source buffer mode


// Rx irq0 source packet mode
#define RF_RX_IRQ0_PLD_RDY              0x00    // d
#define RF_RX_IRQ0_WR_BYTE              0x40
#define RF_RX_IRQ0_N_FIFOEMPTY          0x80
#define RF_RX_IRQ0_SYNC                 0xC0

// Rx irq1 source continuous mode

// Rx irq1 source buffer mode

// Rx irq1 source packet mode
#define RF_RX_IRQ1_CRC_OK               0x00    // d
#define RF_RX_IRQ1_FIFOFULL             0x10
#define RF_RX_IRQ1_RSSI                 0x20
#define RF_RX_IRQ1_FIFO_THRESTH         0x30

// Select TX start condition and IRQ_0 source
#define RF_IRQ0_TX_FIFOTHRESH_START_FIFOTHRESH     0x00
#define RF_IRQ0_TX_FIFOEMPTY_START_FIFONOTEMPTY    0x08

// Select TX IRQ_1 sources
#define RF_IRQ1_TX_FIFOFULL              0x00
#define RF_IRQ1_TX_TXDONE                0x04


// FIFO filling method
#define RF_RX_FIFO_FILL_AUTO            0x00    // d
#define RF_RX_FIFO_FILL_MANUAL          0x80

// FIFO filling control (RF_RX_FIFO_FILL_MANUAL)
#define RF_RX_FIFO_FILL_START           0x40
#define RF_RX_FIFO_FILL_STOP            0x00

//// REG_RXPARAM1
// Passive filter (kHz)
#define RF_RX1_PASSIVEFILT_65            0x00
#define RF_RX1_PASSIVEFILT_82            0x10
#define RF_RX1_PASSIVEFILT_109           0x20
#define RF_RX1_PASSIVEFILT_137           0x30
#define RF_RX1_PASSIVEFILT_157           0x40
#define RF_RX1_PASSIVEFILT_184           0x50
#define RF_RX1_PASSIVEFILT_211           0x60
#define RF_RX1_PASSIVEFILT_234           0x70
#define RF_RX1_PASSIVEFILT_262           0x80
#define RF_RX1_PASSIVEFILT_321           0x90
#define RF_RX1_PASSIVEFILT_378           0xA0   // d
#define RF_RX1_PASSIVEFILT_414           0xB0
#define RF_RX1_PASSIVEFILT_458           0xC0
#define RF_RX1_PASSIVEFILT_514           0xD0
#define RF_RX1_PASSIVEFILT_676           0xE0
#define RF_RX1_PASSIVEFILT_987           0xF0

//// REG_RXPARAM2

//// REG_RXPARAM3
// Polyphase filter enable
#define RF_RX3_POLYPFILT_ON              0x80
#define RF_RX3_POLYPFILT_OFF             0x00

// bit sync
#define RF_RX3_BITSYNC_ON                0x00   // d
#define RF_RX3_BITSYNC_OFF               0x40

// Sync word recognition
#define RF_RX3_SYNC_RECOG_ON             0x20
#define RF_RX3_SYNC_RECOG_OFF            0x00   // d

// Size of the reference sync word
#define RF_RX3_SYNC_SIZE_8               0x00
#define RF_RX3_SYNC_SIZE_16              0x08
#define RF_RX3_SYNC_SIZE_24              0x10
#define RF_RX3_SYNC_SIZE_32              0x18   // d

// Number of tolerated errors for the sync word recognition
#define RF_RX3_SYNC_TOL_0                0x00   // d
#define RF_RX3_SYNC_TOL_1                0x02
#define RF_RX3_SYNC_TOL_2                0x04
#define RF_RX3_SYNC_TOL_3                0x06

// TX Param
// Interpolator filter Tx (kHz)
#define RF_TX_FC_VALUE                   0x70
// Important notes: Values defined below only apply if RFCLKREF = DEFAULT VALUE = 0x07
#define RF_TX_FC_25                      0x00
#define RF_TX_FC_50                      0x10
#define RF_TX_FC_75                      0x20
#define RF_TX_FC_100                     0x30
#define RF_TX_FC_125                     0x40
#define RF_TX_FC_150                     0x50
#define RF_TX_FC_175                     0x60
#define RF_TX_FC_200                     0x70
#define RF_TX_FC_225                     0x80
#define RF_TX_FC_250                     0x90
#define RF_TX_FC_275                     0xA0
#define RF_TX_FC_300                     0xB0
#define RF_TX_FC_325                     0xC0
#define RF_TX_FC_350                     0xD0
#define RF_TX_FC_375                     0xE0
#define RF_TX_FC_400                     0xF0

// Tx Output power (dBm)
#define RF_TX_POWER_MAX                  0x00
#define RF_TX_POWER_PLUS10               0x02
#define RF_TX_POWER_PLUS7                0x04
#define RF_TX_POWER_PLUS4                0x06
#define RF_TX_POWER_PLUS1                0x08
#define RF_TX_POWER_MINUS2               0x0A

// Zero IF Tx operation
#define RF_TX_ZERO_IF_OFF                0x00
#define RF_TX_ZERO_IF_ON                 0x01


// OSC Param
// CLKOUT enable
#define RF_OSC_CLKOUT_ON                 0x80
#define RF_OSC_CLKOUT_OFF                0x00

// CLKOUT frequency (kHz)
#define RF_OSC_CLKOUT_12800              0x00
#define RF_OSC_CLKOUT_6400               0x04
#define RF_OSC_CLKOUT_3200               0x08
#define RF_OSC_CLKOUT_2133               0x0C
#define RF_OSC_CLKOUT_1600               0x10
#define RF_OSC_CLKOUT_1280               0x14
#define RF_OSC_CLKOUT_1067               0x18
#define RF_OSC_CLKOUT_914                0x1C
#define RF_OSC_CLKOUT_800                0x20
#define RF_OSC_CLKOUT_711                0x24
#define RF_OSC_CLKOUT_640                0x28
#define RF_OSC_CLKOUT_582                0x2C
#define RF_OSC_CLKOUT_533                0x30
#define RF_OSC_CLKOUT_492                0x34
#define RF_OSC_CLKOUT_457                0x38
#define RF_OSC_CLKOUT_427                0x3C


// PKT Param 1
// Manchester enable
#define RF_PKT1_MANCHESTER_ON            0x80
#define RF_PKT1_MANCHESTER_OFF           0x00

// Payload length
#define RF_PKT1_LENGTH_VALUE             0x00


// Node Address
#define RF_NODEADRS_VALUE                0x00


// PKT Param 3
//Packet format
#define RF_PKT3_FORMAT_FIXED             0x00
#define RF_PKT3_FORMAT_VARIABLE          0x80

// Preamble size
#define RF_PKT3_PREAMBLE_SIZE_8          0x00
#define RF_PKT3_PREAMBLE_SIZE_16         0x20
#define RF_PKT3_PREAMBLE_SIZE_24         0x40
#define RF_PKT3_PREAMBLE_SIZE_32         0x60

// Whitening enable
#define RF_PKT3_WHITENING_ON             0x10
#define RF_PKT3_WHITENING_OFF            0x00

// CRC enable
#define RF_PKT3_CRC_ON                   0x08
#define RF_PKT3_CRC_OFF                  0x00

// Address filtering
#define RF_PKT3_ADRSFILT_00              0x00
#define RF_PKT3_ADRSFILT_01              0x02
#define RF_PKT3_ADRSFILT_10              0x04
#define RF_PKT3_ADRSFILT_11              0x06

//CRC status (Read only)
#define RF_PKT3_CRC_STATUS               0x01


// PKT Param 4
// FIFO autoclear if CRC failed for current packet
#define RF_PKT4_AUTOCLEAR_ON             0x00
#define RF_PKT4_AUTOCLEAR_OFF            0x80

// Select FIFO access in standby mode (read or write)
#define RF_PKT4_FIFO_STANDBY_WRITE       0x00
#define RF_PKT4_FIFO_STANDBY_READ        0x40

#endif

#endif//_SX1212_H_

/*
================================================================================
-----------------------------------------End Of File----------------------------
================================================================================
*/
