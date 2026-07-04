#include "led.h"
#include "main.h"  

/* 内部状态变量 */
static uint32_t s_timer = 0;
static uint32_t s_timer_heartbeat = 0;
static uint32_t s_timer_blink = 0;
static bool s_motor_enable = false;
static bool s_heartbeat_enable = false;
static uint8_t s_target_blink_num = 0;
static uint8_t s_blink_num = 0;
static uint8_t s_heartbeat_phase = 1;
static uint8_t s_blink_phase = 1;

/* 内部函数：设置LED状态 */
static void LED_SetState(uint8_t id, bool state)
{
    if (state) {
        if (id == 0)
            HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
        else
            HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
    } else {
        if (id == 0)
            HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
        else
            HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
    }
}

void LED_Init(void)
{
    s_timer = 0;
    s_timer_heartbeat = 0;
    s_timer_blink = 0;
    s_motor_enable = false;
    s_heartbeat_enable = false;
    s_target_blink_num = 0;
    s_blink_num = 0;
    s_heartbeat_phase = 1;
    s_blink_phase = 1;
    
    /* 初始状态：两灯都灭 */
    LED_SetState(0, false);
    LED_SetState(1, false);
}

void LED_Tick(uint32_t time_elapse_millis, Motor_State_t state)
{
    s_timer += time_elapse_millis;

    /* 根据电机状态设置LED模式 */
		switch (state)
		{
				case STATE_NO_CALIB:    
						s_motor_enable = false;
						s_heartbeat_enable = false;
						s_target_blink_num = 1;
						break;
				case STATE_RUNNING:      
						s_motor_enable = true;
						s_heartbeat_enable = true;
						s_target_blink_num = 0;
						break;
				case STATE_FINISH:        
						s_motor_enable = true;
						s_heartbeat_enable = false;
						s_target_blink_num = 0;
						break;
				case STATE_STOP:          
						s_motor_enable = false;
						s_heartbeat_enable = false;
						s_target_blink_num = 0;
						break;
				case STATE_OVERLOAD:      
						s_motor_enable = true;
						s_heartbeat_enable = false;
						s_target_blink_num = 3;
						break;
				case STATE_STALL:         
						s_motor_enable = false;
						s_heartbeat_enable = false;
						s_target_blink_num = 2;
						break;
		}
		
    /* LED0 (LED1): 心跳或常亮/常灭控制 */
    if (s_motor_enable)
    {
        if (s_heartbeat_enable)
        {
            switch (s_heartbeat_phase)
            {
                case 1:
                    if (s_timer - s_timer_heartbeat > 100)
                    {
                        LED_SetState(0, false);
                        s_timer_heartbeat = s_timer;
                        s_heartbeat_phase = 2;
                    }
                    break;
                case 2:
                    if (s_timer - s_timer_heartbeat > 100)
                    {
                        LED_SetState(0, true);
                        s_timer_heartbeat = s_timer;
                        s_heartbeat_phase = 3;
                    }
                    break;
                case 3:
                    if (s_timer - s_timer_heartbeat > 100)
                    {
                        LED_SetState(0, false);
                        s_timer_heartbeat = s_timer;
                        s_heartbeat_phase = 4;
                    }
                    break;
                case 4:
                    if (s_timer - s_timer_heartbeat > 700)
                    {
                        LED_SetState(0, true);
                        s_timer_heartbeat = s_timer;
                        s_heartbeat_phase = 1;
                    }
                    break;
                default:
                    s_heartbeat_phase = 1;
                    break;
            }
        }
        else
        {
            LED_SetState(0, true);
            s_heartbeat_phase = 1;
        }
    }
    else
    {
        LED_SetState(0, false);
    }

    /* LED1 (LED2): 错误码闪烁控制 */
    switch (s_blink_phase)
    {
        case 1:
            if (s_timer - s_timer_blink > 100)
            {
                LED_SetState(1, false);
                s_timer_blink = s_timer;
                s_blink_phase = 2;
            }
            break;
        case 2:
            if (s_timer - s_timer_blink > 100)
            {
                s_blink_num++;
                if (s_target_blink_num > s_blink_num)
                {
                    LED_SetState(1, true);
                    s_blink_phase = 1;
                    s_timer_blink = s_timer;
                }
                else
                {
                    LED_SetState(1, false);
                    s_blink_phase = 3;
                    s_timer_blink = s_timer;
                }
            }
            break;
        case 3:
            if (s_timer - s_timer_blink > 1000)
            {
                s_blink_num = 0;
                LED_SetState(1, (s_target_blink_num > 0));
                s_timer_blink = s_timer;
                s_blink_phase = 1;
            }
            break;
        default:
            s_blink_phase = 1;
            break;
    }
}

