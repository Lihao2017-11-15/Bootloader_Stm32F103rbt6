#include "flash_if.h"



static uint32_t GetSize_Sector(uint32_t Size)
{
    uint32_t pagenumber = 0x0;

    if ((Size % PAGE_SIZE) != 0)
    {
        pagenumber = (Size / PAGE_SIZE) + 1;
    }
    else
    {
        pagenumber = Size / PAGE_SIZE;
    }
    return pagenumber;
}

/********************************************************************************
* ������ :  InternalFLASH_Init
* ��  �� :  stm32���ڲ�flash��ʼ��(����)
* ˵  �� :  none
* ��  �� :  none
* ��  �� :  none
* ��  �� :  Shatang                   ʱ  �� :  2020.06.18
* ��  �� :  none                      ʱ  �� :  none
********************************************************************************/
void FLASH_If_Init(void)
{ 
    FLASH_Unlock();

//    /* Clear pending flags (if any) */
//    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
//                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
}
/********************************************************************************
* ������ : FLASH_If_Finish
* ��  �� : stm32���ڲ�flash�ر�(����)
* ˵  �� : none
* ��  �� : none
* ��  �� : none
* ��  �� : Shatang                    ʱ  �� : 2020.07.29
* ��  �� : none                       ʱ  �� : none
********************************************************************************/
void FLASH_If_Finish(void)
{
//    /* Clear pending flags (if any) */
//    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
//                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
    
    FLASH_Lock();
}
/********************************************************************************
* ������ :  FLASH_If_Erase
* ��  �� :  stm32���ڲ�flash����
* ˵  �� :  ֻ���APP�ļ���ַ�Ĵ洢���ݽ��в���
* ��  �� :  none
* ��  �� :  none
* ��  �� :  Shatang                   ʱ  �� :  2020.06.18
* ��  �� :  none                      ʱ  �� :  none
********************************************************************************/
uint32_t FLASH_If_Erase(uint32_t Flash_StartAddr)
{
    uint32_t i = 0;
    uint32_t UserSize_Sector = APPLICATION_END_ADDRESS - Flash_StartAddr;
    uint32_t PageNum = GetSize_Sector(UserSize_Sector);
    FLASH_Status FLASHStatus = FLASH_COMPLETE;
    
    for(i = 0; (i < PageNum) && (FLASHStatus == FLASH_COMPLETE); i++)
    {
        FLASHStatus = FLASH_ErasePage(Flash_StartAddr + (PAGE_SIZE * i));
    }
    
    return (0);
}
/********************************************************************************
* ������ :  FLASH_If_Write
* ��  �� :  stm32���ڲ�flashд����
* ˵  �� :  none
* ��  �� :  none
* ��  �� :  none
* ��  �� :  Shatang                   ʱ  �� :  2020.06.18
* ��  �� :  none                      ʱ  �� :  none
********************************************************************************/
uint32_t FLASH_If_Write(__IO uint32_t* FlashAddress, uint32_t* Data ,uint32_t DataLength)
{
    uint32_t i = 0;

    for (i = 0; (i < DataLength) && (*FlashAddress <= (APPLICATION_END_ADDRESS-4)); i++)
    {
        /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
           be done by word */ 
        if (FLASH_ProgramWord(*FlashAddress, *(uint32_t*)(Data+i)) == FLASH_COMPLETE)
        {
            /* Check the written value */
            if (*(uint32_t*)*FlashAddress != *(uint32_t*)(Data+i))
            {
                /* Flash content doesn't match SRAM content */
                return(2);
            }
            /* Increment FLASH destination address */
            *FlashAddress += 4;
        }
        else
        {
            /* Error occurred while writing data in Flash memory */
            return (1);
        }
    }

    return (0);
}


//��������
//uint16_t FLASH_If_GetWriteProtectionStatus(void);
//uint32_t FLASH_If_DisableWriteProtection(void);
