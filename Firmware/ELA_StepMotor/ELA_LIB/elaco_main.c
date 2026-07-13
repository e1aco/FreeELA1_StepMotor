#include "elaco_main.h"
#include "mb.h"

#ifdef ModTest
    #include "test_mt6816.h"
#endif
/* elaco_main usr start */

/********
 * @ 说明: 主循环函数
 ****/
void elaco_main(void)
{
    Uart_PrintfInit();
    Uart3_DMA_Init();
#ifdef ModTest

#endif

    while (1)
    {

    }
}

/* elaco_main usr end */
//----------------------------------------------------------------------------------
/* elaco_main cac start */

/********
 * @ 说明: USART3 DMA 发送完成回调，通知 TX 模块继续链式发送
 ****/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (USART3 == huart->Instance)
    {
        g_uart3_dma_tx_st.dma_busy = false;
        Uart3_DMA_TxContinue();
    }
}

/********
 * @ 说明: USART3 DMA 循环接收满回调，仅标记缓冲区绕回事件
 * @ 注意: 实际数据搬移由 IDLE 中断完成，此处可留空或打日志
 ****/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (USART3 == huart->Instance)
    {
        /* 循环 DMA 每 256 字节触发一次，数据已在 IDLE 时处理 */
    }
}

/* elaco_main cac end */

