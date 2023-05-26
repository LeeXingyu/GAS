
/* 包含系统头文件 */
#include <stdarg.h>

/* 包含自定义头文件 */
#include "data.h"
/* 自定义新类型 */
typedef enum _Slave_State_e
{
	eCOOKER_PARSE_NOP,
	eCOOKER_PARSE_HEADER,
	eCOOKER_PARSE_ADDR,
	eCOOKER_PARSE_LENGTH,
	eCOOKER_PARSE_CMD,
	eCOOKER_PARSE_PAYLOAD,
	eCOOKER_PARSE_CHECK,
}Slave_State_e;
/* 自定义宏 */
static void MasterParse_Nop(int c);
static void MasterParse_Header(int c);
static void MasterParse_Addr(int c);
static void MasterParse_Length(int c);
static void MasterParse_Cmd(int c);
static void MasterParse_Payload(int c);
static void MasterParse_Check(int c);
    
/* 全局变量定义 */
data_buf data;
static Cooker_Parse_t	tCooker_Entity;
static Slave_State_e	mPARSE_State		= eCOOKER_PARSE_NOP;
static unsigned int	usCooker_ParseLength= 0;
static unsigned int	usCooker_ParseCheck = CRC_INITIAL;

unsigned char Gas_Pressure_state = 0;
unsigned char Bat_Check_state = 0;

unsigned char UID_data[12];

static void (*const a_pfnCooker_Parse[])(int c) =
{
	MasterParse_Nop,
	MasterParse_Header,
	MasterParse_Addr,
	MasterParse_Length,
	MasterParse_Cmd,
	MasterParse_Payload,
	MasterParse_Check,
};

static void MasterParse_Nop(int c)
{

}

static void MasterParse_Header(int c)
{
	if ((unsigned char)COOKER_PARSE_HEADER == (unsigned char)c)
		mPARSE_State = eCOOKER_PARSE_ADDR;
	else
		mPARSE_State = eCOOKER_PARSE_NOP;
}

static void MasterParse_Addr(int c)
{
	tCooker_Entity.addr[usCooker_ParseLength++] = c;

	if (usCooker_ParseLength >= COOKER_PARSE_ADDR_LEN)
	{
		usCooker_ParseLength = 0;

		mPARSE_State = eCOOKER_PARSE_LENGTH;
	}
}

static void MasterParse_Length(int c)
{
	tCooker_Entity.length	= c;
	mPARSE_State			= eCOOKER_PARSE_CMD;
}

static void MasterParse_Cmd(int c)
{
	tCooker_Entity.cmd	= c;
	
    if (tCooker_Entity.length)
        mPARSE_State   = eCOOKER_PARSE_PAYLOAD;
    else
        mPARSE_State   = eCOOKER_PARSE_CHECK;
}

static void MasterParse_Payload(int c)
{
	tCooker_Entity.payload[usCooker_ParseLength++] = c;

	if ((usCooker_ParseLength >= tCooker_Entity.length) || (usCooker_ParseLength >= COOKER_PARSE_PAYLOAD_LEN))
	{
		usCooker_ParseLength = 0;

		mPARSE_State =  eCOOKER_PARSE_CHECK;
	}
}

static void MasterParse_Check(int c)
{
        usCooker_ParseLength++;
	if (++usCooker_ParseLength >= 2)
	{
	    usCooker_ParseLength = 0;
            if ((unsigned char)eCOOKER_SET_SYS_ID != tCooker_Entity.cmd)
            //if (0X01 != tCooker_Entity.cmd)
            {
                unsigned char id[COOKER_PARSE_ADDR_LEN];

                GetMasterId(id);

                if (cmp_buf((char *)id, (char *)tCooker_Entity.addr, COOKER_PARSE_ADDR_LEN))
                    Cooker_AFN_Handle(&tCooker_Entity);
            }
            else
            {   
              Cooker_AFN_Handle(&tCooker_Entity);               
            }
            mPARSE_State++;//放在这里会重启
	}
}

