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

static   INT8U RFState = RF_STOP;     // RF state machine
static   INT8U *pRFFrame;             // Pointer to the RF frame
static   INT8U RFFramePos;            // RF payload current position
static   INT8U RFFrameSize;           // RF payload size
static  INT16U ByteCounter = 0;       // RF frame byte counter
static   INT8U PreMode = RF_STANDBY;  // Previous chip operating mode
static   INT8U SyncSize = 4;          // Size of sync word
static   INT8U SyncValue[4];       // Value of sync word
static  INT32U RFFrameTimeOut = RF_FRAME_TIMEOUT(1600); // Reception counter value (full frame timeout generation)

/*自己写的变量*/
static INT8U flagEnableTimeOut=1;

/*以下是自己修改的接口程序*/
//初始化SPI 这里目前使用SPI1的默认模式 可能后续需要修改
void SPIInit(){ 
  SPI_DeInit  ( SPI1) ;   
  CLK_PeripheralClockConfig(CLK_Peripheral_SPI1, ENABLE); 
}

//设置IRQ_0 1的使能 通过设置对应IO口的工作方式
void enable_IRQ(int n){
  if(n==0){
    //IRQ 0
    disableInterrupts();
    GPIO_Init(GPIOB, GPIO_Pin_4, GPIO_Mode_In_FL_IT);
    EXTI_SetPinSensitivity(EXTI_Pin_4,EXTI_Trigger_Rising);
    enableInterrupts();
    EXTI_ClearITPendingBit(EXTI_IT_Pin4); //这里不知道用不用清  清一下没坏处
  }
  else if(n==1){
    disableInterrupts();
    GPIO_Init(GPIOB, GPIO_Pin_3, GPIO_Mode_In_FL_IT);
    EXTI_SetPinSensitivity(EXTI_Pin_3,EXTI_Trigger_Rising);
    enableInterrupts();
    EXTI_ClearITPendingBit(EXTI_IT_Pin3); //这里不知道用不用清  清一下没坏处
  }
}
//禁用IRQ_0 1的使能 通过设置对应IO口的工作方式
void disable_IRQ(int n){
  if(n==0){
    GPIO_Init(GPIOB, GPIO_Pin_4, GPIO_Mode_Out_PP_Low_Slow);
  }
  else if(n==1){
    GPIO_Init(GPIOB, GPIO_Pin_3, GPIO_Mode_Out_PP_Low_Slow);
  }
}

//置位 使用#define在.h中实现

