#ifndef _ELACO_MAIN_H_
#define _ELACO_MAIN_H_

#include "stdbool.h"
#include "string.h"
#include "stdlib.h"

#include "ela_uart.h"
#include "ela_can_queue.h"
#include "ela_uart_queue.h"

#include "stm32f1xx_hal.h"
#include "main.h"
#include "usart.h"

extern UART_HandleTypeDef huart3;
extern DMA_HandleTypeDef hdma_usart3_rx;
extern DMA_HandleTypeDef hdma_usart3_tx;

void elaco_main(void); /* 主循环，放在 main 函数的 while(1) 前 */

#define ModTest

#endif
