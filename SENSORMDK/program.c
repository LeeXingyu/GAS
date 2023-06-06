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
//����ģʽ ����ʵ��
static void Gas_Check(void)
{            
      // ������
      HX712_CLK_L();
      HX712_Init_Mode(ReadCount_Mode);
      delay_ms(20);
      //����Ƿ�����ѹ   ����ѹ 60sһ�� 10s����һ�� ����5�μ���
  
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
      //�����  
      switch(tx_ReadVoltage())
      {
            case GAS_BAT_LOW:Bat_Check_state = GAS_BAT_LOW;break;
            case GAS_BAT_HIGH:Bat_Check_state = GAS_BAT_HIGH;break;        
            default:Bat_Check_state = 0;break;
      }
}

//�̼���ʼ��
void HardWare_Init(void)
{
      //TEST
//      Usart_Init();//��ʼ������  
//      Usart1_clear_init();//����BUF��ʼ�� 

      TEV_GPIO_INIT();//��ʼ����ŷ����Ϳ���  
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

 //��һ���ϵ����
void FirstPower_CheckService(void)
{
    unsigned char id[FlASH_OPER_SIZE] = {0};
    memset(id,0,FlASH_OPER_SIZE);
    long int timeout = 180000;       
    //����   
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
      //��ؼ��
      Bat_Check();
      Slave_Send_BatState();
      
    }

}

//�����͹���
void LowPowerStart(void)
{ 
     //////printf("\n LowPowerStart  1 \n");
    //δ���յ��ر�����ָ�� �ر������ϱ�
    if(Power_PreState != Power_CurState)
    {
       Power_PreState = Power_CurState;
       delay_ms(20);
       if(!READ_Level())
       {//���͹ر�������ָ��
          Cooker_Parse_t entity;
          entity.cmd	= eCOOKER_CTRL_Gas;
          entity.payload[0]	= COOKER_PARSE_FALSE;
          entity.length		= 1;
          Slave_Load(&entity);             
       }
    }
    //ģ��͹���
    SetRFMode( RF_SLEEP );
    delay_ms(8);
    QA_PowerL();
    HX712_CLK_H();
    delay_ms(10);
    //�ر�ʱ��
    Active_Halt_Colse();
    //״̬λ��ʼ��
    Gas_check_Times = 1;
    Check_flag = 0;
    Gas_check_Times = 0;
    
    enableInterrupts(); //�������ж�  
    PWR_UltraLowPowerCmd(ENABLE);//���͹���
    halt(); 
}

//�˳��͹���
void LowPowerStop(void)
{
    //����ʱ��
    Active_Halt_Open();
    //��ʼ����ʱ�� 
    TIM3_Init();  
    //Usart_Init();//��ʼ������  
    //Usart1_clear_init();//����BUF��ʼ�� 
    SetRFMode( RF_SLEEP ); 
    delay_ms(8);
    Check_flag = 0; 
}

void Slave_Service(void)
{
      //���� ��ѹ���
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