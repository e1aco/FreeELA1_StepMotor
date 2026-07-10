#ifndef MT6816_H
#define MT6816_H

#include <stdint.h>
#include <stdbool.h>

/* 分辨率：14位 = 16384 */
#define MT6816_RESOLUTION  16384

/* 更新角度，返回校准后的角度值(0-16383) */
uint16_t MT6816_UpdateAngle(void);

/* 获取原始角度(未校准) */
uint16_t MT6816_GetRawAngle(void);

/* 获取校准后的角度 */
uint16_t MT6816_GetRectifiedAngle(void);

/* 检查是否已校准 */
bool MT6816_IsCalibrated(void);

/* 设置校准数据指针（在Init之前调用） */
void MT6816_SetCalibrationData(uint16_t* cali_data_ptr);

uint16_t MT6816_TestRead(void);

#endif /* MT6816_H */
