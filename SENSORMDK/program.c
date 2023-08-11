#include "program.h"

extern int Sleep_times;
INT8U Gas_check_Times = 0;
unsigned char  Gas_check_flag = 1;
extern int Bat_Checktimes;
extern unsigned char Gas_Pressure_state ;
extern unsigned char Bat_Check_state;
unsigned char Bat_Check_Prestate = 0;
unsigned char Gas_Check_Prestate = 0;

extern unsigned short Power_PreState;
extern unsigned short  Power_CurState;
extern unsigned short SlaveGasCTRL ;
extern unsigned short SlaveSendSetIdResult;
extern unsigned char  CheckID;
extern unsigned char GasValue_Charge;
//����ģʽ ����ʵ��
static void Gas_Check(void)
{    
    // ������
    HX712_CLK_L();
    delay_ms(10);
    HX712_Init_Mode(ReadCount_Mode);
    delay_ms(50);
    //����Ƿ�����ѹ ����ѹ 60sһ�� 5s����һ�� ����12�μ���
    switch(tx_ReadCount())
    {
          case GAS_LOW:Sleep_times = 12;Gas_Pressure_state = GAS_LOW;Gas_check_flag = 0;break;
          case GAS_HIGH:Sleep_times = 2;Gas_Pressure_state = GAS_HIGH;Gas_check_flag = 0;break;
          case GAS_NORMAL:Sleep_times = 2;Gas_Pressure_state = GAS_NORMAL;Gas_check_flag = 0; break;
          default:Sleep_times = 2;Gas_Pressure_state = 0;break;
    }
}

static void Bat_Check(void)
{          
        HX712_CLK_L();
        delay_ms(10);
        HX712_Init_Mode(ReadVoltage_Mode);
        delay_ms(50);
        //�����  
        switch(tx_ReadVoltage())
        {
              case BAT_LOW:Bat_Check_state =  BAT_LOW;SlaveGasCTRL = 1;GasValue_Charge = 1;break;
              case BAT_NORMAL:Bat_Check_state =  BAT_LOW;GasValue_Charge = 0;break;
              case BAT_HIGH:Bat_Check_state = BAT_HIGH;GasValue_Charge = 0;break;        
              default:Bat_Check_state = 0;break;
        }
}


//�̼���ʼ��
void HardWare_Init(void)
{
      //TEST
 //     Usart_Init();//��ʼ������  
 //     Usart1_clear_init();//����BUF��ʼ�� 

     TEV_GPIO_INIT();//��ʼ����ŷ����Ϳ���  
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

 //��һ���ϵ����
void FirstPower_CheckService(void)
{
    unsigned char id[FlASH_OPER_SIZE] = {0};
    memset(id,0,FlASH_OPER_SIZE);
    long int timeout = 800;  
    unsigned char Ack_times = 5;
    long int Acktime ;
    SlaveSendSetIdResult = 0;

     //1�����ڽ��ж���   
    CheckID = 0;
    //�ӽ�0.075sһ��ѭ��  ��ʱ��Ҫ����
    while((0 != timeout )&&(!SlaveSendSetIdResult))
    {
      Rcv_MasterDataParse();
      //ͨ���ڲ��Դ�75��ms�ĳ�ʱ�ж�
      timeout--;
      delay_ms(1);  
    }
    if(SlaveSendSetIdResult)
    {
      Cooker_SendSetIdResult();
    }
    SlaveSendSetIdResult = 1;
    CheckID = 1;
    delay_ms(50);
    

    Bat_Check();
    Bat_Check_Prestate = Bat_Check_state;
    delay_ms(20);
    
send_data:
    Acktime = 1000;
    Slave_Send_BatState();
    do
    {       
        Rcv_MasterAck();
        Acktime--;
        delay_ms(1);
    }while(Master_Ack.Bat_Ack == 0 && (Acktime != 0));  
    if((Acktime | Master_Ack.Bat_Ack) == 0)
    {
      Ack_times--;
      if(Ack_times > 0)
      {
          goto send_data;
      }
      else
      {
        Bat_Check_Prestate = 0;
      }
    }
    else Master_Ack.Bat_Ack = 0;

}

//�����͹���
void LowPowerStart(void)
{ 
    //ģ��͹���
    SetRFMode( RF_SLEEP );
    delay_ms(8);
    QA_PowerL();
    HX712_CLK_H();
    delay_ms(10);
    //�ر�ʱ��
    Active_Halt_Colse();
    //״̬λ��ʼ��
    Gas_check_Times = 0;
  
}

//�˳��͹���
void LowPowerStop(void)
{
    //����ʱ��
    Active_Halt_Open();
    SetRFMode( RF_SLEEP ); 
    delay_ms(8); 
    Gas_check_Times = 0;
}

void Slave_Service(void)
{
        Gas_Check();  
        delay_ms(20);
        if(Bat_Checktimes == 0)
        {
                Bat_Checktimes = 6900;
                Bat_Check();
                delay_ms(20);
        }
        HX712_CLK_H();
        delay_ms(20);
}

void Slave_GasBat_SendService(void)
{
    int send_time;
    unsigned char gas_acktimes = 5;
    unsigned char bat_acktimes = 5;
    //��������
    if(GAS_HIGH == Gas_Pressure_state )
    {
      Gas_Check_Prestate = Gas_Pressure_state;
      delay_ms(20);
          
      send_gasdata:
          send_time = 500;
          Slave_Send_GasState();
          do
          {       
              Rcv_MasterAck();
              send_time--;
              delay_ms(1);
          }while(Master_Ack.Gas_Ack == 0 && (send_time != 0));  
          if((send_time | Master_Ack.Gas_Ack) == 0)
          {
            gas_acktimes--;
            if(gas_acktimes > 0)
            {
                goto send_gasdata;
            }
            else
            {
              Gas_Check_Prestate = 0;
            }
          }
          else Master_Ack.Gas_Ack = 0;    
    }
    
    //���͵��
    if(Bat_Check_Prestate != Bat_Check_state)
    {
      Bat_Check_Prestate = Bat_Check_state;
      delay_ms(20);
      
 send_batdata:
          send_time = 500;
          Slave_Send_BatState();
          do
          {       
              Rcv_MasterAck();
              send_time--;
              delay_ms(1);
          }while(Master_Ack.Bat_Ack == 0 && (send_time != 0));  
          if((send_time | Master_Ack.Bat_Ack) == 0)
          {
            bat_acktimes--;
            if(bat_acktimes > 0)
            {
                goto send_batdata;
            }
            else
            {
              Bat_Check_Prestate = 0;
            }
          }
          else Master_Ack.Bat_Ack = 0;    
    }
}

void Shutdown_GasSend(void)
{
      Gas_Pressure_state = 0x55;
      Gas_Check_Prestate = Gas_Pressure_state;
      delay_ms(3);
}
