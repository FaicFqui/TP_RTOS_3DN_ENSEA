#ifndef INC_DRV_UART_H_
#define INC_DRV_UART_H_

#include <stdint.h>


uint8_t drv_uart_transmit(char *pData, uint16_t size);

uint8_t drv_uart_receive(char *pData, uint16_t size);

void drv_uart_init_IT(void);

#endif /* INC_DRV_UART_H_ */
