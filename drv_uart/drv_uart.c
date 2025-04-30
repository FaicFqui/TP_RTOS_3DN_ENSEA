#include "drv_uart.h"
#include "usart.h"
#include "FreeRTOS.h"
#include "task.h"

static TaskHandle_t uart_task_handle = NULL;
char uart_rx_char;

void drv_uart_set_task_handle(void *handle) {
	uart_task_handle = (TaskHandle_t)handle;
}

uint8_t drv_uart_receive(char *pData, uint16_t size) {
	// Attend que l'interruption notifie la tâche
	ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	*pData = uart_rx_char;
	return 1;
}

uint8_t drv_uart_transmit(char *pData, uint16_t size) {
	return (HAL_UART_Transmit(&huart1, (uint8_t *)pData, size, HAL_MAX_DELAY) == HAL_OK);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1 && uart_task_handle) {
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;

		vTaskNotifyGiveFromISR(uart_task_handle, &xHigherPriorityTaskWoken);

		// Redémarre la réception du caractère suivant
		HAL_UART_Receive_IT(&huart1, (uint8_t *)&uart_rx_char, 1);

		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
}
