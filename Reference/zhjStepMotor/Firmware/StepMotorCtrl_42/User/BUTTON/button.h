#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>
#include <stdbool.h>

void Button_Init(void);
void Button_Tick(void);           // 每 10ms 调用一次
bool Button_GetClick(uint8_t id);   // 获取单击，读后自动清除
bool Button_GetLong(uint8_t id);    // 获取长按，读后自动清除
bool Button_IsPressed(uint8_t id);  // 当前是否按下

#endif