INT16U RegistersCfg[] = { // SX1212 configuration registers values
               //0X30
                DEF_MCPARAM1 | RF_MC1_STANDBY | RF_MC1_BAND_430470 | RF_MC1_SUBBAND_FIRST,                    
                DEF_MCPARAM2 | RF_MC2_DATA_MODE_PACKET | RF_MC2_MODULATION_FSK | RF_MC2_OOK_THRESH_TYPE_PEAK | RF_MC2_GAIN_IF_00,                      
                DEF_FDEV | RF_FDEV_100,
                DEF_BITRATE_MSB | RF_BITRATE_MSB_25000,
                DEF_BITRATE_LSB | RF_BITRATE_LSB_25000,                  
                DEF_MCPARAM6 | RF_MC6_PARAMP_11 | RF_MC6_LOW_POWER_RX_OFF | RF_MC6_VCO_TRIM_11 | RF_MC6_RPS_SELECT_1,                   
                DEF_R1 | 107,                            
                DEF_P1 | 42,                            
                DEF_S1 | 30,                            
                DEF_R2 | RF_R2_VALUE,                            
                DEF_P2 | RF_P2_VALUE,                            
                DEF_S2 | RF_S2_VALUE,                         
                
                DEF_FIFOCONFIG | RF_FIFO_SIZE_64 | RF_FIFO_THRESH_VALUE,
                DEF_IRQPARAM1 | RF_IRQ0_RX_STDBY_PAYLOADREADY | RF_IRQ1_RX_STDBY_CRCOK | RF_IRQ0_TX_FIFOEMPTY_START_FIFONOTEMPTY | RF_IRQ1_TX_TXDONE,                     
                DEF_IRQPARAM2 | RF_IRQ2_PLL_LOCK_PIN_ON,                     
                DEF_RSSIIRQTHRESH | RF_RSSIIRQTHRESH_VALUE,                 
                
                DEF_RXPARAM1 | RF_RX1_PASSIVEFILT_378 | RF_RX1_FC_FOPLUS100,                      
                DEF_RXPARAM2 | RF_RX2_FO_100,                      
                DEF_RXPARAM3 | RF_RX3_POLYPFILT_OFF | RF_RX3_SYNC_SIZE_32 | RF_RX3_SYNC_TOL_0,                      
                DEF_OOKFLOORTHRESH | RF_OOKFLOORTHRESH_VALUE,                       
      //RSSI Value (Read only)               
                DEF_RXPARAM6 | RF_RX6_OOK_THRESH_DECSTEP_000 | RF_RX6_OOK_THRESH_DECPERIOD_000 | RF_RX6_OOK_THRESH_AVERAGING_00,                       
                
                DEF_SYNCBYTE1 | 0x69, // 1st byte of Sync word,                     
                DEF_SYNCBYTE2 | 0x81, // 2nd byte of Sync word,                     
                DEF_SYNCBYTE3 | 0x7E, // 3rd byte of Sync word,                     
                DEF_SYNCBYTE4 | 0x96, // 4th byte of Sync word,                     
                
                DEF_TXPARAM | RF_TX_FC_200 | RF_TX_POWER_PLUS10 | RF_TX_ZERO_IF_OFF,                       
                
                DEF_OSCPARAM | RF_OSC_CLKOUT_ON | RF_OSC_CLKOUT_427,                     

                DEF_PKTPARAM1 | RF_PKT1_MANCHESTER_OFF | 64,                  
                DEF_NODEADRS  | RF_NODEADRS_VALUE,                 
                DEF_PKTPARAM3 | RF_PKT3_FORMAT_VARIABLE | RF_PKT3_PREAMBLE_SIZE_32 | RF_PKT3_WHITENING_OFF | RF_PKT3_CRC_ON | RF_PKT3_ADRSFILT_00,                    
                DEF_PKTPARAM4 | RF_PKT4_AUTOCLEAR_ON | RF_PKT4_FIFO_STANDBY_WRITE
   
};


