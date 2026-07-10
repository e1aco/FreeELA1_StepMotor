/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id$
 */

#include "port.h"
#include "usart.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- static functions ---------------------------------*/
static void prvvUARTTxReadyISR(void);
static void prvvUARTRxISR(void);

/* ----------------------- Start implementation -----------------------------*/
void vMBPortSerialEnable(BOOL xRxEnable, BOOL xTxEnable)
{
    /* If xRXEnable enable serial receive interrupts. If xTxENable enable
     * transmitter empty interrupts.
     */
    if (xRxEnable)
    {
        // 如果需要修改为带片选的485，那么在这里拉低片选
        __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE); // 使能接收中断
    }
    else
    {
        __HAL_UART_DISABLE_IT(&huart1, UART_IT_RXNE); // 禁止接收中断
    }
    if (xTxEnable)
    {
        // 如果需要修改为带片选的485，那么在这里拉高片选
        __HAL_UART_ENABLE_IT(&huart1, UART_IT_TXE); // 使能发送中断
    }
    else
    {
        __HAL_UART_DISABLE_IT(&huart1, UART_IT_TXE); // 禁止发送中断
    }
}

BOOL xMBPortSerialInit(UCHAR ucPORT,
                       ULONG ulBaudRate,
                       UCHAR ucDataBits,
                       eMBParity eParity)
{
    huart1.Instance = USART1;
    huart1.Init.BaudRate = ulBaudRate;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    switch (eParity)
    {
    case MB_PAR_ODD:
        huart1.Init.Parity = UART_PARITY_ODD;
        huart1.Init.WordLength = UART_WORDLENGTH_9B;
        break;
    case MB_PAR_EVEN:
        huart1.Init.Parity = UART_PARITY_EVEN;
        huart1.Init.WordLength = UART_WORDLENGTH_9B;
        break;
    case MB_PAR_NONE:
        huart1.Init.Parity = UART_PARITY_NONE;
        huart1.Init.WordLength = UART_WORDLENGTH_8B;
        break;
    }
    return HAL_UART_Init(&huart1) == HAL_OK ? TRUE : FALSE;
}

BOOL xMBPortSerialPutByte(CHAR ucByte)
{
    /* Put a byte in the UARTs transmit buffer. This function is called
     * by the protocol stack if pxMBFrameCBTransmitterEmpty( ) has been
     * called. */
    USART1->DR = ucByte;
    return TRUE;
}

BOOL xMBPortSerialGetByte(CHAR *pucByte)
{
    /* Return the byte in the UARTs receive buffer. This function is called
     * by the protocol stack after pxMBFrameCBByteReceived( ) has been called.
     */
    *pucByte = (char)(USART1->DR & (uint16_t)0x00FF);
    return TRUE;
}

/* Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call
 * xMBPortSerialPutByte( ) to send the character.
 */
static void prvvUARTTxReadyISR(void)
{
    pxMBFrameCBTransmitterEmpty();
}

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
static void prvvUARTRxISR(void)
{
    pxMBFrameCBByteReceived();
}
void USART1_IRHandler(void)
{
    if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE))
    {
        __HAL_UART_CLEAR_FLAG(&huart1, UART_FLAG_RXNE);
        prvvUARTRxISR(); // 数据已接收完成
    }
    if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TXE) &&
        __HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC))
    {
        __HAL_UART_CLEAR_FLAG(&huart1, UART_FLAG_TXE);
        prvvUARTTxReadyISR(); //数据已发送完成
    }
}
