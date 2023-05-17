#include "SPI.h"

/*==============================================================================
SPI_ExchangeByte( );
Function : Exchange a byte via the SPI bus
INTPUT   : input, The input byte
OUTPUT   : The output byte from SPI bus
==============================================================================*/
INT8U SPI_ExchangeByte( INT8U input )
{
   
  /* Loop while DR register in not emplty */
  while (SPI_GetFlagStatus(SPI1, SPI_FLAG_TXE) == RESET);

  /* Send byte through the SPI1 peripheral */
  SPI_SendData(SPI1,input);

  /* Wait to receive a byte */
  while (SPI_GetFlagStatus(SPI1,SPI_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
  return SPI_ReceiveData(SPI1);
}

/*==============================================================================
SPI_Initial( );
Function : Initialize the SPI bus
INTPUT   : None
OUTPUT   : None
==============================================================================*/
void SPI_Initial( void )
{
  /* 初始化SPI */
  /*
    高位在前 4分频（16/4Mhz）主机模式 空闲低电平  上升沿触发 双工模式 
    软件片选  开启crc7校验 
  */
        CLK_PeripheralClockConfig(CLK_Peripheral_SPI1, ENABLE); 
        SPI_DeInit(SPI1);
	SPI_Init(SPI1,SPI_FirstBit_MSB, SPI_BaudRatePrescaler_4, SPI_Mode_Master,\
		SPI_CPOL_Low, SPI_CPHA_1Edge, \
		SPI_Direction_2Lines_FullDuplex, SPI_NSS_Soft, 0x07);
	    
	 SPI_Cmd(SPI1,ENABLE);	/* 使能SPI */
         		//Config the GPIOs for SPI bus

}

/*==============================================================================
GPIO_Initial( );
Function : Initialize the other GPIOs of the board
INTPUT   : None
OUTPUT   : None
==============================================================================*/
void GPIO_Initial( void )
{
          GPIO_Init( GPIOB, GPIO_Pin_7,GPIO_Mode_In_PU_No_IT );
        GPIO_Init( GPIOB, GPIO_Pin_5 | GPIO_Pin_6, GPIO_Mode_Out_PP_Low_Fast );
    //config the CSN for RF module, CSN = PB2
    GPIO_Init( GPIOB, GPIO_Pin_2, GPIO_Mode_Out_PP_Low_Fast );
    GPIO_SetBits( GPIOB, GPIO_Pin_2);
		
    //config the PD4、PD5, CFG = PD5, DATA = PD4 for Sx1212
    GPIO_Init( GPIOD, GPIO_Pin_4|GPIO_Pin_5, GPIO_Mode_Out_PP_Low_Fast );
		
    //config the IRQ0 and IRQ1 for Sx1212, IRQ0 = PB4, IRQ1 = PB3
    GPIO_Init( GPIOB, GPIO_Pin_4 | GPIO_Pin_3, GPIO_Mode_In_FL_No_IT );
    
}

/*==============================================================================
------------------------------------End of FILE---------------------------------
==============================================================================*/

