#ifndef _ELA_UART_QUEUE_H_
#define _ELA_UART_QUEUE_H_

#define UART_QUEUE_SIZE 256

#define FALSE 1
#define TRUE  0

typedef struct UART_QUEUE
{
    unsigned char buffer[UART_QUEUE_SIZE];
    unsigned short head;   // 写入位置
    unsigned short tail;   // 读取位置
    unsigned short count;  // 当前字节数
} UART_QUEUE_T;

void          UartQueue_Init(UART_QUEUE_T *me);
unsigned char UartQueue_Put(UART_QUEUE_T *me, unsigned char byte);
unsigned char UartQueue_Get(UART_QUEUE_T *me, unsigned char *byte);
unsigned short UartQueue_PutBuf(UART_QUEUE_T *me,
                                unsigned char *data,
                                unsigned short len);
unsigned short UartQueue_GetBuf(UART_QUEUE_T *me,
                                unsigned char *data,
                                unsigned short len);
unsigned char UartQueue_IsFull(UART_QUEUE_T *me);
unsigned char UartQueue_IsEmpty(UART_QUEUE_T *me);
unsigned short UartQueue_Count(UART_QUEUE_T *me);

#endif
