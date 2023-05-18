#include "Extdata.h"

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
        FLASH_Lock(FLASH_MemType_Program);                //上锁
    }
}

void FLASH_ReadNByte(uint8_t* pBuffer, uint32_t ReadAddr, uint8_t nByte)
{
  u8 j=0;
   FLASH_Unlock(FLASH_MemType_Data);
  for(j = 0;j < EEPROMPAGESIZE; j++)
  {
    pBuffer[j]= FLASH_ReadByte(ReadAddr);
    ReadAddr++;
  }
  FLASH_Lock(FLASH_MemType_Program);                //上锁
}

void Cooker_AFN_Handle(Cooker_Parse_t *entity)
{
    switch ((Cooker_Cmd_e)entity->cmd)
    {
    case eCOOKER_SET_SYS_ID:
    {
        if (1 == entity->length)
        {
            FLASH_WriteNByte((unsigned char *)entity->addr,PARA_START_INDEX,FlASH_OPER_SIZE);
        }
    }
    break;
    case eCOOKER_FIRE_STATE:
    {
        // if (1 == entity->length)
        //	bSend_FireState = TRUE;
    }
    break;
    case eCOOKER_CTRL_Gas:
    {
        if ((1 == entity->length) && (COOKER_PARSE_FALSE == entity->payload[0]))
        {
            //关闭气阀
            QA_PowerH();//关闭电磁阀
            if(!READ_Level())
            {
               delay_ms(6);
               if(!READ_Level())
               {//发送关闭气阀的指令
                Cooker_Parse_t entity;
                entity.cmd	= eCOOKER_CTRL_Gas;
                entity.payload[0]	= COOKER_PARSE_FALSE;
                entity.length		= 1;
                Slave_Load(&entity);             
               }
               else break;
            }
        }
    }
    break;

    default:
        break;
    }
}