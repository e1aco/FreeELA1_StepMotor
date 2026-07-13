#include "ela_uart.h"

#include "usart.h"
#include <stdio.h>

/* Static variables */
static uint16_t rx_buf_pos;

/* Global instances */
UART_DMA_RX_T g_uart3_dma_rx_st;
UART_DMA_TX_T g_uart3_dma_tx_st;

/* uart drv start */

/********
 * @ 说明: 重定向 printf 到 USART3 (ARM Compiler / Keil 路径)
 * @ 注意: 每字符阻塞发送，调试信息量大时可能影响实时性。DMA 发送
 *        期间掉数据风险低（调试口和业务口共用 USART3），可接受
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

/********
 * @ 说明: USART3 DMA 接收空闲中断处理，将 DMA 缓冲区数据搬入 RX 队列
 * @ 注意: 由 USART3_IRQHandler 在 HAL_UART_IRQHandler 之前调用。
 *        必须在 HAL 读 DR 之前检查 IDLE，否则 HAL 会误清 IDLE 标志
 ****/
void Uart3_DMA_RxIdleHandler(void)
{
    uint16_t ndtr;
    uint16_t wr_ptr;
    uint16_t len;

    if (0 == __HAL_UART_GET_FLAG(&huart3, UART_FLAG_IDLE))
    {
        return;
    }
    /* 读 SR 再读 DR 清除 IDLE 标志 */
    __HAL_UART_CLEAR_IDLEFLAG(&huart3);

    ndtr   = __HAL_DMA_GET_COUNTER(huart3.hdmarx);
    wr_ptr = UART_DMA_RX_BUF_SIZE - ndtr;

    if (wr_ptr > rx_buf_pos)
    {
        /* 数据连续，未绕回 */
        len = wr_ptr - rx_buf_pos;
        UartQueue_PutBuf(&g_uart3_dma_rx_st.queue,
                         &g_uart3_dma_rx_st.dma_buf[rx_buf_pos], len);
    }
    else if (wr_ptr < rx_buf_pos)
    {
        /* DMA 绕回，分两段拷贝 */
        len = UART_DMA_RX_BUF_SIZE - rx_buf_pos;
        UartQueue_PutBuf(&g_uart3_dma_rx_st.queue,
                         &g_uart3_dma_rx_st.dma_buf[rx_buf_pos], len);
        if (wr_ptr > 0)
        {
            UartQueue_PutBuf(&g_uart3_dma_rx_st.queue,
                             g_uart3_dma_rx_st.dma_buf, wr_ptr);
        }
    }
    /* wr_ptr == rx_buf_pos: 无新数据 */

    rx_buf_pos = wr_ptr;
}

/********
 * @ 说明: 检查 TX 队列，有数据则启动 DMA 发送
 * @ 注意: 在 HAL_UART_TxCpltCallback 和各 Send 函数末尾调用，
 *        形成链式自动发送。发送中 dma_busy 为 true 则直接返回
 ****/
void Uart3_DMA_TxContinue(void)
{
    uint16_t len;

    if (g_uart3_dma_tx_st.dma_busy)
    {
        return;
    }

    len = UartQueue_Count(&g_uart3_dma_tx_st.queue);
    if (0 == len)
    {
        return;
    }

    if (len > UART_DMA_TX_BUF_SIZE)
    {
        len = UART_DMA_TX_BUF_SIZE;
    }

    len = UartQueue_GetBuf(&g_uart3_dma_tx_st.queue,
                           g_uart3_dma_tx_st.dma_buf, len);
    if (len > 0)
    {
        g_uart3_dma_tx_st.dma_busy = true;
        HAL_UART_Transmit_DMA(&huart3, g_uart3_dma_tx_st.dma_buf, len);
    }
}

/********
 * @ 说明: 初始化 USART3 DMA 收发，启动循环接收
 * @ 注意: 调用后 DMA 自动填充 RX 缓冲区，IDLE 中断将数据搬入
 *        RX 队列。TX 队列初始化后等待用户写入
 ****/
void Uart3_DMA_Init(void)
{
    UartQueue_Init(&g_uart3_dma_rx_st.queue);
    rx_buf_pos = 0;

    UartQueue_Init(&g_uart3_dma_tx_st.queue);
    g_uart3_dma_tx_st.dma_busy = false;

    HAL_UART_Receive_DMA(&huart3, g_uart3_dma_rx_st.dma_buf,
                         UART_DMA_RX_BUF_SIZE);
    __HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);
}

/* uart drv end */
//----------------------------------------------------------------------------------
/* uart usr start */

/********
 * @ 输入: data: 源数据; len: 发送长度
 * @ 输出: 实际入队字节数
 * @ 说明: 非阻塞发送，数据先入 TX 队列，由 DMA 自动搬送。
 *        TX 队列满时可能丢弃超量数据
 ****/
uint16_t Uart3_DMA_SendBuf(uint8_t *data, uint16_t len)
{
    uint16_t written;

    written = UartQueue_PutBuf(&g_uart3_dma_tx_st.queue, data, len);
    Uart3_DMA_TxContinue();

    return written;
}

/********
 * @ 输入: byte: 发送字节
 * @ 输出: true 成功, false 队列满
 * @ 说明: 非阻塞发送单个字节
 ****/
bool Uart3_DMA_SendByte(uint8_t byte)
{
    if (!UartQueue_Put(&g_uart3_dma_tx_st.queue, byte))
    {
        return false;
    }
    Uart3_DMA_TxContinue();
    return true;
}

/********
 * @ 输入: data: 目标缓冲区; len: 读取长度
 * @ 输出: 实际读出字节数
 * @ 说明: 从 RX 队列读取数据。无数据时返回 0
 ****/
uint16_t Uart3_DMA_RecvBuf(uint8_t *data, uint16_t len)
{
    return UartQueue_GetBuf(&g_uart3_dma_rx_st.queue, data, len);
}

/********
 * @ 输入: byte: 读出字节指针
 * @ 输出: true 成功, false 队列空
 * @ 说明: 从 RX 队列读取单个字节
 ****/
bool Uart3_DMA_RecvByte(uint8_t *byte)
{
    return UartQueue_Get(&g_uart3_dma_rx_st.queue, byte);
}

/********
 * @ 输出: RX 队列中可读字节数
 * @ 说明: 查询当前已接收但未被应用层读取的数据长度
 ****/
uint16_t Uart3_DMA_Available(void)
{
    return UartQueue_Count(&g_uart3_dma_rx_st.queue);
}

/********
 * @ 说明: 初始化 printf 重定向，禁用 stdout 行缓冲
 * @ 注意: 调用后 printf 输出将通过 fputc / __io_putchar 发送到 USART3
 ****/
void Uart_PrintfInit(void)
{
    setvbuf(stdout, NULL, _IONBF, 0);
}

/* uart usr end */

