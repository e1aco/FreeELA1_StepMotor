#include "ela_uart.h"

#include "usart.h"
#include <stdio.h>

/* 串口3 (调试口) 接收变量 */
unsigned char g_rx3_buf[DMA_BUF_SIZE];
unsigned char g_rx3_offset;

/* uart drv start */

/********
 * @ 说明: 重定向 printf 到 USART3 (ARM Compiler / Keil 路径)
 * @ 注意: 每字符阻塞发送，调试信息量大时可能影响实时性
 ****/
int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&huart3, (unsigned char *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}

/********
 * @ 说明: 重定向 printf 到 USART3 (Newlib / PicoLibC _write 路径)
 * @ 注意: 提供字符级输出接口，覆盖 syscalls.c 中的弱定义
 ****/
int __io_putchar(int ch)
{
    HAL_UART_Transmit(&huart3, (unsigned char *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}

/* uart drv end */
//----------------------------------------------------------------------------------
/* uart usr start */

/********
 * @ 说明: 初始化 printf 重定向，禁用 stdout 行缓冲
 * @ 注意: 调用后 printf 输出将通过 fputc / __io_putchar 发送到 USART3
 ****/
void Uart_PrintfInit(void)
{
    setvbuf(stdout, NULL, _IONBF, 0);
}

/* uart usr end */
