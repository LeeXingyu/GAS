#include "program.h"

INT8U  Check_flag = 0;
extern INT8U Rfm_Times;
extern INT8U Gas_check_Times ;

//固件初始化
void HardWare_Init(void)
{
      //TEST
 //     Usart_Init();//初始化串口  
 //     Usart1_clear_init();//串口BUF初始化 

      TEV_GPIO_INIT();//初始化电磁阀检测和开关  
      TEV_CTRL_INIT();
//      
      HX712_GPIO_INIT();//hx712&ADC GPIO_init
      HX712_CLK_H();
      delay_ms(1);
//
//      SPI_Initial();
      GPIO_Initial();//SPI_GPIO_INIT       
//    复位CSx1212 
//      SX1212_Init( );
//      SX1212_EnterReceiveMode(  );//接收使能
//      SX1212_SetMode( MODE_SLEEP );//转为sleep模式
      //RFM64
      InitSX1212();
      SetRFMode(RF_SLEEP);
      delay_ms(10);        
}

//开启低功耗
void LowPowerStart(void)
{  
    //模块低功耗
    SetRFMode( MODE_SLEEP );
    QA_PowerL();
    HX712_CLK_H();
    //关闭时钟
    Active_Halt_Colse();
    //状态位初始化
    Gas_check_Times = 1;
    Check_flag = 0;
    
    enableInterrupts(); //开启总中断  
    PWR_UltraLowPowerCmd(ENABLE);//超低功耗
    halt(); 
}

//退出低功耗
void LowPowerStop(void)
{
    //开启时钟
    Active_Halt_Open();
 
    //初始化定时器 sx1212 adc
    TIM3_Init();
//  
    Air_detection_Init();//ADC初始化      
//    
//    SPI_Initial();
//    GPIO_Initial();//SPI_GPIO_INIT   
////  复位CSx1212 
//    SX1212_Init( );
    
//   SX1212_EnterReceiveMode(  );//接收使能
    SetRFMode( MODE_SLEEP );//转为sleep模式   
    delay_ms(8);
}

//待机模式 功能实现
void StandyFun(void)
{    
     Rfm_Times = 40;  //定时器10s计数状态位
     Check_flag = 0;

     //10s 通信检测
    //SX1212_EnterReceiveMode();  
     
    //Hx712 初始状态设定
    HX712_CLK_L();
    HX712_Init_Mode(ReadCount_Mode);
    delay_ms(10);  
    //读取电池电压并发送
    tx_ReadCount();
    
    //接受主机发送关闭阀门的指令
   // recv_sx1212_data();
    
    // 气体检测
     if(!Gas_check_Times)
    {
      //检测是否有气压
      if(Air_detection())
      {
        //有气压 10s检测一次 并上传
        Gas_check_Times = 1;
        tx_ReadCount();
      }
      else
      {
        //无气压 60s检测一次
        Gas_check_Times = 6; 
      }
      tx_ReadVoltage();
    }

}
