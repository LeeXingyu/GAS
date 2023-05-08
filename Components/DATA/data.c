
/* 包含系统头文件 */
#include <stdarg.h>

/* 包含自定义头文件 */
#include "data.h"
/* 自定义新类型 */

/* 自定义宏 */
data_buf data;
     
/* 全局变量定义 */

static int TimeOut = 100;

//SX1212接收数据处理
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
           if(data.rx_data[3] == 0x04)//读取电磁阀关闭
           {
            QA_PowerH();//关闭电磁阀
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
