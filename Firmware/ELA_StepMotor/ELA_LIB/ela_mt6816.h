#ifndef _ELA_MT6816_H_
#define _ELA_MT6816_H_

#include "main.h"
#include <stdbool.h>

/* 角度获取结构体 */
typedef struct MT6816_ANGLE
{
    unsigned short raw_data;    // 原始数据 16位，包含14位角度数据、1位无磁场标志和1位奇偶校验位（一般用不上）
    unsigned short raw_angle;   // 解析后的角度值 (0 ~ 16383)
    unsigned int micro_angle;   // 角度值 (0 ~ 51200),每一个编码器值对应的微步的值。
    bool data_valid;            // 传感器数据有效标志，true表示数据正确，false表示数据错误（如校验失败）
    bool magnet_valid;          // 磁铁有效标志，true表示磁铁存在，false表示磁铁不存在
    bool direciton;             // 方向标志，0表示正向，1表示反向（所以默认是正向）
}MT6816_ANGLE_T;

extern MT6816_ANGLE_T g_mt6816_st; // 角度获取结构体

/* 读取命令值 */
#define MT6816_CMD_ANGLE      0x03  /* 读取角度 */
#define MT6816_CMD_RAW_ANGLE  0x04  /* 读取原始角度 */

/* SPI片选引脚电平转换 */
#define MT6816_CS_HIGH()    HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_SET)
#define MT6816_CS_LOW()     HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_RESET)

/* 函数声明 */
void MT6816_Init(void);
void MT6816_GetAngle(void);

#endif
