#include "button.h"
#include "main.h"

#define BUTTON_NUM      2
#define LONG_PRESS_MS   3000

static bool s_pressed[BUTTON_NUM + 1];
static uint32_t s_press_time[BUTTON_NUM + 1];
static bool s_click_flag[BUTTON_NUM + 1];
static bool s_long_flag[BUTTON_NUM + 1];
static bool s_last_state[BUTTON_NUM + 1];

static bool ReadPin(uint8_t id)
{
    switch (id) {
        case 1: return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == GPIO_PIN_RESET;
        case 2: return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2) == GPIO_PIN_RESET;
        default: return false;
    }
}

void Button_Init(void)
{
    for (int i = 1; i <= BUTTON_NUM; i++) {
        s_last_state[i] = ReadPin(i);
        s_pressed[i] = false;
        s_press_time[i] = 0;
        s_click_flag[i] = false;
        s_long_flag[i] = false;
    }
}

void Button_Tick(void)
{
    uint32_t now = HAL_GetTick();
    
    for (int i = 1; i <= BUTTON_NUM; i++) {
        bool cur = ReadPin(i);
        
        // 检测下降沿（按下）
        if (cur == true && s_last_state[i] == false) {
            s_pressed[i] = true;
            s_press_time[i] = now;
            s_click_flag[i] = false;
            s_long_flag[i] = false;
        }
        
        // 检测上升沿（释放）
        if (cur == false && s_last_state[i] == true) {
            s_pressed[i] = false;
            // 释放时判断是单击还是长按
            if (s_long_flag[i] == false) {
                s_click_flag[i] = true;
            }
        }
        
        // 按住中，检测长按
        if (s_pressed[i] == true) {
            if (s_long_flag[i] == false && (now - s_press_time[i]) >= LONG_PRESS_MS) {
                s_long_flag[i] = true;
                s_click_flag[i] = false;  // 触发了长按就不算单击
            }
        }
        
        s_last_state[i] = cur;
    }
}

bool Button_GetClick(uint8_t id)
{
    if (id < 1 || id > BUTTON_NUM) return false;
    if (s_click_flag[id]) {
        s_click_flag[id] = false;
        return true;
    }
    return false;
}

bool Button_GetLong(uint8_t id)
{
    if (id < 1 || id > BUTTON_NUM) return false;
    if (s_long_flag[id]) {
        s_long_flag[id] = false;
        return true;
    }
    return false;
}

bool Button_IsPressed(uint8_t id)
{
	if(id == 1)
		return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == GPIO_PIN_RESET;
	if(id == 2)
		return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2) == GPIO_PIN_RESET;
}