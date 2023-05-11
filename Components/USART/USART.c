/******************************************************************************************
    |------------------------|
    |  TXD - PC3(UART1->RX)  |
    |  RXD - PC2(UART1->TX)  |
    |------------------------|

*******************************************************************************/

/* ����ϵͳͷ�ļ� */
#include <stdarg.h>

/* �����Զ���ͷ�ļ� */
#include "usart.h"
/* �Զ��������� */

/* �Զ���� */

uart usart1;
/* ȫ�ֱ������� */
/*******************************************************************************
 * ����: Uart_Init
 * ����: UART1��ʼ������
 * �β�: ��
 * ����: ��
 * ˵��: ��
 ******************************************************************************/
void Usart_Init(void)
{
    /*
    ��ʼ��IO
    TxD  ���ó������������ģʽ
    RxD  ���ó������������ж�ģʽ
      */
    CLK_PeripheralClockConfig(CLK_Peripheral_USART1, ENABLE); // ����USARTʱ��

    USART_DeInit(USART1); // ���½�USART1����Ϊȱʡֵ��USART1

    GPIO_Init(USART_GPIO_PORT, TXD_GPIO_PINS, GPIO_Mode_Out_PP_High_Fast); // ��ʼ�����������������
    GPIO_Init(USART_GPIO_PORT, RXD_GPIO_PINS, GPIO_Mode_In_PU_No_IT);      // ��ʼ�������������룬�����ж�
    //       GPIO_Init(USART_GPIO_PORT, CS_GPIO_PINS, GPIO_Mode_Out_PP_High_Slow);

    /*
        SYSCFG_REMAPDeInit();//���Ÿ��ù���
            SYSCFG_REMAPPinConfig(REMAP_Pin_USART1TxRxPortC,ENABLE);//��USART Tx--Rx����ӳ�䵽PC5-PC6
      */

    /*
     * ��UART1����Ϊ��
     * ������ = 115200
     * ����λ = 8
     * 1λֹͣλ
     * ��У��λ
     * ʹ�ܽ��պͷ���
     * ʹ�ܽ����ж�
     */

    USART_Init(USART1, (uint32_t)115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, USART_Mode_Tx | USART_Mode_Rx); // ����USART����115200��8N1������/����

    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    USART_ITConfig(USART1, USART_IT_OR, ENABLE); // ʹ�ܽ����ж�,�ж�������Ϊ28
    // USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    // USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
    //  USART_ITConfig (USART1,USART_IT_TC,ENABLE);//ʹ�ܽ����ж�,�ж�������Ϊ28
    USART_Cmd(USART1, ENABLE); // ʹ��USART
}
// ��ʼ������
void Usart1_clear_init(void)
{
    memset(usart1.rx_data, 0, sizeof(usart1.rx_data));
    memset(usart1.tx_data, 0, sizeof(usart1.tx_data));
}
/*******************************************************************************
 * ����: UART1_SendByte
 * ����: UART1����һ���ֽ�
 * �β�: data -> Ҫ���͵��ֽ�
 * ����: ��
 * ˵��: ��
 ******************************************************************************/
/*
void USART_SendByte(u8 data)
{
     USART_SendData8(USART1,(unsigned char)data);

    // �ȴ��������
    while (USART_GetFlagStatus (USART1,USART_FLAG_TXE) == RESET);
}
*/
/*******************************************************************************
****��ڲ�������Ҫ���͵��ַ���
****���ڲ�������
****������ע��USART���ͺ���
****��Ȩ��Ϣ������Ƕ��ʽϵͳ
*******************************************************************************/

void USART_SendStr(unsigned char *Str)
{
    while (*Str != 0) // ��Ϊ����
    {
        USART_SendData8(USART1, *Str); // ��������
        while (!USART_GetFlagStatus(USART1, USART_FLAG_TXE))
            ;  // �ȴ��������
        Str++; // ��һ������
    }
}


/*--------------------------------------------------------------------------------------------------------
                                                       0ooo
                                                ooo0     (   )
                                                (   )     ) /
                                                 \ (     (_/
                                                  \_)
----------------------------------------------------------------------------------------------------------*/
