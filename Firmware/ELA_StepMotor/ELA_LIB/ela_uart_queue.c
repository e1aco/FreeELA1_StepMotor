#include "ela_uart_queue.h"

/* uart_queue usr start */

/********
 * @ 说明: 初始化队列为空
 ****/
void UartQueue_Init(UART_QUEUE_T *me)
{
    me->head = 0;
    me->tail = 0;
    me->count = 0;
}

/********
 * @ 输入: UART_QUEUE_T *me: 队列指针; unsigned char byte: 写入字节
 * @ 输出: bool: true 成功, false 队列满
 * @ 说明: 向队尾写入一个字节
 ****/
bool UartQueue_Put(UART_QUEUE_T *me, unsigned char byte)
{
    if (me->count >= UART_QUEUE_SIZE)
    {
        return false;
    }
    me->buffer[me->head] = byte;
    me->head = (me->head + 1) % UART_QUEUE_SIZE;
    me->count++;
    return true;
}

/********
 * @ 输入: UART_QUEUE_T *me: 队列指针; unsigned char *byte: 读出字节
 * @ 输出: bool: true 成功, false 队列空
 * @ 说明: 从队头读出一个字节
 ****/
bool UartQueue_Get(UART_QUEUE_T *me, unsigned char *byte)
{
    if (0 == me->count)
    {
        return false;
    }
    *byte = me->buffer[me->tail];
    me->tail = (me->tail + 1) % UART_QUEUE_SIZE;
    me->count--;
    return true;
}

/********
 * @ 输入: UART_QUEUE_T *me: 队列指针;
 *        unsigned char *data: 源数据; unsigned short len: 写入长度
 * @ 输出: unsigned short: 实际写入字节数
 * @ 说明: 批量写入，适用于 ISR 中整帧入队
 ****/
unsigned short UartQueue_PutBuf(UART_QUEUE_T *me,
                                unsigned char *data,
                                unsigned short len)
{
    unsigned short i;
    unsigned short avail;

    avail = UART_QUEUE_SIZE - me->count;
    if (len > avail)
    {
        len = avail;
    }

    for (i = 0; i < len; i++)
    {
        me->buffer[me->head] = data[i];
        me->head = (me->head + 1) % UART_QUEUE_SIZE;
    }
    me->count += len;
    return len;
}

/********
 * @ 输入: UART_QUEUE_T *me: 队列指针;
 *        unsigned char *data: 目标缓冲区; unsigned short len: 读取长度
 * @ 输出: unsigned short: 实际读出字节数
 * @ 说明: 批量读取，适用于应用层整帧出队
 ****/
unsigned short UartQueue_GetBuf(UART_QUEUE_T *me,
                                unsigned char *data,
                                unsigned short len)
{
    unsigned short i;

    if (len > me->count)
    {
        len = me->count;
    }

    for (i = 0; i < len; i++)
    {
        data[i] = me->buffer[me->tail];
        me->tail = (me->tail + 1) % UART_QUEUE_SIZE;
    }
    me->count -= len;
    return len;
}

/********
 * @ 输出: bool: true 满, false 未满
 * @ 说明: 判断队列是否已满
 ****/
bool UartQueue_IsFull(UART_QUEUE_T *me)
{
    return (me->count >= UART_QUEUE_SIZE);
}

/********
 * @ 输出: bool: true 空, false 非空
 * @ 说明: 判断队列是否为空
 ****/
bool UartQueue_IsEmpty(UART_QUEUE_T *me)
{
    return (0 == me->count);
}

/********
 * @ 输出: unsigned short: 当前队列中的字节数
 * @ 说明: 返回队列中可读的字节数
 ****/
unsigned short UartQueue_Count(UART_QUEUE_T *me)
{
    return me->count;
}

/* uart_queue usr end */

