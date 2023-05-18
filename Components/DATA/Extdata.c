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
        FLASH_Lock(FLASH_MemType_Program);                //ÉÏËø
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
  FLASH_Lock(FLASH_MemType_Program);                //ÉÏËø
}

void Cooker_AFNChk(Cooker_Parse_t *entity)
{
  FLASH_WriteNByte((unsigned char *)entity->addr,PARA_START_INDEX,FlASH_OPER_SIZE);
//    switch ((Cooker_Cmd_e)entity->cmd)
//    {
//    case eCOOKER_SET_SYS_ID:
//    {
//        if (0 == entity->length)
//        {
//            //ucSendSetIdResult = 1;
//
//            //memcpy((char *)a_ucCooker_BackupId, (char *)entity->addr, COOKER_PARSE_ADDR_LEN);
//            FLASH_WriteNByte((char *)entity->addr,PARA_START_INDEX,FlASH_OPER_SIZE);
//        }
//    }
//    break;
//    case eCOOKER_FIRE_STATE:
//    {
//        // if (1 == entity->length)
//        //	bSend_FireState = TRUE;
//    }
//    break;
//    case eCOOKER_CTRL_FIRE:
//    {
//        if ((1 == entity->length) && (COOKER_PARSE_FALSE == entity->payload[0]))
//            Cooker_MasterCtrlOff();
//    }
//    break;
//
//    default:
//        break;
//    }
}