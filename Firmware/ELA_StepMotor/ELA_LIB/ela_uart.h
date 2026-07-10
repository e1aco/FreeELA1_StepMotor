#ifndef _ELA_UART_H_
#define _ELA_UART_H_

#include "elaco_main.h"

#define DMA_BUF_SIZE 256

extern unsigned char g_rx3_buf[DMA_BUF_SIZE];
extern unsigned char g_rx3_offset;

void Uart_PrintfInit(void);

#endif
