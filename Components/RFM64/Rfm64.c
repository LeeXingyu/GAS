/*
*这个程序由官方API移植到STM8l15x.使用了STM8l库.目前程序尚未测试.
* astwyg@126.com
* 2013年6月8日 10:52:25
*/


#include "Rfm64.h"
#include "stm8l15x.h"
#include "stm8l15x_spi.h"
#include "stm8l15x_tim1.h"
#include "mytypedef.h"
#include "delay.h"

            // ======== struct for SX1212 register init  ======== //
typedef struct {
    unsigned char addr;
    unsigned char val;
}_SX1212_REG;

            // ======== global Var  ======== //
_RF_PACKET    RF_Pkt;               // RF packet


            // ======== static fucntion  ======== //
static unsigned char SpiInOut (unsigned char val);


            // ======== macro  ======== //         // !!! user must redefine them based on MCU
//#define SPI_MISO()          RA5             // input  pin
//#define SPI_mosi(i)         RC5 = i         // output pin
//#define SPI_sck(i)          RC4 = i         // output pin
//#define SPI_nss_cfg(i)      RC7 = i         // output pin
//#define SPI_nss_dat(i)      RC6 = i         // output pin
//#define _nop_spi()          // do nothing, use nop if needed
//#define SPIInit()          do {                     \
//                            set_NSS_CONFIG;         \
//                            set_NSS_DATA;         \
//                            set_MOSI;            \
//                            clr_SCK;             \
//                        }while(0)
//
//#define DelayTS_OS()        Delay(55536)            // 5ms
//#define DelayTS_FS()        Delay(63936)            // 800us
//#define DelayTS_RE()        Delay(64536)            // 500us
//#define DelayTS_TR()        Delay(64536)            // 500us


#define RF_IRQ0()           read_IRQ_0
#define RF_IRQ1()           read_IRQ_1

#define SPI_MISO()          SPIMisoTest()			// input  pin
#define SPIInit()          do {                     \
                            set_NSS_CONFIG;         \
                            set_NSS_DATA;         \
                            set_MOSI;            \
                            clr_SCK;             \
                        }while(0)
#define _nop_spi()          // do nothing, use nop if needed


#define DelayTS_OS()        delay_ms(3)			// 5ms
#define DelayTS_FS()        delay_us(800)			// 800us
#define DelayTS_RE()        delay_us(500)			// 500us
#define DelayTS_TR()        delay_us(500)			// 500us

            // ======== static var  ======== //
