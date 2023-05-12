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
     
      TIM3_Init();
      
      HX712_CLK_H();
      delay_ms(1);
      QA_PowerL();
    //状态位初始化
        
}

void SX1212_SEND(void)
{
        HardWare_Init();//系统初始化
        TEV_STATUS();//检测气压       
        //ReadVoltageValue();//读取电源电压          
        tx_ReadCount();
}

//开启低功耗
void LowPowerStart(void)
{    
    Active_Halt_Colse();
    Gas_check_Times = 1;
    Check_flag = 0;

    halt(); 
}
//退出低功耗
void LowPowerStop(void)
{
    QA_PowerH();

    //开启时钟
    Active_Halt_Open();
    //初始化定时器 sx1212 adc
    TIM3_Init();
//      SPI_Initial();
//      GPIO_Initial();//SPI_GPIO_INIT
//  
      Air_detection_Init();//ADC初始化        
//      复位CSx1212 
//      SX1212_Init( );
//      SX1212_EnterReceiveMode(  );//接收使能
//      SX1212_SetMode( MODE_SLEEP );//转为sleep模式
    
    delay_ms(10);
    //SX1212_SetMode( MODE_SLEEP );
    HX712_CLK_H();
    QA_PowerH();//外接指示灯进行判断
    delay_ms(50);
    QA_PowerL();
    delay_ms(50);
}

//待机模式下 电量与通讯接收
void StandyFun(void)
{
  if(Check_flag)    
  {  
     CLK_PeripheralClockConfig(CLK_Peripheral_TIM3,DISABLE);
     Rfm_Times = 40;
     Check_flag = 0;
     QA_PowerL();
     delay_ms(100);
    //10s 通信检测
    //SX1212_EnterReceiveMode();    
    //Hx712 初始状态设定
    HX712_CLK_L();
    Read_Init_Mode(ReadVoltage_Mode);
    delay_ms(10);  
    //待测试  读取电池电压并发送
    tx_ReadVoltage();
    
    //接受主机发送关闭阀门的指令
   // recv_sx1212_data();
  }
}
//待机状态下气体检测设定
void Gas_CheckFun(void)
{
    char i = 0;
    if(!Gas_check_Times)
    {

      //检测是否有气压
      if(Air_detection())
      {
        for(i = 0;i<50;i++)
        {
            QA_PowerH();//外接指示灯进行判断
            delay_ms(50);
            QA_PowerL();
            delay_ms(10);
        }
        //有气压 10s检测一次 并上传
        Gas_check_Times = 1;
        tx_ReadCount();
      }
      else
      {
        //无气压 60s检测一次
        Gas_check_Times = 6; 
        QA_PowerH();//外接指示灯进行判断
        delay_ms(50);
        QA_PowerL();
        delay_ms(50);
      }
    }
  
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM3,ENABLE);  
  //SX1212_SetMode( MODE_SLEEP );
  HX712_CLK_H(); 
}