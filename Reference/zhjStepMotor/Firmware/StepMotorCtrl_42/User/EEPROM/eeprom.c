#include "eeprom.h"
#include "stockpile_f103cb.h"
#include <string.h>

/* EEPROM存储地址（使用stockpile_data分区） */
#define EEPROM_BASE_ADDR    STOCKPILE_APP_DATA_ADDR
#define EEPROM_MAX_SIZE     1024  /* 1KB */

/* 标记分区是否已擦除 */
static bool s_is_erased = false;

void EEPROM_Read(uint32_t addr, void* data, uint32_t size)
{
    if (addr + size > EEPROM_MAX_SIZE) return;
    
    uint8_t* dst = (uint8_t*)data;
    uint32_t flash_addr = EEPROM_BASE_ADDR + addr;
    
    for (uint32_t i = 0; i < size; i++) {
        dst[i] = *((uint8_t*)flash_addr + i);
    }
}

void EEPROM_Write(uint32_t addr, void* data, uint32_t size)
{
    if (addr + size > EEPROM_MAX_SIZE) return;
    
    /* 第一次写入时，先擦除整个分区 */
    if (!s_is_erased) {
        Stockpile_Flash_Data_Empty(&stockpile_data);
        s_is_erased = true;
    }
    
    /* 开始写入 */
    Stockpile_Flash_Data_Begin(&stockpile_data);
    
    /* 设置写地址 */
    Stockpile_Flash_Data_Set_Write_Add(&stockpile_data, EEPROM_BASE_ADDR + addr);
    
    /* 写入数据（16位对齐） */
    uint32_t halfword_count = (size + 1) / 2;
    Stockpile_Flash_Data_Write_Data16(&stockpile_data, (uint16_t*)data, halfword_count);
    
    /* 结束写入 */
    Stockpile_Flash_Data_End(&stockpile_data);
}

bool EEPROM_IsValid(void)
{
    uint32_t* pFirst = (uint32_t*)EEPROM_BASE_ADDR;
    
    /* 检查第一个字是否全0xFF（空Flash） */
    return (*pFirst != 0xFFFFFFFF);
}

/* 可选：手动擦除整片 EEPROM 区域 */
void EEPROM_Erase(void)
{
    Stockpile_Flash_Data_Empty(&stockpile_data);
    s_is_erased = true;
}