#define DEF_MCPARAM1    (RF_MC1_SLEEP|RF_MC1_BAND_400_440|RF_MC1_SUB_BAND_1st)
static const _SX1212_REG RegistersCfg[] = { // !!! user can reconfigure register based on application
//  {addr,           val},
    {REG_MCPARAM1, DEF_MCPARAM1},

    #if DATA_MODE == BUFFER_MODE
    {REG_MCPARAM2, RF_MC2_BUFFER_MODE|RF_MC2_FSK},
    #endif

    #if DATA_MODE == PACKET_MODE
    {REG_MCPARAM2, RF_MC2_PACKET_MODE|RF_MC2_FSK},
    #endif

    {REG_FDEV, RF_FDEV_50},

    {REG_BITRATE_MSB, RF_BIRATE_4800_MSB},
    {REG_BITRATE_LSB, RF_BIRATE_4800_LSB},

    {REG_R1, RX_CENTER_FREQ_433MHZ_R},
    {REG_P1, RX_CENTER_FREQ_433MHZ_P},
    {REG_S1, RX_CENTER_FREQ_433MHZ_S},

    {REG_FIFOCONFIG, RF_FIFO_SIZE64|5},     // FIFO size = 64 bytes, threshold = 5 bytes

    //{REG_IRQPARAM1, RF_RX_IRQ0_PLD_RDY},
    //{REG_IRQPARAM2, RF_RX_FIFO_FILL_AUTO},

    {REG_RXPARAM1, RF_RX1_PASSIVEFILT_378|3},
    {REG_RXPARAM2, 0x38},

    // bit sync on, sync word on, 4 bytes, 0 bit tolerance
    {REG_RXPARAM3, RF_RX3_BITSYNC_ON    |  \
                   RF_RX3_SYNC_RECOG_ON |  \
                   RF_RX3_SYNC_SIZE_32  |  \
                   RF_RX3_SYNC_TOL_0},

    {REG_SYNCBYTE1, SYNC_WORD1},
    {REG_SYNCBYTE2, SYNC_WORD2},
    {REG_SYNCBYTE3, SYNC_WORD3},
    {REG_SYNCBYTE4, SYNC_WORD4},

    {REG_TXPARAM, RF_TX_FC_200|RF_TX_POWER},

    {REG_OSCPARAM, RF_OSC_CLKOUT_OFF},

    //{REG_NODEADRS, 0x00},

    #if DATA_MODE == PACKET_MODE
        #if LEN_TYPE == FIXED_LEN
            {REG_PKTPARAM1, RF_PKT1_MANCHESTER_OFF|sizeof(_RF_PACKET)},

            #ifdef CRC
                {REG_PKTPARAM3, RF_PKT3_FORMAT_FIXED        | \
                                RF_PKT3_PREAMBLE_SIZE_32    | \
                                RF_PKT3_WHITENING_OFF       | \
                                RF_PKT3_CRC_ON              | \
                                RF_PKT3_ADRSFILT_00},
            #else
                {REG_PKTPARAM3, RF_PKT3_FORMAT_FIXED        | \
                                RF_PKT3_PREAMBLE_SIZE_32    | \
                                RF_PKT3_WHITENING_OFF       | \
                                RF_PKT3_CRC_OFF             | \
                                RF_PKT3_ADRSFILT_00},
            #endif
        #endif

        #if LEN_TYPE == VARIABLE_LEN
            {REG_PKTPARAM1, RF_PKT1_MANCHESTER_OFF|64},

            #ifdef CRC
                {REG_PKTPARAM3, RF_PKT3_FORMAT_VARIABLE     | \
                                RF_PKT3_PREAMBLE_SIZE_32    | \
                                RF_PKT3_WHITENING_OFF       | \
                                RF_PKT3_CRC_ON              | \
                                RF_PKT3_ADRSFILT_00},
            #else
                {REG_PKTPARAM3, RF_PKT3_FORMAT_VARIABLE     | \
                                RF_PKT3_PREAMBLE_SIZE_32    | \
                                RF_PKT3_WHITENING_OFF       | \
                                RF_PKT3_CRC_OFF             | \
                                RF_PKT3_ADRSFILT_00},
            #endif
        #endif

    #endif

    //{REG_PKTPARAM4, 0x00},
};


/*====================================
InitSX1212

======================================*/
void InitSX1212 (void) {
    _SX1212_REG const *p;
    unsigned char i;

   SPIInit();


    SetRFMode(RF_SLEEP);
    p = RegistersCfg;
    for(i=sizeof(RegistersCfg)/sizeof(_SX1212_REG); i>0; i--) {
        SpiWriteCfg(p->addr, p->val);
        p++;
    }
}


