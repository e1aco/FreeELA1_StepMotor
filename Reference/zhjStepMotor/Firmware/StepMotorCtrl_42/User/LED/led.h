#ifndef LED_H
#define LED_H

#include <stdint.h>
#include <stdbool.h>
#include "motor.h"

/* LED初始化 */
void LED_Init(void);

/* 定时器Tick处理，在主循环或定时器中调用 */
void LED_Tick(uint32_t time_elapse_millis, Motor_State_t state);

#endif /* LED_H */

