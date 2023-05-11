/******************************************************************************************
    |------------------------|
    |  TXD - PC3(UART1->RX)  |
    |  RXD - PC2(UART1->TX)  |
    |------------------------|

*******************************************************************************/

/* 包含系统头文件 */
#include <stdarg.h>

/* 包含自定义头文件 */
#include "usart.h"
/* 自定义新类型 */

/* 自定义宏 */

uart usart1;
/* 全局变量定义 */
/*******************************************************************************
 * 名称: Uart_Init
 * 功能: UART1初始化操作
 * 形参: 无
 * 返回: 无
 * 说明: 无
 ******************************************************************************/
void Usart_Init(void)
{
    /*
    初始化IO
    TxD  配置成输出上拉高速模式
    RxD  配置成输入上拉无中断模式
      */
    CLK_PeripheralClockConfig(CLK_Peripheral_USART1, ENABLE); // 开启USART时钟

    USART_DeInit(USART1); // 重新将USART1设置为缺省值的USART1

    GPIO_Init(USART_GPIO_PORT, TXD_GPIO_PINS, GPIO_Mode_Out_PP_High_Fast); // 初始化，推挽输出，快速
    GPIO_Init(USART_GPIO_PORT, RXD_GPIO_PINS, GPIO_Mode_In_PU_No_IT);      // 初始化，带上拉输入，不带中断
    //       GPIO_Init(USART_GPIO_PORT, CS_GPIO_PINS, GPIO_Mode_Out_PP_High_Slow);

    /*
        SYSCFG_REMAPDeInit();//引脚复用功能
            SYSCFG_REMAPPinConfig(REMAP_Pin_USART1TxRxPortC,ENABLE);//将USART Tx--Rx（）映射到PC5-PC6
      */

    /*
     * 将UART1配置为：
     * 波特率 = 115200
     * 数据位 = 8
     * 1位停止位
     * 无校验位
     * 使能接收和发送
     * 使能接收中断
     */

    USART_Init(USART1, (uint32_t)115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, USART_Mode_Tx | USART_Mode_Rx); // 设置USART参数115200，8N1，接收/发送

    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    USART_ITConfig(USART1, USART_IT_OR, ENABLE); // 使能接收中断,中断向量号为28
    // USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    // USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
    //  USART_ITConfig (USART1,USART_IT_TC,ENABLE);//使能接收中断,中断向量号为28
    USART_Cmd(USART1, ENABLE); // 使能USART
}
// 初始化串口
void Usart1_clear_init(void)
{
    memset(usart1.rx_data, 0, sizeof(usart1.rx_data));
    memset(usart1.tx_data, 0, sizeof(usart1.tx_data));
}
/*******************************************************************************
 * 名称: UART1_SendByte
 * 功能: UART1发送一个字节
 * 形参: data -> 要发送的字节
 * 返回: 无
 * 说明: 无
 ******************************************************************************/
/*
void USART_SendByte(u8 data)
{
     USART_SendData8(USART1,(unsigned char)data);

    // 等待传输结束
    while (USART_GetFlagStatus (USART1,USART_FLAG_TXE) == RESET);
}
*/
/*******************************************************************************
****入口参数：需要发送的字符串
****出口参数：无
****函数备注：USART发送函数
****版权信息：蓝旗嵌入式系统
*******************************************************************************/

void USART_SendStr(unsigned char *Str)
{
    while (*Str != 0) // 不为结束
    {
        USART_SendData8(USART1, *Str); // 发送数据
        while (!USART_GetFlagStatus(USART1, USART_FLAG_TXE))
            ;  // 等待发送完毕
        Str++; // 下一个数据
    }
}


/*--------------------------------------------------------------------------------------------------------
                                                       0ooo
                                                ooo0     (   )
                                                (   )     ) /
                                                 \ (     (_/
                                                  \_)
----------------------------------------------------------------------------------------------------------*/
