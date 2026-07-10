#include "uart_cmd.h"
#include "motor.h"
#include "configurations.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>

/* 外部引用 */
extern BoardConfig_t boardConfig;

void UartCmd_Process(uint8_t* data, uint16_t len)
{
    float cur, pos, vel;
    int ret = 0;
    char buffer[64];
    
    if (len == 0) return;
	
    switch (data[0])
    {
        case 'c':  /* 电流模式 */
            ret = sscanf((char*)data, "c %f", &cur);
            if (ret < 1) {
                Uart_SendString("[error] Command format error!\r\n");
            } else {
                if (Motor_GetMode() != MODE_COMMAND_CURRENT) {
                    Motor_SetMode(MODE_COMMAND_CURRENT);
                }
                Motor_SetCurrent((int32_t)(cur * 1000));
            }
            break;
            
        case 'v':  /* 速度模式 */
            ret = sscanf((char*)data, "v %f", &vel);
            if (ret < 1) {
                Uart_SendString("[error] Command format error!\r\n");
            } else {
                if (Motor_GetMode() != MODE_COMMAND_VELOCITY) {
                    Motor_SetMode(MODE_COMMAND_VELOCITY);
                }
                Motor_SetVelocity((int32_t)(vel * MOTOR_SUBDIVIDE_STEPS));
            }
            break;
            
        case 'p':  /* 位置模式 */
            ret = sscanf((char*)data, "p %f", &pos);  
            if (ret < 1) {
                Uart_SendString("[error] Command format error!\r\n");
            } else {
                if (Motor_GetMode() != MODE_COMMAND_POSITION) {
                    Motor_SetMode(MODE_COMMAND_POSITION);
                }
                Motor_SetPosition((int32_t)(pos * MOTOR_SUBDIVIDE_STEPS));
            }
            break;
						
				case 's':  /* 停止 */
							Motor_SetMode(MODE_STOP);
              break;
				
				case 'z':  /* 位置清零 */
							Motor_ZeroPosition();
              break;
				
         case 'l':  /* 清楚堵转 */
							Motor_ClearStallFlag();
              break;
        default:
            sprintf(buffer, "[error] Unknown command: %c\r\n", data[0]);
            Uart_SendString(buffer);
            break;
    }
}