/*******************************************************************
** InitRFChip : This routine initializes the RFChip registers     **
**              Using Pre Initialized variables                   **
********************************************************************
** In  : -                                                        **
** Out : -                                                        **
*******************************************************************/
void InitRFChip (void){
    INT16U i;
    // Initializes SX1212
    SPIInit();
    
    //初始化IO口
    GPIO_Init(GPIOB,GPIO_Pin_5,GPIO_Mode_Out_PP_Low_Fast);  //替换的是set_bit(PORTO, (SCK + NSS_DATA + NSS_CONFIG + MOSI)); 如果模式驱动不了 再改
    GPIO_Init(GPIOD,GPIO_Pin_4,GPIO_Mode_Out_PP_Low_Fast);
    GPIO_Init(GPIOD,GPIO_Pin_5,GPIO_Mode_Out_PP_Low_Fast);
    GPIO_Init(GPIOB,GPIO_Pin_6,GPIO_Mode_Out_PP_Low_Fast);
    //上述4个口应该都是out 这句不用配置                     //替换的是set_bit(PORTP, (SCK + NSS_DATA + NSS_CONFIG + MOSI)); 如果模式驱动不了 再改
    GPIO_Init(GPIOB,GPIO_Pin_7,GPIO_Mode_In_PU_No_IT);  //MISO 不需要中断
    GPIO_Init(GPIOB,GPIO_Pin_4,GPIO_Mode_In_FL_IT);     //IRQ_0 外部中断
    GPIO_Init(GPIOB,GPIO_Pin_3,GPIO_Mode_In_FL_IT);     //IRQ_1 外部中断
    
    /*初始化其他外设*/
    //初始化外部中断
    EXTI_SelectPort( EXTI_Port_B);    //选择B口接受外部中断
    /* Enable TIM1 clock */
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM1, ENABLE);
    
    
    for(i = 0; (i + 1) <= REG_PKTPARAM4; i++){
        if(i < REG_RSSIVALUE){
            WriteRegister(i, RegistersCfg[i]);
        }
        else{
            WriteRegister(i + 1, RegistersCfg[i]);
        }
    }

    SyncSize = ((RegistersCfg[REG_RXPARAM3] >> 3) & 0x03) + 1;
    for(i = 0; i < SyncSize; i++){
        SyncValue[i] = RegistersCfg[REG_SYNCBYTE1 - 1 + i];
    }

    if((RegistersCfg[REG_BITRATE_MSB] == RF_BITRATE_MSB_1600) && (RegistersCfg[REG_BITRATE_LSB] == RF_BITRATE_LSB_1600)){
        RFFrameTimeOut = RF_FRAME_TIMEOUT(1600);
    }
    else if((RegistersCfg[REG_BITRATE_MSB] == RF_BITRATE_MSB_2000) && (RegistersCfg[REG_BITRATE_LSB] == RF_BITRATE_LSB_2000)){
        RFFrameTimeOut = RF_FRAME_TIMEOUT(2000);
    }
    else if((RegistersCfg[REG_BITRATE_MSB] == RF_BITRATE_MSB_2500) && (RegistersCfg[REG_BITRATE_LSB] == RF_BITRATE_LSB_2500)){
        RFFrameTimeOut = RF_FRAME_TIMEOUT(2500);
    }
    else if((RegistersCfg[REG_BITRATE_MSB] == RF_BITRATE_MSB_5000) && (RegistersCfg[REG_BITRATE_LSB] == RF_BITRATE_LSB_5000)){
        RFFrameTimeOut = RF_FRAME_TIMEOUT(5000);
    }
    else if((RegistersCfg[REG_BITRATE_MSB] == RF_BITRATE_MSB_5000) && (RegistersCfg[REG_BITRATE_LSB] == RF_BITRATE_LSB_5000)){
        RFFrameTimeOut = RF_FRAME_TIMEOUT(8000);
    }
    else if((RegistersCfg[REG_BITRATE_MSB] == RF_BITRATE_MSB_10000) && (RegistersCfg[REG_BITRATE_LSB] == RF_BITRATE_LSB_10000)){
        RFFrameTimeOut = RF_FRAME_TIMEOUT(10000);
    }
    else if((RegistersCfg[REG_BITRATE_MSB] == RF_BITRATE_MSB_20000) && (RegistersCfg[REG_BITRATE_LSB] == RF_BITRATE_LSB_20000)){    
        RFFrameTimeOut = RF_FRAME_TIMEOUT(20000);
    }
    else if((RegistersCfg[REG_BITRATE_MSB] == RF_BITRATE_MSB_25000) && (RegistersCfg[REG_BITRATE_LSB] == RF_BITRATE_LSB_25000)){
        RFFrameTimeOut = RF_FRAME_TIMEOUT(25000);
    }
    else if((RegistersCfg[REG_BITRATE_MSB] == RF_BITRATE_MSB_40000) && (RegistersCfg[REG_BITRATE_LSB] == RF_BITRATE_LSB_40000)){
        RFFrameTimeOut = RF_FRAME_TIMEOUT(40000);
    }
    else if((RegistersCfg[REG_BITRATE_MSB] == RF_BITRATE_MSB_50000) && (RegistersCfg[REG_BITRATE_LSB] == RF_BITRATE_LSB_50000)){
        RFFrameTimeOut = RF_FRAME_TIMEOUT(50000);
    }
    else if((RegistersCfg[REG_BITRATE_MSB] == RF_BITRATE_MSB_100000) && (RegistersCfg[REG_BITRATE_LSB] == RF_BITRATE_LSB_100000)){
        RFFrameTimeOut = RF_FRAME_TIMEOUT(100000);
    }
    else if((RegistersCfg[REG_BITRATE_MSB] == RF_BITRATE_MSB_200000) && (RegistersCfg[REG_BITRATE_LSB] == RF_BITRATE_LSB_200000)){
        RFFrameTimeOut = RF_FRAME_TIMEOUT(200000);
    }
    else {
        RFFrameTimeOut = RF_FRAME_TIMEOUT(1600);
    }

    SetRFMode(RF_SLEEP);
}


