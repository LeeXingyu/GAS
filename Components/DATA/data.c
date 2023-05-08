
/* ����ϵͳͷ�ļ� */
#include <stdarg.h>

/* �����Զ���ͷ�ļ� */
#include "data.h"
/* �Զ��������� */

/* �Զ���� */
data_buf data;
     
/* ȫ�ֱ������� */

static int TimeOut = 100;

//SX1212�������ݴ���
void recv_sx1212_data(void)
{
        INT8U txBuffer[5] = {0x43,0x01,0x01,0x04,0x01};
        SX1212_EnterReceiveMode(  );
        while( !SX1212_READ_IRQ_1( ))
        {
          TimeOut--;
          delay_us();
          if(!TimeOut) goto exit;
        }
        data.rx_len = SX1212_ReceivePacket(data.rx_data);
        if(data.rx_len <= 5)
        {
          if((data.rx_data[0] == 0x43) && (data.rx_data[2] == 1))
         {
           if(data.rx_data[3] == 0x04)//��ȡ��ŷ��ر�
           {
            QA_PowerH();//�رյ�ŷ�
            SX1212_SendPacket_Var(txBuffer,5);
           }
         }
       }
       else                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
       {
         data.rx_len = 0;
       }
exit:
      data.rx_len = 0;
}


/*--------------------------------------------------------------------------------------------------------
                   									   0ooo
                   								ooo0     (   )
                								(   )     ) /
                								 \ (     (_/
                								  \_)
----------------------------------------------------------------------------------------------------------*/