void Rcv_MasterDataParse(void)
{
	unsigned char rc = ERROR;
	unsigned int count;

	ReceiveRfFrame((unsigned char *)(&RF_Pkt), sizeof(RF_Pkt), &rc);
	if(rc == OK)
	{
		count = 0;
		while (count < RF_Pkt.key)
		{
			Master_data_Prase(RF_Pkt.fill[count]);
			count++;
		}
	}
}

void Master_data_Prase(int c)
{
    if (mPARSE_State > eCOOKER_PARSE_CHECK)
            mPARSE_State = eCOOKER_PARSE_NOP;
    
    if (eCOOKER_PARSE_NOP == mPARSE_State)
    {
            memset((char *)&tCooker_Entity, 0, sizeof(Cooker_Parse_t));

            usCooker_ParseCheck = CRC_INITIAL;
            usCooker_ParseLength= 0;

            mPARSE_State = eCOOKER_PARSE_HEADER;
    }

    usCooker_ParseCheck = crc16_ccitt_byte(usCooker_ParseCheck, c);

    a_pfnCooker_Parse[mPARSE_State](c);
}

void GetMasterId(unsigned char *id)
{
  FLASH_ReadNByte((unsigned char *)id,PARA_START_INDEX,FlASH_OPER_SIZE);
}

unsigned int Slave_Load(Cooker_Parse_t *entity)
{
        unsigned int check = CRC_INITIAL;
	unsigned int index = 0, count;
	unsigned char id[COOKER_PARSE_ADDR_LEN];
	char load[60];

	GetMasterId(id);
       // while(1);
        load[index++] = COOKER_PARSE_HEADER;
	check = crc16_ccitt_byte(check, COOKER_PARSE_HEADER);
	for (count = 0; count < COOKER_PARSE_ADDR_LEN; count++)
	{
		load[index++] = id[count];
		check = crc16_ccitt_byte(check, id[count]);
	}
	
	load[index++] = entity->length;
	check = crc16_ccitt_byte(check, entity->length);

	load[index++] = entity->cmd;
	check = crc16_ccitt_byte(check, entity->cmd);

	for (count = 0; count < entity->length; count++)
	{
		load[index++] = entity->payload[count];
		check = crc16_ccitt_byte(check, entity->payload[count]);
	}

	check = ~check;
	load[index++] = (unsigned char)check;
	load[index++] = (unsigned char)(check >> 8);

	Slave_WirelessSendLoad(load, index);

	return index;
}

void Slave_WirelessSendLoad(char *load, unsigned int len)
{
	if (BuildPacket((unsigned char *)load, len))
		SendRfFrame((unsigned char *)(&RF_Pkt), sizeof(RF_Pkt));
    

}

void Slave_Send_GasState(void)
{
      Cooker_Parse_t entity;

      entity.cmd	= eCOOKER_STATE_Gas;
      entity.payload[0]	= Gas_Pressure_state;
      entity.length		= 1;

      Slave_Load(&entity);
}

void Slave_Send_BatState(void)
{
      Cooker_Parse_t entity;

      entity.cmd	= eCOOKER_STATE_Bat;
      entity.payload[0]	= Bat_Check_state;
      entity.length		= 1;

      Slave_Load(&entity);
}

/*
//读取stm8L的ID
void Cooker_GetId(unsigned char *CPU_XDATA id)
{
  UID_data[0]=*(unsigned char*)(0x4926);
  UID_data[1]=*(unsigned char*)(0x4927);
  UID_data[2]=*(unsigned char*)(0x4928);
  UID_data[3]=*(unsigned char*)(0x4929);
  UID_data[4]=*(unsigned char*)(0x492a);
  UID_data[5]=*(unsigned char*)(0x492b);
  UID_data[6]=*(unsigned char*)(0x492c);
  UID_data[7]=*(unsigned char*)(0x492d);
  UID_data[8]=*(unsigned char*)(0x492e);
  UID_data[9]=*(unsigned char*)(0x492f);
  UID_data[10]=*(unsigned char*)(0x4930);
  UID_data[11]=*(unsigned char*)(0x4931);
}
*/




/*--------------------------------------------------------------------------------------------------------
                   									   0ooo
                   								ooo0     (   )
                								(   )     ) /
                								 \ (     (_/
                								  \_)
----------------------------------------------------------------------------------------------------------*/