/*******************************************************************
** SetRFMode : Sets the SX1212 operating mode                     **
********************************************************************
** In  : mode                                                     **
** Out : -                                                        **
*******************************************************************/
void SetRFMode(INT8U mode){
    if(mode != PreMode){
        if(mode == RF_TRANSMITTER){
        
                        if (PreMode == RF_SLEEP){
                       WriteRegister(REG_MCPARAM1, (RegistersCfg[REG_MCPARAM1] & 0x1F) | RF_STANDBY);                        
                           Wait(TS_OS);                        
                       WriteRegister(REG_MCPARAM1, (RegistersCfg[REG_MCPARAM1] & 0x1F) | RF_SYNTHESIZER);                        
                           Wait(TS_FS);                         
                           WriteRegister(REG_MCPARAM1, (RegistersCfg[REG_MCPARAM1] & 0x1F) | RF_TRANSMITTER);
                           Wait(TS_TR);
                        }

                        else if (PreMode == RF_STANDBY){
                           WriteRegister(REG_MCPARAM1, (RegistersCfg[REG_MCPARAM1] & 0x1F) | RF_SYNTHESIZER);                        
                           Wait(TS_FS);                         
                           WriteRegister(REG_MCPARAM1, (RegistersCfg[REG_MCPARAM1] & 0x1F) | RF_TRANSMITTER);
                           Wait(TS_TR);
                        }

                        else if (PreMode == RF_SYNTHESIZER){
                           WriteRegister(REG_MCPARAM1, (RegistersCfg[REG_MCPARAM1] & 0x1F) | RF_TRANSMITTER);
                           Wait(TS_TR);
                        }

                        else if (PreMode == RF_RECEIVER){
                           WriteRegister(REG_MCPARAM1, (RegistersCfg[REG_MCPARAM1] & 0x1F) | RF_TRANSMITTER);
                           Wait(TS_TR);
                        }                        
                        PreMode = RF_TRANSMITTER;
        }
        
        else if(mode == RF_RECEIVER){
        
            if (PreMode == RF_SLEEP){
                           WriteRegister(REG_MCPARAM1, (RegistersCfg[REG_MCPARAM1] & 0x1F) | RF_STANDBY);                        
                           Wait(TS_OS);                        
                       WriteRegister(REG_MCPARAM1, (RegistersCfg[REG_MCPARAM1] & 0x1F) | RF_SYNTHESIZER);                        
                           Wait(TS_FS); 
                           WriteRegister(REG_MCPARAM1, (RegistersCfg[REG_MCPARAM1] & 0x1F) | RF_RECEIVER);
                           Wait(TS_RE);
                        }

                        else if (PreMode == RF_STANDBY){
                       WriteRegister(REG_MCPARAM1, (RegistersCfg[REG_MCPARAM1] & 0x1F) | RF_SYNTHESIZER);                        
                           Wait(TS_FS); 
                           WriteRegister(REG_MCPARAM1, (RegistersCfg[REG_MCPARAM1] & 0x1F) | RF_RECEIVER);
                           Wait(TS_RE);
                        }

                        else if (PreMode == RF_SYNTHESIZER){
                           WriteRegister(REG_MCPARAM1, (RegistersCfg[REG_MCPARAM1] & 0x1F) | RF_RECEIVER);
                           Wait(TS_RE);                     
                        }

                        else if (PreMode == RF_TRANSMITTER){        
                           WriteRegister(REG_MCPARAM1, (RegistersCfg[REG_MCPARAM1] & 0x1F) | RF_RECEIVER);
                           Wait(TS_RE);
                        }
                        PreMode = RF_RECEIVER;
        }
        
        else if(mode == RF_SYNTHESIZER){
        
            if (PreMode == RF_SLEEP){
                    WriteRegister(REG_MCPARAM1, (RegistersCfg[REG_MCPARAM1] & 0x1F) | RF_STANDBY);                        
                           Wait(TS_OS);                        
                    WriteRegister(REG_MCPARAM1, (RegistersCfg[REG_MCPARAM1] & 0x1F) | RF_SYNTHESIZER);                        
                           Wait(TS_FS); 
                        }

                        else if (PreMode == RF_STANDBY){
                    WriteRegister(REG_MCPARAM1, (RegistersCfg[REG_MCPARAM1] & 0x1F) | RF_SYNTHESIZER);                        
                        Wait(TS_FS); 
                        }

                        else {
                    WriteRegister(REG_MCPARAM1, (RegistersCfg[REG_MCPARAM1] & 0x1F) | RF_SYNTHESIZER);                        
                        }
            
            PreMode = RF_SYNTHESIZER;
        }
        
        else if(mode == RF_STANDBY){
        
            if (PreMode == RF_SLEEP){
                    WriteRegister(REG_MCPARAM1, (RegistersCfg[REG_MCPARAM1] & 0x1F) | RF_STANDBY);                        
                        Wait(TS_OS);
                        }

                        else {
                    WriteRegister(REG_MCPARAM1, (RegistersCfg[REG_MCPARAM1] & 0x1F) | RF_STANDBY);                       
                        }
                        
                        PreMode = RF_STANDBY;
        }
        
        else {// mode == RF_SLEEP
        WriteRegister(REG_MCPARAM1, (RegistersCfg[REG_MCPARAM1] & 0x1F) | RF_SLEEP);
        PreMode = RF_SLEEP;
        }
    }
}

