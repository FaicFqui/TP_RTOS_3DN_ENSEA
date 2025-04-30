#ifndef DRV_UART_H
#define DRV_UART_H

#include <stdint.h>
#include "FreeRTOS.h"

void drv_uart_set_task_handle(void *handle);
uint8_t drv_uart_receive(char *pData, uint16_t size);
uint8_t drv_uart_transmit(char *pData, uint16_t size);

extern char uart_rx_char;

#endif // DRV_UART_H