/*=====================================================
SetRFMode

input
mode: mode to set

output
MODE_CHANGED    0x00
MODE_NOCHANGED  0xFF
=====================================================*/
unsigned char SetRFMode(unsigned char mode){
    static unsigned char pre_mode = RF_STANDBY;

    if(mode != pre_mode){
        if(mode == RF_TRANSMITTER){
    		if(pre_mode == RF_SLEEP){
               SpiWriteCfg(REG_MCPARAM1, (DEF_MCPARAM1 & 0x1F) | RF_STANDBY);
    		   DelayTS_OS();
               SpiWriteCfg(REG_MCPARAM1, (DEF_MCPARAM1 & 0x1F) | RF_SYNTHESIZER);
    		   DelayTS_FS();
    		   SpiWriteCfg(REG_MCPARAM1, (DEF_MCPARAM1 & 0x1F) | RF_TRANSMITTER);
    		   DelayTS_TR();
    		}
    		else if(pre_mode == RF_STANDBY){
    		   SpiWriteCfg(REG_MCPARAM1, (DEF_MCPARAM1 & 0x1F) | RF_SYNTHESIZER);
    		   DelayTS_FS();
    		   SpiWriteCfg(REG_MCPARAM1, (DEF_MCPARAM1 & 0x1F) | RF_TRANSMITTER);
    		   DelayTS_TR();
    		}
    		else if(pre_mode == RF_SYNTHESIZER){
    		   SpiWriteCfg(REG_MCPARAM1, (DEF_MCPARAM1 & 0x1F) | RF_TRANSMITTER);
    		   DelayTS_TR();
    		}
    		else if(pre_mode == RF_RECEIVER){
    		   SpiWriteCfg(REG_MCPARAM1, (DEF_MCPARAM1 & 0x1F) | RF_TRANSMITTER);
    		   DelayTS_TR();
    		}
    		pre_mode = RF_TRANSMITTER;
        }
        else if(mode == RF_RECEIVER){
            if(pre_mode == RF_SLEEP){
                SpiWriteCfg(REG_MCPARAM1, (DEF_MCPARAM1 & 0x1F) | RF_STANDBY);
                DelayTS_OS();
                SpiWriteCfg(REG_MCPARAM1, (DEF_MCPARAM1 & 0x1F) | RF_SYNTHESIZER);
                DelayTS_FS();
                SpiWriteCfg(REG_MCPARAM1, (DEF_MCPARAM1 & 0x1F) | RF_RECEIVER);
                DelayTS_RE();
    		}
    		else if(pre_mode == RF_STANDBY){
    		    SpiWriteCfg(REG_MCPARAM1, (DEF_MCPARAM1 & 0x1F) | RF_SYNTHESIZER);
                DelayTS_FS();
                SpiWriteCfg(REG_MCPARAM1, (DEF_MCPARAM1 & 0x1F) | RF_RECEIVER);
                DelayTS_RE();
    		}
            else if(pre_mode == RF_SYNTHESIZER){
                SpiWriteCfg(REG_MCPARAM1, (DEF_MCPARAM1 & 0x1F) | RF_RECEIVER);
        		DelayTS_RE();
        	}
            else if(pre_mode == RF_TRANSMITTER){
                SpiWriteCfg(REG_MCPARAM1, (DEF_MCPARAM1 & 0x1F) | RF_RECEIVER);
        		DelayTS_RE();
        	}
            pre_mode = RF_RECEIVER;
        }
        else if(mode == RF_SYNTHESIZER){
            if(pre_mode == RF_SLEEP){
	            SpiWriteCfg(REG_MCPARAM1, (DEF_MCPARAM1 & 0x1F) | RF_STANDBY);
        		DelayTS_OS();
	            SpiWriteCfg(REG_MCPARAM1, (DEF_MCPARAM1 & 0x1F) | RF_SYNTHESIZER);
                DelayTS_FS();
            }
            else if (pre_mode == RF_STANDBY){
        	    SpiWriteCfg(REG_MCPARAM1, (DEF_MCPARAM1 & 0x1F) | RF_SYNTHESIZER);
        		DelayTS_FS();
        	}
            else {
        	    SpiWriteCfg(REG_MCPARAM1, (DEF_MCPARAM1 & 0x1F) | RF_SYNTHESIZER);
        	}
            pre_mode = RF_SYNTHESIZER;
        }
        else if(mode == RF_STANDBY){
            if (pre_mode == RF_SLEEP){
	            SpiWriteCfg(REG_MCPARAM1, (DEF_MCPARAM1 & 0x1F) | RF_STANDBY);
        		DelayTS_OS();
        	}
        	else {
	            SpiWriteCfg(REG_MCPARAM1, (DEF_MCPARAM1 & 0x1F) | RF_STANDBY);
	        }
            pre_mode = RF_STANDBY;
        }
        else {// mode == RF_SLEEP
            SpiWriteCfg(REG_MCPARAM1, (DEF_MCPARAM1 & 0x1F) | RF_SLEEP);
            pre_mode = RF_SLEEP;
        }
        return MODE_CHANGED;
    }
    else {
        return MODE_NOCHANGED;
    }
}


