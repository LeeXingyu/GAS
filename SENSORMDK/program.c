#include "program.h"

extern int Rfm_Times;
INT8U Gas_check_Times = 0;
extern int  Check_flag;
extern unsigned char Gas_Pressure_state ;
extern unsigned char Bat_Check_state;
extern unsigned int BatCheck_Flag;
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
      delay_ms(20);
      //检测是否有气压   低气压 60s一次 10s进入一次 进入5次即可
  
      if(Gas_check_Times == 0)
      {
          switch(tx_ReadCount())
          {
                case GAS_BAT_LOW:Gas_check_Times = 5;Gas_Pressure_state = GAS_BAT_LOW;break;
                case GAS_BAT_HIGH:Gas_check_Times = 0;Gas_Pressure_state = GAS_BAT_HIGH;break;
                case GAS_NORMAL:Gas_check_Times = 0;Gas_Pressure_state = GAS_NORMAL; break;
                default:Gas_Pressure_state = 0;break;
          }
      }
      else 
      {
        Gas_check_Times--;
        if(Gas_check_Times <= 0)
          Gas_check_Times = 0;
      }
}

static void Bat_Check(void)
{            
      HX712_CLK_L();
      HX712_Init_Mode(ReadVoltage_Mode);
      delay_ms(10);
      //检测电池  
      switch(tx_ReadVoltage())
      {
            case GAS_BAT_LOW:Bat_Check_state = GAS_BAT_LOW;break;
            case GAS_BAT_HIGH:Bat_Check_state = GAS_BAT_HIGH;break;        
            default:Bat_Check_state = 0;break;
      }
}

//固件初始化
void HardWare_Init(void)
{
      //TEST
//      Usart_Init();//初始化串口  
//      Usart1_clear_init();//串口BUF初始化 

      TEV_GPIO_INIT();//初始化电磁阀检测和开关  
      TEV_CTRL_INIT();
      
      HX712_GPIO_INIT();//hx712&ADC GPIO_init
      HX712_CLK_H();
      delay_ms(1);

      GPIO_Initial();//SPI_GPIO_INIT       

      //RFM64
      InitSX1212();
      SetRFMode(RF_SLEEP);
      delay_ms(10); 
}

 //第一次上电对码
void FirstPower_CheckService(void)
{
    unsigned char id[FlASH_OPER_SIZE] = {0};
    memset(id,0,FlASH_OPER_SIZE);
    long int timeout = 180000;       
    //对码   
    GetMasterId(id);
    ////printf("\n id %d %d %d   \n",id[0],id[1],id[2]);
    if(id[0] != 0x24 && id[2] != 0x1C)
    {
      CheckID = 0;
      while((0 != timeout )&&(!SlaveSendSetIdResult))
      {
        Rcv_MasterDataParse();
        timeout--;
        delay_ms(1);    
      }
      Cooker_SendSetIdResult();
      CheckID = 1;
      delay_ms(10);
      ////printf("\n GetMasterId \n");
    }
    else
    {
      ////printf("\n CheckID  1 \n");
      CheckID = 1;
      //电池检测
      Bat_Check();
      Slave_Send_BatState();
      
    }

}

//开启低功耗
void LowPowerStart(void)
{ 
     //////printf("\n LowPowerStart  1 \n");
    //未接收到关闭气阀指令 关闭气阀上报
    if(Power_PreState != Power_CurState)
    {
       Power_PreState = Power_CurState;
       delay_ms(20);
       if(!READ_Level())
       {//发送关闭气阀的指令
          Cooker_Parse_t entity;
          entity.cmd	= eCOOKER_CTRL_Gas;
          entity.payload[0]	= COOKER_PARSE_FALSE;
          entity.length		= 1;
          Slave_Load(&entity);             
       }
    }
    //模块低功耗
    SetRFMode( RF_SLEEP );
    delay_ms(8);
    QA_PowerL();
    HX712_CLK_H();
    delay_ms(10);
    //关闭时钟
    Active_Halt_Colse();
    //状态位初始化
    Gas_check_Times = 1;
    Check_flag = 0;
    Gas_check_Times = 0;
    
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
    //Usart_Init();//初始化串口  
    //Usart1_clear_init();//串口BUF初始化 
    SetRFMode( RF_SLEEP ); 
    delay_ms(8);
    Check_flag = 0; 
}

void Slave_Service(void)
{
      //气体 电压检测
    if(READ_Level())
    {
        delay_ms(20);
        if(READ_Level())
        {
           Gas_Check();
           ////printf("\n Gas_Check \n");
        }
    }
    ////printf("\nBatCheck_Flag %d\n",BatCheck_Flag);
    if((21 <= BatCheck_Flag) || (Bat_Check_state == GAS_BAT_LOW))
    {
      BatCheck_Flag = 0;
      Bat_Check();
      ////printf("\n Bat_Check  %d \n",Bat_Check_state);
    }
}