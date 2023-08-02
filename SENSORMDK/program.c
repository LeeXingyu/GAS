#include "program.h"

extern int Rfm_Times;
INT8U Gas_check_Times = 0;
unsigned char  Gas_check_flag = 1;

extern unsigned char Gas_Pressure_state ;
extern unsigned char Bat_Check_state;

extern unsigned short Power_PreState;
extern unsigned short  Power_CurState;

extern unsigned short SlaveSendSetIdResult;
extern unsigned char  CheckID;
//待机模式 功能实现
static void Gas_Check(void)
{    
    // 气体检测
    HX712_CLK_L();
    HX712_Init_Mode(ReadCount_Mode);
    delay_ms(30);
    //检测是否有气压 低气压 60s一次 10s进入一次 进入5次即可
    switch(tx_ReadCount())
    {
          case GAS_LOW:Gas_check_Times = 5;Gas_Pressure_state = GAS_LOW;Gas_check_flag = 0;break;
          case GAS_HIGH:Gas_check_Times = 0;Gas_Pressure_state = GAS_HIGH;Gas_check_flag = 0;break;
          case GAS_NORMAL:Gas_check_Times = 0;Gas_Pressure_state = GAS_HIGH;Gas_check_flag = 0; break;
          default:Gas_Pressure_state = 0;break;
    }
}

static void Bat_Check(void)
{          
        HX712_CLK_L();
        HX712_Init_Mode(ReadVoltage_Mode);
        delay_ms(100);
        //检测电池  
        switch(tx_ReadVoltage())
        {
              case BAT_LOW:Bat_Check_state =  BAT_LOW;break;
              case BAT_HIGH:Bat_Check_state = BAT_HIGH;break;        
              default:Bat_Check_state = 0;break;
        }
}



//固件初始化
void HardWare_Init(void)
{
      //TEST
 //     Usart_Init();//初始化串口  
 //     Usart1_clear_init();//串口BUF初始化 

     TEV_GPIO_INIT();//初始化电磁阀检测和开关  
     TEV_CTRL_INIT();
      
     HX712_GPIO_INIT();//hx712&ADC GPIO_init
     HX712_CLK_H();
     delay_ms(1);

     GPIO_Initial();//SPI_GPIO_INIT       

      //RFM64
     InitSX1212();
     SetRFMode(RF_SLEEP);
     delay_ms(20); 
}

 //第一次上电对码
void FirstPower_CheckService(void)
{
    unsigned char id[FlASH_OPER_SIZE] = {0};
    memset(id,0,FlASH_OPER_SIZE);
    long int timeout = 108;  
    SlaveSendSetIdResult = 0;

     //1分钟内进行对码   
    CheckID = 0;
    //接近0.56s一个循环  定时需要计算
    while((0 != timeout )&&(!SlaveSendSetIdResult))
    {
      Rcv_MasterDataParse();
      //通信内部自带400多ms的超时判定
      timeout--;
      delay_ms(1);  
    }
    if(SlaveSendSetIdResult)
    {
      Cooker_SendSetIdResult();
    }
    SlaveSendSetIdResult = 1;
    CheckID = 1;
    delay_ms(10);
}

//开启低功耗
void LowPowerStart(void)
{ 

    //模块低功耗
    SetRFMode( RF_SLEEP );
    delay_ms(8);
    QA_PowerL();
    HX712_CLK_H();
    delay_ms(10);
    //关闭时钟
    Active_Halt_Colse();
    //状态位初始化
    Gas_check_Times = 0;
   
}

//退出低功耗
void LowPowerStop(void)
{
    //开启时钟
    Active_Halt_Open();
    //初始化定时器 
    //TIM3_Init();  
    //Usart_Init();//初始化串口  
    //Usart1_clear_init();//串口BUF初始化 
    SetRFMode( RF_SLEEP ); 
    delay_ms(8); 
    Gas_check_Times = 0;
}

void Slave_Service(void)
{
    if(Gas_check_Times == 0)
    {
        Gas_Check();  
        delay_ms(100);
        Bat_Check();
        delay_ms(20);
    }
    else 
    {
      Gas_check_flag = 1;
      Gas_check_Times--;
      if(Gas_check_Times <= 0)
        Gas_check_Times = 0;
    }
}

void Slave_GasBat_SendService(void)
{
    //发送气体
    if(Gas_Pressure_state == GAS_HIGH)
    {
      delay_ms(100);
      Slave_Send_GasState();
    }
    //发送电池
    if(Bat_Check_state == BAT_LOW)
    {
      delay_ms(200);
      Slave_Send_BatState();
    }
}