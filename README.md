# TP_RTOS_3DN_ENSEA

Clignotement led sans FreeRtos et avec FreeRTOS est fonctionnel

Source de temps pour FreeRTOS est TIM6

Manipulation des sémaphores, notifications, queues est effectuée.

Solution sémaphore Mutex proposé pour proteger l'accès à la fonction task_bug

Compilation et intégration du shell sans son utilisation dans une tache FreeRTOS est fonctionnel :
affichage console RealTerm :

===== Monsieur Shell v0.2 =====
                                               
> ff
                                                                          
                                                                           
Je suis une fonction bidon

*****************
Chaque unité dev citée au-dessus est tracée par des commit sur le repo GitHub.

Dans usart.c :

HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);
HAL_NVIC_EnableIRQ(USART1_IRQn);

Dans FreeRTOSConfig.h : #define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 5

Création de paire drv_uart.c et .h dans lesquel on délègue une partie du travail qui était fait dasn shell.c. : 
Transmission uart - réception uart en interruption

Intégration de la fonction shell_run() dans une tache RTOS crée dans le main :

void ShellTask(void *argument) {
	drv_uart_set_task_handle(xTaskGetCurrentTaskHandle());
	HAL_UART_Receive_IT(&huart1, (uint8_t *)&uart_rx_char, 1);

	shell_init();
	shell_add('f', fonction, "Une fonction inutile");

	shell_run();  // Boucle bloquante
}

xTaskCreate(ShellTask, "Shell", 512, NULL, tskIDLE_PRIORITY + 1, NULL);
 
