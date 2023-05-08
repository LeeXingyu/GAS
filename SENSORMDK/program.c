#include "program.h"

INT8U  Check_flag = 0;
extern INT8U Gas_check_Times;


void HardWare_Init(void)
{
      //TEST
      Usart_Init();//初始化串口  
      Usart1_clear_init();//串口BUF初始化  
      TEV_GPIO_INIT();//初始化电磁阀检测和开关  
      HX712_GPIO_INIT();//hx712&ADC GPIO_init
     
      TIM3_Init();
      SPI_Initial();
      GPIO_Initial();//SPI_GPIO_INIT
  
      Air_detection_Init();//ADC初始化        
      //复位CSx1212 
      SX1212_Init( );
      SX1212_EnterReceiveMode(  );//接收使能
      SX1212_SetMode( MODE_SLEEP );
        
}

void SX1212_SEND(void)
{
        HardWare_Init();//系统初始化
        TEV_STATUS();//检测气压       
        //ReadVoltageValue();//读取电源电压          
        tx_ReadCount();
}

//低功耗情况下,关闭hx711,设置sleep模式
void LowPowerConsumption_Cmd(void)
{    
    HX712_CLK_H();
    delay_ms(1);    
    //状态位初始化
    Gas_check_Times = 1;
    Check_flag = 0;

    halt(); 
    //改变系统时钟频率
    CLK_Config(CLK_SYSCLKSource_HSI);
}
//待机模式下 固定功能检测
void StandyFun(void)
{
  if(Check_flag)    
  {   
    //10s 通信检测
    SX1212_EnterReceiveMode();    
    //Hx712 初始状态设定
    HX712_CLK_L();
    Read_Init_Mode(ReadVoltage_Mode);
    delay_ms(10);  
    //待测试  读取电池电压并发送
    tx_ReadVoltage();
    
    //接受主机发送关闭阀门的指令
    recv_sx1212_data();
  }
}

void Gas_CheckFun(void)
{
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
    } 
    
  SX1212_SetMode( MODE_SLEEP );
  HX712_CLK_H();
  
}