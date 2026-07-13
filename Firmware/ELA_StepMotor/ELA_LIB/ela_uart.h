#ifndef _ELA_UART_H_
#define _ELA_UART_H_

#include "elaco_main.h"
#include "ela_uart_queue.h"

#include "usart.h"
#include <stdio.h>

#define UART_DMA_RX_BUF_SIZE 256
#define UART_DMA_TX_BUF_SIZE 256

typedef struct UART_DMA_TX {
    UART_QUEUE_T  queue;
    volatile bool dma_busy;
    uint8_t       dma_buf[UART_DMA_TX_BUF_SIZE];
} UART_DMA_TX_T;

typedef struct UART_DMA_RX {
    UART_QUEUE_T queue;
    uint8_t      dma_buf[UART_DMA_RX_BUF_SIZE];
} UART_DMA_RX_T;

extern UART_DMA_RX_T g_uart3_dma_rx_st;
extern UART_DMA_TX_T g_uart3_dma_tx_st;

void     Uart3_DMA_Init(void);
uint16_t Uart3_DMA_SendBuf(uint8_t *data, uint16_t len);
bool     Uart3_DMA_SendByte(uint8_t byte);
uint16_t Uart3_DMA_RecvBuf(uint8_t *data, uint16_t len);
bool     Uart3_DMA_RecvByte(uint8_t *byte);
uint16_t Uart3_DMA_Available(void);
void     Uart3_DMA_RxIdleHandler(void);
void     Uart3_DMA_TxContinue(void);

void     Uart_PrintfInit(void);

#endif

