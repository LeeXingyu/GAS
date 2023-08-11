#include "Extdata.h"
#include <stdlib.h>

extern unsigned short Power_PreState;
extern unsigned short  Power_CurState;
unsigned short SlaveSendSetIdResult = 0;
unsigned short SlaveGasCTRL = 0;
static unsigned char Slave_BackupId[FlASH_OPER_SIZE];

struct Sx1212_Ack Master_Ack = {0,0,0};

void FLASH_WriteNByte(uint8_t* pBuffer, uint32_t WriteAddr, uint8_t nByte)
{
    u8 i=0;  
    for(i = 12;i < EEPROMPAGESIZE;i++)
    {
      pBuffer[i] = (uint8_t) (rand() % 90 +32);
    }
    i = 0;
    if((WriteAddr >= EEP_ADDRESS_START) && (WriteAddr <= EEP_ADDRESS_END))
    {  
        FLASH_SetProgrammingTime(FLASH_ProgramTime_Standard);
        FLASH_Unlock(FLASH_MemType_Data);
        while(FLASH_GetFlagStatus(FLASH_FLAG_DUL) == RESET);
         for(i = 0;i < EEPROMPAGESIZE; i++)	
        {
            FLASH_ProgramByte(WriteAddr, (pBuffer[i]) + 0x05);            
            FLASH_WaitForLastOperation(FLASH_MemType_Data);
            WriteAddr++;   
        }
        FLASH_Lock(FLASH_MemType_Data);                //����
    }
}

void FLASH_ReadNByte(unsigned char* pBuffer, uint32_t ReadAddr, uint8_t nByte)
{
  u8 j=0;
   FLASH_Unlock(FLASH_MemType_Data);
  for(j = 0;j < nByte; j++)
  {
    pBuffer[j]= (FLASH_ReadByte(ReadAddr) - 0x05);
    ReadAddr++;
  }
  FLASH_Lock(FLASH_MemType_Data);                //����
}

void Cooker_AFN_Handle(Cooker_Parse_t *entity)
{
    switch ((Cooker_Cmd_e)entity->cmd)
    {
        case eCOOKER_SET_SYS_ID:
        {
            if (0 == entity->length)
            {
                if(SlaveSendSetIdResult != 1)
                {
                  //printf("write id\n");
                  SlaveSendSetIdResult = 1;
                  FLASH_WriteNByte((unsigned char *)entity->addr,PARA_START_INDEX,FlASH_OPER_SIZE);
                  memcpy((char *)Slave_BackupId, (char *)entity->addr, COOKER_PARSE_ADDR_LEN);
                }
            }
        }
        break;
        
        case eCOOKER_FIRE_STATE:    //��ѹ״̬
        {
            // if (1 == entity->length)
            //	bSend_FireState = TRUE;
        }
        break;
        
        case eCOOKER_CTRL_Gas:
        {
            if ((1 == entity->length) && (COOKER_PARSE_FALSE == entity->payload[0]))
            {
              SlaveGasCTRL = 1;
            }
            else if((1 == entity->length) && (COOKER_PARSE_OK == entity->payload[0]))
            {
              Master_Ack.CtrlGas_Ack = 1;
            }
        }
        break;
        
        case eCOOKER_STATE_Gas:
        {
        if((1 == entity->length) && (COOKER_PARSE_OK == entity->payload[0]))
        {
          Master_Ack.Gas_Ack = 1;
        }
      
      }
        break;
      
        case eCOOKER_STATE_Bat:
        {
          if((1 == entity->length) && (COOKER_PARSE_OK == entity->payload[0]))
          {
            Master_Ack.Bat_Ack = 1;
          }   
        }
         break;
          
         default:break; //SlaveGasCTRL = 1;
             
    }
}

void Cooker_SendSetIdResult(void)
{
    unsigned char id_check[FlASH_OPER_SIZE];
    if (1 == SlaveSendSetIdResult)
    {        
          GetMasterId(id_check);
          if  ((id_check[COOKER_PARSE_ADDR_LEN - 1] == Slave_BackupId[COOKER_PARSE_ADDR_LEN - 1])&& (cmp_buf((char *)id_check,(char *)Slave_BackupId, COOKER_PARSE_ADDR_LEN)))
          {
                Cooker_Parse_t entity;

                entity.cmd	= eCOOKER_SET_SYS_ID;
                entity.length	= 0;                                                                                                                                                                                                            

                Slave_Load(&entity);
          }
    }
}
void Cooker_SendGas_CTRL(void)
{
    if(READ_Level())
    {
      //�ر�����
      QA_PowerH();//�رյ�ŷ�
      delay_ms(300);
      QA_PowerL();//�رյ�ŷ�
      delay_ms(100);
    }
    if(!READ_Level())
    {
       delay_ms(20);
       if(!READ_Level())
       {//���͹ر�������ָ��
        Slave_Send_GasCtrl(COOKER_PARSE_FALSE);
        Power_CurState = READ_Level();
        Power_PreState = READ_Level();
       }
    }
    
    //printf("\n Cooker_SendGas_CTRL \n");
}

