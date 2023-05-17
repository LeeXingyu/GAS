
/* 包含系统头文件 */
#include <stdarg.h>

/* 包含自定义头文件 */
#include "data.h"
/* 自定义新类型 */

/* 自定义宏 */
data_buf data;
     
/* 全局变量定义 */

static int TimeOut = 20000;

//SX1212接收数据处理
INT8U recv_sx1212_data(void)
{
        INT8U txBuffer[5] = {0x43,0x01,0x01,0x04,0x01};
       // SX1212_SendPacket_Var(txBuffer,5);
        char revalue;
        SX1212_EnterReceiveMode(  );
         
        while( !SX1212_READ_IRQ_1( ))
        {
//          TimeOut--;
//          delay_us();
//          if(!TimeOut) 
//          {
//            USART_SendStr("recv sx1212 timeout\n");
//            revalue =  0;
//          }
        };
        data.rx_len = SX1212_ReceivePacket(data.rx_data);
        if(data.rx_len <= 5)
        {
          if((data.rx_data[0] == 0x43) && (data.rx_data[2] == 1))
         {
           if(data.rx_data[3] == 0x04)//读取电磁阀关闭
           {
            QA_PowerH();//关闭电磁阀
            SX1212_SendPacket_Var(txBuffer,5);
            revalue =  1;
           }
         }
         else 
         {
           USART_SendStr("recv sx1212 data_head error\n");
           revalue =  0;
         }
       }
       else                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
       {
         data.rx_len = 0;
         USART_SendStr("recv sx1212 data error\n");
         revalue =  0;
       } 
       return revalue;
}


/*--------------------------------------------------------------------------------------------------------
                   									   0ooo
                   								ooo0     (   )
                								(   )     ) /
                								 \ (     (_/
                								  \_)
----------------------------------------------------------------------------------------------------------*/
