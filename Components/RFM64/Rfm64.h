#ifndef _RFM64_H_
#define _RFM64_H_

#include "SPI.h"



//设置IRQ_0 1的使能 通过设置对应IO口的工作方式
void enable_IRQ(int n);
//禁用IRQ_0 1的使能 通过设置对应IO口的工作方式
void disable_IRQ(int n);

//以下是SPI接口
#define set_SCK          GPIO_WriteBit(GPIOB, GPIO_Pin_5, SET)
#define set_NSS_DATA     GPIO_WriteBit(GPIOD, GPIO_Pin_4, SET)
#define set_NSS_CONFIG   GPIO_WriteBit(GPIOD, GPIO_Pin_5, SET)
#define set_MOSI         GPIO_WriteBit(GPIOB, GPIO_Pin_6, SET)
#define clr_SCK          GPIO_WriteBit(GPIOB, GPIO_Pin_5, RESET)
#define clr_NSS_DATA     GPIO_WriteBit(GPIOD, GPIO_Pin_4, RESET)
#define clr_NSS_CONFIG   GPIO_WriteBit(GPIOD, GPIO_Pin_5, RESET)
#define clr_MOSI         GPIO_WriteBit(GPIOB, GPIO_Pin_6, RESET)
#define SPIMisoTest()    GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7) //read_MISO


#define read_IRQ_0       GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4)
#define read_IRQ_1       GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)

//以下部分应该无需修改
/*******************************************************************
** RF packet definition                                           **
*******************************************************************/
#define RF_BUFFER_SIZE_MAX   64
#define RF_BUFFER_SIZE       64

/*******************************************************************
** RF State machine                                               **
*******************************************************************/
#define RF_STOP              0x01
#define RF_BUSY              0x02
#define RF_RX_DONE           0x04
#define RF_TX_DONE           0x08
#define RF_ERROR             0x10
#define RF_TIMEOUT           0x20

/*******************************************************************
** RF function return codes                                       **
*******************************************************************/
//#define OK                   0x00
#define RETURN_ERROR             0x01    //这里之前是ERROR改了是因为和"stm8l15x.h"冲突
#define RX_TIMEOUT           0x02
#define RX_RUNNING           0x03
#define TX_TIMEOUT           0x04
#define TX_RUNNING           0x05
/*******************************************************************
** SX1212 Operating modes definition                              **
*******************************************************************/


/*******************************************************************
** SX1212 definitions                                             **
*******************************************************************/

/*******************************************************************
** SX1212 Internal registers Address                              **
*******************************************************************/
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


/*******************************************************************
** SX1212 bit control definition                                  **
*******************************************************************/
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

#define RF_BIRATE_200_MSB                0xFA
#define RF_BIRATE_200_LSB                0xFE

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






            // ======== build conditionals  ======== //
// !!! user can reconfigure them
#define SYNC_WORD1          0x63
#define SYNC_WORD2          0x64
#define SYNC_WORD3          0x65
#define SYNC_WORD4          0x66

#define RF_TX_POWER         RF_TX_POWER_PLUS10

#define DATA_MODE   PACKET_MODE
    #define PACKET_MODE     1
    #define BUFFER_MODE     2

#define LEN_TYPE            FIXED_LEN
    #define FIXED_LEN       3       // fixed length
    #define VARIABLE_LEN    4       // variable length

//#define CRC                 1
#if DATA_MODE == BUFFER_MODE
    #ifdef CRC
        #error "CRC doesn't support in buffer mode"
    #endif
#endif

// ======== a packet format demo  for user reference ======== //
// !!! user can redefine RF packet format
#define TEST_ID     0xBB          // a test ID
#define FILLS       40            // fill some contents in RF packet for test

#if LEN_TYPE == FIXED_LEN
    typedef struct {
        unsigned char fill[FILLS];
        unsigned char test_id;
        unsigned char counter;
        unsigned char key;
    }_RF_PACKET;
    #if DATA_MODE == PACKET_MODE
        #if FILLS > 61
            #error "packet too big"
        #endif
    #endif
#endif

#if LEN_TYPE == VARIABLE_LEN
    typedef struct {
        unsigned char len;
        unsigned char fill[FILLS];
        unsigned char test_id;
        unsigned char counter;
        unsigned char key;
    }_RF_PACKET;
    #if DATA_MODE == PACKET_MODE
        #if FILLS > 60
            #error "packet too big"
        #endif
    #endif
#endif

            // ======== pin definition  ======== //
//#define RF_IRQ0()     RA2
//#define RF_IRQ1()     RA4

            // ======== extern functions  ======== //
extern void InitSX1212 (void);
extern unsigned char SetRFMode(unsigned char mode);
    #define RF_SLEEP                         (0<<5)
    #define RF_STANDBY                       (1<<5)
    #define RF_SYNTHESIZER                   (2<<5)
    #define RF_RECEIVER                      (3<<5)
    #define RF_TRANSMITTER                   (4<<5)

    #define MODE_CHANGED                     0x00
    #define MODE_NOCHANGED                   0xFF


extern unsigned char _SpiConfig (unsigned char addr, unsigned char val);     // note: don't call this routine directly, use below macros to call it
#define SpiReadCfg(addr)        _SpiConfig((((addr<<1)&0x3E)|0x40), 0)
#define SpiWriteCfg(addr, val)  _SpiConfig(((addr<<1)&0x3E), val)

#define clearFIFO()     SpiWriteCfg(REG_IRQPARAM2, 0x1F)
//extern void BuildPacket (unsigned char test_val);       // !!! this is a packet builder demo for reference, user should define his own packet builder
extern unsigned char BuildPacket(unsigned char *buffer, unsigned int len);
extern void SendRfFrame (unsigned char *buffer, unsigned char len);


extern void ReceiveRfFrame (unsigned char *buffer, unsigned char len, unsigned char *rc);
    #define RF_RX_RUNNING        0x01
    #define ERROR                0x02
    #define OK                   0x04

#define ReadRssi()  SpiReadCfg(REG_RSSIVALUE)

            // ======== global vars ======== //
extern _RF_PACKET RF_Pkt;




#endif