/*******************************************************************
** WriteRegister : Writes the register value at the given address **
**                  on the SX1212                                 **
********************************************************************
** In  : address, value                                           **
** Out : -                                                        **
*******************************************************************/
void WriteRegister(INT8U address, INT16U value){
    
    SPIInit();
    address = (address << 1) & 0x3E ;
    set_NSS_DATA;
    clr_NSS_CONFIG;
    SpiInOut(address);
    SpiInOut(value);
    set_NSS_CONFIG;
}

/*******************************************************************
** ReadRegister : Reads the register value at the given address on**
**                the SX1212                                      **
********************************************************************
** In  : address                                                  **
** Out : value                                                    **
*******************************************************************/
INT16U ReadRegister(INT8U address){
    INT8U value = 0;

    SPIInit();
    set_NSS_DATA;
    address = ((address << 1) & 0x7E) | 0x40;
    clr_NSS_CONFIG;
    SpiInOut(address);
    value = SpiInOut(0);
    set_NSS_CONFIG;

    return value;
}

/*******************************************************************
** Communication functions                                        **
*******************************************************************/

/*******************************************************************
** SendRfFrame : Sends a RF frame                                 **
********************************************************************
** In  : *buffer, size                                            **
** Out : *pReturnCode                                             **
*******************************************************************/
void SendRfFrame(INT8U *buffer, INT8U size, INT8U *pReturnCode){
    if((size+1) > (((RegistersCfg[REG_FIFOCONFIG])>>6)+1)*16){  // If (size + length byte) > FIFO size
        RFState |= RF_STOP;
        *pReturnCode = ERROR;
        return;
    }

    RFState |= RF_BUSY;
    RFState &= ~RF_STOP;
    RFFrameSize = size;
    pRFFrame = buffer;
    
    SetRFMode(RF_STANDBY);
    WriteRegister(REG_PKTPARAM4, (RegistersCfg[REG_PKTPARAM4-1] & 0xBF) | RF_PKT4_FIFO_STANDBY_WRITE);
    
    SendByte(RFFrameSize);
    for(ByteCounter = 0, RFFramePos = 0; ByteCounter < RFFrameSize;){
            SendByte(pRFFrame[RFFramePos++]); 
            ByteCounter++; 
    }

    SetRFMode(RF_TRANSMITTER); // Cf RF_IRQ0_TX_FIFOEMPTY_START_FIFONOTEMPTY in RegistersConfig 
    
    do{
    }while(!read_IRQ_1); // Wait for TX done. Cf RF_IRQ1_TX_TXDONE in RegistersConfig
    Wait(1000); // Wait for last bit to be sent (worst case bitrate)

    //SetRFMode(RF_SLEEP);
    SetRFMode(RF_STANDBY);


    RFState |= RF_STOP;
    RFState &= ~RF_TX_DONE;
    *pReturnCode = OK;
} // void SendRfFrame(INT8U *buffer, INT8U size, INT8U *pReturnCode)

