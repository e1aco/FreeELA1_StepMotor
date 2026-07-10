#ifndef EEPROM_H
#define EEPROM_H

#include <stdint.h>
#include <stdbool.h>

/* ��ȡ���ݣ���Flash���ڴ棩 */
void EEPROM_Read(uint32_t addr, void* data, uint32_t size);

/* д�����ݣ��ڴ浽Flash�� */
void EEPROM_Write(uint32_t addr, void* data, uint32_t size);

/* ���EEPROM�Ƿ�����Ч���� */
bool EEPROM_IsValid(void);

#endif

