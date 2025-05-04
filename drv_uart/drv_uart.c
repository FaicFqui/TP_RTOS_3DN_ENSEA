#include "drv_uart.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"

extern UART_HandleTypeDef huart1;

static TaskHandle_t rxTaskHandle = NULL;   // Tâche à notifier quand un caractère arrive
static uint8_t rxBuffer;                   // Buffer pour 1 caractère reçu

void drv_uart_init_IT(void) {
    // Lancement de la première réception en IT
    HAL_UART_Receive_IT(&huart1, &rxBuffer, 1);
}

uint8_t drv_uart_transmit(char *pData, uint16_t size) {
    return (HAL_UART_Transmit(&huart1, (uint8_t *)pData, size, HAL_MAX_DELAY) == HAL_OK) ? 0 : 1;
}

uint8_t drv_uart_receive(char *pData, uint16_t size) {
    if (size != 1) return 1;

    // Enregistrement la tâche courante pour notification
    rxTaskHandle = xTaskGetCurrentTaskHandle();

    // Attente de la notification de réception
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    *pData = rxBuffer;
    return 0;
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1 && rxTaskHandle != NULL) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        // Pour debug
        //HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_1);

        vTaskNotifyGiveFromISR(rxTaskHandle, &xHigherPriorityTaskWoken);
        rxTaskHandle = NULL;

        HAL_UART_Receive_IT(huart, &rxBuffer, 1);

        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}