/*******************************************************************
** ReceiveRfFrame : Receives a RF frame                           **
********************************************************************
** In  : -                                                        **
** Out : *buffer, size, *pReturnCode                              **
*******************************************************************/
void ReceiveRfFrame(INT8U *buffer, INT8U *size, INT8U *pReturnCode){
    
       INT8U TempRFState;
       
       *pReturnCode = RX_RUNNING;

       TempRFState = RFState; 

    if(TempRFState & RF_STOP){
        pRFFrame = buffer;
        RFFramePos = 0;
        RFFrameSize = 2;
                
        //RegIrqEnMid |= 0x02; // Enables Port A pin 1 (PA1=IRQ1)interrupt IRQ_1 (CRCOK, Cf RF_IRQ1_RX_STDBY_CRCOK in RegistersConfig)
        enable_IRQ(1);
        
        SetRFMode(RF_RECEIVER);
        EnableTimeOut(TRUE);
        RFState |= RF_BUSY;
        RFState &= ~RF_STOP;
        RFState &= ~RF_TIMEOUT;
        return;
    }
    else if(TempRFState & RF_RX_DONE){

        SetRFMode(RF_STANDBY);        
        WriteRegister(REG_PKTPARAM4, (RegistersCfg[REG_PKTPARAM4-1] & 0xBF) | RF_PKT4_FIFO_STANDBY_READ);
            
        RFFrameSize = ReceiveByte();
        for(ByteCounter = 0, RFFramePos = 0; ByteCounter < RFFrameSize; ){
                        pRFFrame[RFFramePos++] = ReceiveByte();
            ByteCounter++; 
        }
        //SetRFMode(RF_SLEEP);
        
        *size = RFFrameSize;
        *pReturnCode = OK;
        RFState |= RF_STOP;
        EnableTimeOut(FALSE);
        RFState &= ~RF_RX_DONE;
        disable_IRQ(1);//RegIrqEnMid &= ~0x02; // Disables Port A pin 1 interrupt IRQ_1 (CRCOK, Cf RF_IRQ1_RX_STDBY_CRCOK in RegistersConfig)
        set_NSS_DATA;
        return;
    }
    else if(TempRFState & RF_ERROR){
        RFState |= RF_STOP;
        RFState &= ~RF_ERROR;
        *pReturnCode = ERROR;
        disable_IRQ(1); // Disables Port A pin 1 interrupt IRQ_1 (CRCOK, Cf RF_IRQ1_RX_STDBY_CRCOK in RegistersConfig)
        set_NSS_DATA;
        return;
    }
    else if(TempRFState & RF_TIMEOUT){
        RFState |= RF_STOP;
        RFState &= ~RF_TIMEOUT;
        EnableTimeOut(FALSE);
        *pReturnCode = RX_TIMEOUT;
        disable_IRQ(1); // Disables Port A pin 1 interrupt IRQ_1 (CRCOK, Cf RF_IRQ1_RX_STDBY_CRCOK in RegistersConfig)
        set_NSS_DATA;
        return;
    }
} // void ReceiveRfFrame(INT8U *buffer, INT8U size, INT8U *pReturnCode)

/*******************************************************************
** SendByte : Sends a data to the transceiver trough the SPI      **
**            interface                                           **
********************************************************************
** In  : b                                                        **
** Out : -                                                        **
*******************************************************************/
/*******************************************************************
**  Information                                                   **
********************************************************************
** This function has been optimized to send a byte to the         **
** transceiver SPI interface as quick as possible by using        **
** IO ports.                                                      **
** If the microcontroller has an  SPI hardware interface there is **
** no need to optimize the function                               **
*******************************************************************/
void SendByte(INT8U b){
    SPIInit();
    set_NSS_CONFIG;
    clr_NSS_DATA;

    clr_SCK;
    if (b & 0x80){
        set_MOSI;
    }
    else{
        clr_MOSI;
    }
    set_SCK;
    clr_SCK;
    if (b & 0x40){
        set_MOSI;
    }
    else{
        clr_MOSI;
    }
    set_SCK;
    clr_SCK;
    if (b & 0x20){
        set_MOSI;
    }
    else{
        clr_MOSI;
    }
    set_SCK;
    clr_SCK;
    if (b & 0x10){
        set_MOSI;
    }
    else{
        clr_MOSI;
    }
    set_SCK;
    clr_SCK;
    if (b & 0x08){
        set_MOSI;
    }
    else{
        clr_MOSI;
    }
    set_SCK;
    clr_SCK;
    if (b & 0x04){
        set_MOSI;
    }
    else{
        clr_MOSI;
    }
    set_SCK;
    clr_SCK;
    if (b & 0x02){
        set_MOSI;
    }
    else{
        clr_MOSI;
    }
    set_SCK;
    clr_SCK;
    if (b & 0x01){
        set_MOSI;
    }
    else{
        clr_MOSI;
    }
    set_SCK;
    clr_SCK;
    clr_MOSI;

    set_NSS_DATA;
} // void SendByte(INT8U b)

