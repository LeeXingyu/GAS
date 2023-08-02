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
//����ģʽ ����ʵ��
static void Gas_Check(void)
{    
    // ������
    HX712_CLK_L();
    HX712_Init_Mode(ReadCount_Mode);
    delay_ms(30);
    //����Ƿ�����ѹ ����ѹ 60sһ�� 10s����һ�� ����5�μ���
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
        //�����  
        switch(tx_ReadVoltage())
        {
              case BAT_LOW:Bat_Check_state =  BAT_LOW;break;
              case BAT_HIGH:Bat_Check_state = BAT_HIGH;break;        
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
    long int timeout = 108;  
    SlaveSendSetIdResult = 0;

     //1�����ڽ��ж���   
    CheckID = 0;
    //�ӽ�0.56sһ��ѭ��  ��ʱ��Ҫ����
    while((0 != timeout )&&(!SlaveSendSetIdResult))
    {
      Rcv_MasterDataParse();
      //ͨ���ڲ��Դ�400��ms�ĳ�ʱ�ж�
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
    //��ʼ����ʱ�� 
    //TIM3_Init();  
    //Usart_Init();//��ʼ������  
    //Usart1_clear_init();//����BUF��ʼ�� 
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
    //��������
    if(Gas_Pressure_state == GAS_HIGH)
    {
      delay_ms(100);
      Slave_Send_GasState();
    }
    //���͵��
    if(Bat_Check_state == BAT_LOW)
    {
      delay_ms(200);
      Slave_Send_BatState();
    }
}