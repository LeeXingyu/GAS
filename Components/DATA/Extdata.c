#include "Extdata.h"

extern unsigned short Power_PreState;
extern unsigned short  Power_CurState;
unsigned short SlaveSendSetIdResult = 0;
unsigned short SlaveGasCTRL = 0;
static unsigned char Slave_BackupId[FlASH_OPER_SIZE];

void FLASH_WriteNByte(uint8_t* pBuffer, uint32_t WriteAddr, uint8_t nByte)
{
    u8 i=0;  
    if((WriteAddr >= EEP_ADDRESS_START) && (WriteAddr <= EEP_ADDRESS_END))
    {  
        FLASH_SetProgrammingTime(FLASH_ProgramTime_Standard);
        FLASH_Unlock(FLASH_MemType_Data);
        while(FLASH_GetFlagStatus(FLASH_FLAG_DUL) == RESET);
         for(i = 0;i < EEPROMPAGESIZE; i++)	
        {
            FLASH_ProgramByte(WriteAddr, pBuffer[i]);            
            FLASH_WaitForLastOperation(FLASH_MemType_Data);
            WriteAddr++;   
        }
        FLASH_Lock(FLASH_MemType_Data);                //上锁
    }
}

void FLASH_ReadNByte(unsigned char* pBuffer, uint32_t ReadAddr, uint8_t nByte)
{
  u8 j=0;
   FLASH_Unlock(FLASH_MemType_Data);
  for(j = 0;j < EEPROMPAGESIZE; j++)
  {
    pBuffer[j]= FLASH_ReadByte(ReadAddr);
    ReadAddr++;
  }
  FLASH_Lock(FLASH_MemType_Data);                //上锁
}

void Cooker_AFN_Handle(Cooker_Parse_t *entity)
{
    switch ((Cooker_Cmd_e)entity->cmd)
    {
        case eCOOKER_SET_SYS_ID:
        {
            if (0 == entity->length)
            {
                SlaveSendSetIdResult   = 1;
                printf("write id\n");
                FLASH_WriteNByte((unsigned char *)entity->addr,PARA_START_INDEX,FlASH_OPER_SIZE);
                memcpy((char *)Slave_BackupId, (char *)entity->addr, COOKER_PARSE_ADDR_LEN);              
            }
        }
        break;
        case eCOOKER_FIRE_STATE:    //气压状态
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
        }
        break;

        default:SlaveGasCTRL = 1;
            break;
    }
}

void Cooker_SendSetIdResult(void)
{
    unsigned char id_check[FlASH_OPER_SIZE];
    if (1 == SlaveSendSetIdResult)
    {
          SlaveSendSetIdResult   = 0;
          GetMasterId(id_check);
          if  ((id_check[COOKER_PARSE_ADDR_LEN] == Slave_BackupId[COOKER_PARSE_ADDR_LEN])&& (cmp_buf((char *)id_check,(char *)Slave_BackupId, COOKER_PARSE_ADDR_LEN)))
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

    SlaveGasCTRL = 0;
    //关闭气阀
    QA_PowerL();//关闭电磁阀
    delay_ms(100);
    QA_PowerH();//关闭电磁阀
    delay_ms(5000);
    QA_PowerL();//关闭电磁阀
    delay_ms(200);
    if(!READ_Level())
    {
       delay_ms(20);
       if(!READ_Level())
       {//发送关闭气阀的指令
        Cooker_Parse_t entity;
        entity.cmd	= eCOOKER_CTRL_Gas;
        entity.payload[0]	= COOKER_PARSE_FALSE;
        entity.length		= 1;
        Slave_Load(&entity);      
        Power_CurState = READ_Level();
        Power_PreState = READ_Level();
       }
    }
    //printf("\n Cooker_SendGas_CTRL \n");
}