/*******************************************************************
** ReceiveByte : Receives a data from the transceiver trough the  **
**               SPI interface                                    **                                                
********************************************************************
** In  : -                                                        **
** Out : b                                                        **
*******************************************************************/
/*******************************************************************
**  Information                                                   **
********************************************************************
** This function has been optimized to receive a byte from the    **
** transceiver SPI interface as quick as possible by using        **
** IO ports.                                                      **
** If the microcontroller has an  SPI hardware interface there is **
** no need to optimize the function                               **
*******************************************************************/
INT8U ReceiveByte(void){
    INT8U inputByte = 0;

    SPIInit();
    set_NSS_CONFIG;
    clr_NSS_DATA;

        clr_SCK;
    set_SCK;
    if (SPIMisoTest()){
        inputByte |= 0x80;
    }
    clr_SCK;
    set_SCK;
    if (SPIMisoTest()){
        inputByte |= 0x40;
    }
    clr_SCK;
    set_SCK;
    if (SPIMisoTest()){
        inputByte |= 0x20;
    }
    clr_SCK;
    set_SCK;
    if (SPIMisoTest()){
        inputByte |= 0x10;
    }
    clr_SCK;
    set_SCK;
    if (SPIMisoTest()){
        inputByte |= 0x08;
    }
    clr_SCK;
    set_SCK;
    if (SPIMisoTest()){
        inputByte |= 0x04;
    }
    clr_SCK;
    set_SCK;
    if (SPIMisoTest()){
        inputByte |= 0x02;
    }
    clr_SCK;
    set_SCK;
    if (SPIMisoTest()){
        inputByte |= 0x01;
    }
    clr_SCK;
        clr_MOSI;

    set_NSS_DATA;

        return inputByte;
}
/*******************************************************************
** Transceiver specific functions                                 **
*******************************************************************/

/*******************************************************************
** ReadRssi : Reads the Rssi value from the SX1212                **
********************************************************************
** In  : -                                                        **
** Out : value                                                    **
*******************************************************************/
INT16U ReadRssi(void){
        INT16U value;
        value = ReadRegister(REG_RSSIVALUE);  // Reads the RSSI result
        return value;
}

/*******************************************************************
** Utility functions                                              **
*******************************************************************/

/*******************************************************************
** Wait : This routine uses the counter A&B to create a delay     **
**        using the RC ck source                                  **
********************************************************************
** In   : cntVal                                                  **
** Out  : -                                                       **
*******************************************************************/
//等待一段时间. 等待cntVal*407ns 每个nop需时间1s/16M=62ns 大约7个nop.先对付一下.不行再想办法
void Wait(INT16U cntVal){
  INT16U i,j;
  if(flagEnableTimeOut){
    
    for(i=0;i<cntVal;i++){
      for(j=0;j<7;i++) asm("nop");
    }
  }
} // void Wait(INT16U cntVal)