/*=====================================================
BuildPacket

this is a packet builder demo for reference
!!! user should define his own packet builder
=====================================================*/
//void BuildPacket (unsigned char test_val) {
//        unsigned char i, *p;
//        unsigned char j = 0x00;
//        static unsigned char counter = 0;
//
//    #if LEN_TYPE == FIXED_LEN
//        p = (unsigned char *)(&RF_Pkt.fill);
//        for(i = sizeof(RF_Pkt.fill); i > 0; i--) {
//            *p++ = j++;      // 0x00, 0x01... as demo
//        }
//
//        RF_Pkt.test_id = TEST_ID;
//        RF_Pkt.counter = ++counter;
//        RF_Pkt.key = test_val;
//    #endif
//
//    #if LEN_TYPE == VARIABLE_LEN
//        RF_Pkt.len = sizeof(RF_Pkt)-1;     // 1st byte of pld is to specify the length of the rest of packet
//
//        p = (unsigned char *)(&RF_Pkt.fill);
//        for(i = sizeof(RF_Pkt.fill); i > 0; i--) {
//            *p++ = j++;      // 0x00, 0x01... as demo
//        }
//
//        RF_Pkt.test_id = TEST_ID;
//        RF_Pkt.counter = ++counter;
//        RF_Pkt.key = test_val;
//    #endif
//}

unsigned char BuildPacket(unsigned char *buffer, unsigned int len)
{
	unsigned char i, *p, *p_dest;
	static unsigned char counter = 0;

	if (len > sizeof(RF_Pkt.fill))
		return 0;

#if LEN_TYPE == FIXED_LEN
	p		= (unsigned char *)(&RF_Pkt.fill);
	p_dest	= (unsigned char *)buffer;
	for(i = len; i > 0; i--)
	{
	    *p++ = *p_dest++;
	}
	
	RF_Pkt.test_id = TEST_ID;
	RF_Pkt.counter = ++counter;
	RF_Pkt.key = len;
#endif

#if LEN_TYPE == VARIABLE_LEN
	RF_Pkt.len = sizeof(RF_Pkt)-1;     // 1st byte of pld is to specify the length of the rest of packet
	
	p		= (unsigned char *)(&RF_Pkt.fill);
	p_dest	= (unsigned char *)buffer;
	for(i = len; i > 0; i--)
	{
	    *p++ = *p_dest++;
	}
	
	RF_Pkt.test_id = TEST_ID;
	RF_Pkt.counter = ++counter;
	RF_Pkt.key = len;
#endif

	return 1;
}


/*=====================================================
SendRfFrame

input
buffer: pointer to send buffer
len: packet length

call example:
SendRfFrame((unsigned char *)(&RF_Pkt), sizeof(RF_Pkt));
=====================================================*/
#if DATA_MODE == PACKET_MODE
void SendRfFrame (unsigned char *buffer, unsigned char len) {
    SetRFMode(RF_STANDBY);
    SpiWriteCfg(REG_PKTPARAM4, RF_PKT4_FIFO_STANDBY_WRITE);  // allow write FIFO in standby mode
    SpiWriteCfg(REG_IRQPARAM1, RF_IRQ1_TX_TXDONE);  // IRQ1 = Tx done
    clearFIFO();

    do {
        clr_NSS_DATA;        // select "data register"
        SpiInOut(*buffer++);
        set_NSS_DATA;        // pulse on data nss every byte
    }while(--len);

    SetRFMode(RF_TRANSMITTER);
    while(!RF_IRQ1());        // Tx done
    SetRFMode(RF_SLEEP);
}
#endif

