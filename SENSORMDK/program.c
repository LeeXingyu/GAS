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
      GPIO_Initial();//SPI_GPIO_INIT       

      //RFM64
      InitSX1212();
      SetRFMode(RF_SLEEP);
      delay_ms(10);        
}

//开启低功耗
void LowPowerStart(void)
{  
    //模块低功耗
    SetRFMode( RF_SLEEP );
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
    //初始化定时器 
    TIM3_Init();  
    //Air_detection_Init();//ADC初始化      
    SetRFMode( RF_SLEEP );//转为sleep模式   
    delay_ms(8);
}

//待机模式 功能实现
void StandyFun(void)
{    
     Rfm_Times = 40;  //定时器10s计数状态位
     Check_flag = 0;
         
    // 气体检测
     if(!Gas_check_Times)
    {
      //Hx712 初始状态设定
      HX712_CLK_L();
      HX712_Init_Mode(ReadCount_Mode);
      delay_ms(10);
      //检测是否有气压      
      if(tx_ReadCount() != GAS_BAT_LOW)
      {
        //有气压 10s检测一次 并上传
        Gas_check_Times = 1;       
      }
      else
      {
        //无气压 60s检测一次
        Gas_check_Times = 6; 
      }
      tx_ReadVoltage();
    }

}