/*******************************************************************
** EnableTimeOut : Enables/Disables the RF frame timeout          **
********************************************************************
** In  : enable                                                   **
** Out : -      
void EnableTimeOut(INT8U enable){
    RegCntCtrlCk = (RegCntCtrlCk & 0xFC) | 0x03;        // Selects 128 Hz frequency as clock source for counter A&B
    RegCntConfig1 |=  0x34;                             // A&B counters count up, counter A&B  are in cascade mode

    RegCntA = (INT8U)RFFrameTimeOut;                      // LSB of RFFrameTimeOut
    RegCntB = (INT8U)(RFFrameTimeOut >> 8);               // MSB of RFFrameTimeOut

    if(enable){
        RegIrqEnHig |= 0x10;                            // Enables IRQ for the counter A&B
        RegCntOn |= 0x03;                               // Enables counter A&B
    }
    else{
        RegIrqEnHig &= ~0x10;                           // Disables IRQ for the counter A&B
        RegCntOn &= ~0x03;                              // Disables counter A&B
    }
} // void EnableTimeOut(INT8U enable)**
*******************************************************************/
void EnableTimeOut(INT8U enable){
  if(enable){
    flagEnableTimeOut=1;
  }
  else{
    flagEnableTimeOut=0;
  } 
}

/*******************************************************************
** InvertByte : Inverts a byte. MSB -> LSB, LSB -> MSB            **
********************************************************************
** In  : b                                                        **
** Out : b                                                        **
INT8U InvertByte(INT8U b){
    asm("   move %r0, #0x08");
    asm("LoopInvertByte:");
    asm("   shl  %r3");
    asm("   shrc %r2");
    asm("   dec  %r0");
    asm("   jzc  LoopInvertByte");
} // INT8U InvertByte(INT8U b)
*******************************************************************/
//注意 在keil a=InvertByte(a)会出错
INT8U InvertByte(INT8U b){
  INT8U tmpA=b; //保护数据源,尽量不直接对b操作.在keil下会异常
  INT8U tmpB=0x00;
  INT8U shifter1=0x01;
  INT8U shifter2=0x80;
  do{
    if((tmpA&shifter1)==shifter1) 
      tmpB=(tmpB|shifter2);
    shifter1=(shifter1<<1);
    shifter2=(shifter2>>1);
  }
  while(shifter1!=0x80);
  return tmpB;
}

/*******************************************************************
** SpiInOut : Sends and receives a byte from the SPI bus          **
********************************************************************
** In  : outputByte                                               **
** Out : inputByte                                                **
*******************************************************************/
INT8U SpiInOut (INT8U outputByte){
    INT8U bitCounter;
    INT8U inputByte = 0;

    clr_SCK;
    for(bitCounter = 0x80; bitCounter != 0x00; bitCounter >>= 1){
        if (outputByte & bitCounter){
            set_MOSI;
        }
        else{
            clr_MOSI;
        }
        set_SCK;
        if (SPIMisoTest()){
            inputByte |= bitCounter;
        }
        clr_SCK;
    }  // for(BitCounter = 0x80; BitCounter != 0x00; BitCounter >>= 1)
    clr_MOSI;

    return inputByte;
} // INT8U SpiInOut (INT8U outputByte)

/*******************************************************************
** SX1212 Packet interrupt handlers                             **
*******************************************************************/

/*******************************************************************
** Handle_Irq_Pa1 : Handles the interruption from the Pin 1 of    **
**                  Port A                                        **
********************************************************************
** In  : -                                                        **
** Out : -                                                        **
*******************************************************************/
void Handle_Irq_Pa1 (void){ // IRQ_1 = CRC_OK , Cf RF_IRQ1_RX_STDBY_CRCOK in RegistersConfig (Irq routine only used in Rx)
 
     RFState |= RF_RX_DONE;
     RFState &= ~RF_BUSY;
        
} //End Handle_Irq_Pa1

/*******************************************************************
** Handle_Irq_Pa2 : Handles the interruption from the Pin 2 of    **
**                  Port A                                        **
********************************************************************
** In  : -                                                        **
** Out : -                                                        **
*******************************************************************/
void Handle_Irq_Pa2 (void){ // IRQ_0 


} //End Handle_Irq_Pa2

/*******************************************************************
** Handle_Irq_CntA : Handles the interruption from the Counter A  **
********************************************************************
** In              : -                                            **
** Out             : -                                            **
*******************************************************************/
void Handle_Irq_CntA (void){
    RFState |= RF_TIMEOUT;
    RFState &= ~RF_BUSY;
} //End Handle_Irq_CntA