#if DATA_MODE == BUFFER_MODE
void SendRfFrame (unsigned char *buffer, unsigned char len) {
    unsigned char i;

    SetRFMode(RF_SLEEP);
    SpiWriteCfg(REG_IRQPARAM1, RF_IRQ0_TX_FIFOTHRESH_START_FIFOTHRESH|RF_IRQ1_TX_FIFOFULL);
    clearFIFO();

    SetRFMode(RF_TRANSMITTER);

    // preamble and sysncword must be handle by MCU itself in buffer mode
    for(i = 4; i > 0; i--) {
        clr_NSS_DATA;
        SpiInOut(0x55);     // 4 bytes preamble
        set_NSS_DATA;
    }

    clr_NSS_DATA;
    SpiInOut(SYNC_WORD1);
    set_NSS_DATA;

    clr_NSS_DATA;
    SpiInOut(SYNC_WORD2);
    set_NSS_DATA;

    clr_NSS_DATA;
    SpiInOut(SYNC_WORD3);
    set_NSS_DATA;

    clr_NSS_DATA;
    SpiInOut(SYNC_WORD4);
    set_NSS_DATA;

    do {
        while(RF_IRQ1());      // wait if FIFO is full

        clr_NSS_DATA;        // select "data register"
        SpiInOut(*buffer++);
        set_NSS_DATA;        // pulse on data nss every byte
    }while(--len);

    SpiWriteCfg(REG_IRQPARAM1, RF_IRQ1_TX_TXDONE);
    while(!RF_IRQ1());         // Tx done
    SetRFMode(RF_SLEEP);
}
#endif


/*=====================================================
ReceiveRfFrame

input
buffer: pointer to receive buffer
len: packet length

output
rc: return code
    - RF_RX_RUNNING  Rx running
    - ERROR          error
    - OK             packet received

note: for VARIABLE_LEN, buffer[0] is the actual len of payload

call example:
ReceiveRfFrame((unsigned char *)(&RF_Pkt), sizeof(RF_Pkt), &rc);
=====================================================*/
#if DATA_MODE == PACKET_MODE
#if LEN_TYPE == FIXED_LEN
void ReceiveRfFrame (unsigned char *buffer, unsigned char len, unsigned char *rc) {
    if(SetRFMode(RF_RECEIVER) == MODE_CHANGED) {
        SpiWriteCfg(REG_IRQPARAM1, RF_RX_IRQ0_PLD_RDY);         // IRQ0 = payload ready
        SpiWriteCfg(REG_PKTPARAM4, RF_PKT4_FIFO_STANDBY_READ);  // allow read FIFO in standby mode
    }
    *rc = RF_RX_RUNNING;

    if(RF_IRQ0()) {
        SetRFMode(RF_STANDBY);

        do {
            clr_NSS_DATA;                 // select "data register"
            *buffer++ = SpiInOut(0);
            set_NSS_DATA;                 // pulse on data nss every byte
        }while(--len);
        clearFIFO();
        SetRFMode(RF_SLEEP);
        *rc = OK;
    }
}
#endif

#if LEN_TYPE == VARIABLE_LEN
void ReceiveRfFrame (unsigned char *buffer, unsigned char buffer_size, unsigned char *rc) {
    unsigned char t_len;      // len from RF payload

    if(SetRFMode(RF_RECEIVER) == MODE_CHANGED) {
        SpiWriteCfg(REG_IRQPARAM1, RF_RX_IRQ0_PLD_RDY);         // IRQ0 = payload ready
        SpiWriteCfg(REG_PKTPARAM4, RF_PKT4_FIFO_STANDBY_READ);  // allow read FIFO in standby mode
    }
    *rc = RF_RX_RUNNING;

    if(RF_IRQ0()) {
        SetRFMode(RF_STANDBY);

        clr_NSS_DATA;         // select "data register"
        *buffer = SpiInOut(0);  // 1st byte of pld is to specify the length of the rest of packet
        set_NSS_DATA;         // pulse on data nss

        t_len = *buffer++;

        if(t_len > buffer_size || t_len == 0) {
            // buffer is not big enough to store pld or len = 0
            *rc = ERROR;
            clearFIFO();
            SetRFMode(RF_SLEEP);
            return;
        }

        do {
            clr_NSS_DATA;                 // select "data register"
            *buffer++ = SpiInOut(0);
            set_NSS_DATA;                 // pulse on data nss every byte
        }while(--t_len);
        clearFIFO();
        SetRFMode(RF_SLEEP);
        *rc = OK;
    }
}
#endif
#endif


#if DATA_MODE == BUFFER_MODE
#if LEN_TYPE == FIXED_LEN
void ReceiveRfFrame (unsigned char *buffer, unsigned char len, unsigned char *rc) {
    if(SetRFMode(RF_RECEIVER) == MODE_CHANGED) {
        SpiWriteCfg(REG_IRQPARAM1, RF_RX_IRQ0_N_FIFOEMPTY);
        SpiWriteCfg(REG_IRQPARAM2, RF_RX_FIFO_FILL_AUTO);
    }
    *rc = RF_RX_RUNNING;

    if(RF_IRQ0()) {
        do {
            while(!RF_IRQ0());              // wait if FIFO is empty

            clr_NSS_DATA;                 // select "data register"
            *buffer++ = SpiInOut(0);
            set_NSS_DATA;                 // pulse on data nss every byte
        }while(--len);
        SetRFMode(RF_SLEEP);
        clearFIFO();
        *rc = OK;
    }
}
#endif

#if LEN_TYPE == VARIABLE_LEN
void ReceiveRfFrame (unsigned char *buffer, unsigned char buffer_size, unsigned char *rc) {
    unsigned char t_len;      // len from RF payload

    if(SetRFMode(RF_RECEIVER) == MODE_CHANGED) {
        SpiWriteCfg(REG_IRQPARAM1, RF_RX_IRQ0_N_FIFOEMPTY);
        SpiWriteCfg(REG_IRQPARAM2, RF_RX_FIFO_FILL_AUTO);
    }
    *rc = RF_RX_RUNNING;

    if(RF_IRQ0()) {

        clr_NSS_DATA;         // select "data register"
        *buffer = SpiInOut(0);  // 1st byte of pld is to specify the length of the rest of packet
        set_NSS_DATA;         // pulse on data nss

        t_len = *buffer++;

        if(t_len > buffer_size || t_len == 0) {
            // buffer is not big enough to store pld or len = 0
            *rc = ERROR;
            SetRFMode(RF_SLEEP);
            clearFIFO();
            return;
        }

        do {
            while(!RF_IRQ0());              // wait if FIFO is empty

            clr_NSS_DATA;                 // select "data register"
            *buffer++ = SpiInOut(0);
            set_NSS_DATA;                 // pulse on data nss every byte
        }while(--t_len);
        SetRFMode(RF_SLEEP);
        clearFIFO();
        *rc = OK;
    }
}
#endif
#endif




/*=====================================================
_SpiConfig

SPI read from or write to "configure register"

Note: don't call it directely
use macro SPIReadCfg and SPIWriteCfg
defined in SX1212.h to call this ruotine

for read:
- addr: address to read (01xxxxx0)
- data: dont' care
- return: data back from SX1212

for write:
- addr: address to write (00xxxxx0)
- data: data to write to SX1212
- return: don't care
=====================================================*/
unsigned char _SpiConfig (unsigned char addr, unsigned char val) {
    unsigned char rc;

   SPIInit();


    clr_NSS_CONFIG;             // select "configure register"

    SpiInOut(addr);
    rc = SpiInOut(val);

    set_NSS_CONFIG;
    return rc;
}


/*=====================================================
SpiInOut

=====================================================*/
static unsigned char SpiInOut (unsigned char mosi) {
    unsigned char i;

    for(i = 8; i > 0; i--) {
        _nop_spi();

        if(mosi & 0x80) {
            set_MOSI;
        }
        else {
            clr_MOSI;
        }

        _nop_spi();
        set_SCK;

        mosi <<= 1;      // next bit

        if(SPI_MISO()) {
            mosi++;      // 1 found on MISO
        }

        _nop_spi();
        clr_SCK;

    }

    return mosi;         // mosi now becomes the value of miso
}
