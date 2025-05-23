/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "semphr.h"
#include <stdio.h>
#include "shell.h"
#include "drv_uart.h"
#include <string.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "stm32f7xx_hal_tim.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

volatile uint32_t tim6_overflow_count = 0;
extern TIM_HandleTypeDef htim6;

int __io_putchar(int ch) {
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
return ch;
}





/*void LedCli(void *argument);

void LedCli(void *argument)
      {
        for(;;)
        {
          HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_1); // Inverse l'état de la LED
          vTaskDelay(pdMS_TO_TICKS(500));        // Attendre 500ms :: vTaskDelay(500 / portTICK_PERIOD_MS);
        }
      }*/

// Taches utilisant un sémaphore binaire
/*SemaphoreHandle_t sem;

void TaskGive(void *argument);
void TaskTake(void *argument);

void TaskGive(void *argument){
    TickType_t delay = 100; // Délai initial en ms

    for(;;)
    {
        printf("TaskGive: Avant de donner le sémaphore\n");

        if (xSemaphoreGive(sem) == pdPASS)
        {
            printf("TaskGive: Sémaphore donné\n");
        }

        vTaskDelay(pdMS_TO_TICKS(delay)); // Attendre delay ms
        delay += 100; // Ajoute 100ms à chaque itération
    }
}



void TaskTake(void *argument){
    static int erreur_count = 0;

    for(;;)
    {
        printf("TaskTake: Avant de prendre le sémaphore\n");

        if (xSemaphoreTake(sem, pdMS_TO_TICKS(1000)) == pdTRUE)
        {
            printf("TaskTake: Sémaphore pris\n");
            erreur_count = 0; // Reset du compteur d'erreur
        }
        else
        {
            erreur_count++;
            printf("TaskTake: Erreur d'acquisition #%d\n", erreur_count);

            if (erreur_count >= 3)
            {
                printf("TaskTake: Trop d'échecs. Redémarrage dans 5s...\n");
                vTaskDelay(pdMS_TO_TICKS(5000));
                NVIC_SystemReset(); // Redémarrage logiciel
            }
        }
    }
}*/


// Taches avec Notification
/*TaskHandle_t taskTakeHandle;

void TaskGive(void *argument){
    TickType_t delay = 100; // Délai initial en ms

    for(;;)
    {
        printf("TaskGive: Avant de notifier la tâche\n");

        // Envoie une notification à TaskTake
        xTaskNotifyGive(taskTakeHandle);

        printf("TaskGive: Notification envoyée\n");

        vTaskDelay(pdMS_TO_TICKS(delay));
        delay += 100;
    }
}

void TaskTake(void *argument){
    static int erreur_count = 0;

    for(;;)
    {
        printf("TaskTake: Avant de recevoir la notification\n");

        // Attend la notification (pendant 1 seconde max)
        if (ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(1000)) > 0)
        {
            printf("TaskTake: Notification reçue\n");
            erreur_count = 0;
        }
        else
        {
            erreur_count++;
            printf("TaskTake: Timeout #%d - pas de notification reçue\n", erreur_count);

            if (erreur_count >= 3)
            {
                printf("TaskTake: Trop d'échecs. Redémarrage dans 5s...\n");
                vTaskDelay(pdMS_TO_TICKS(5000));
                NVIC_SystemReset();
            }
        }
    }
}*/

//Tache avec mutex
/*SemaphoreHandle_t delay_mutex;

#define STACK_SIZE 256
#define TASK1_PRIORITY 1
#define TASK2_PRIORITY 2
#define TASK1_DELAY 1
#define TASK2_DELAY 2

void task_bug(void * pvParameters)
{
    int delay;

    for (;;)
    {
        // Prendre le mutex pour lire la valeur de delay de manière sécurisée
        if (xSemaphoreTake(delay_mutex, portMAX_DELAY) == pdTRUE)
        {
        	delay = (int) pvParameters;
            xSemaphoreGive(delay_mutex);
        }

        printf("Je suis %s et je m'endors pour %d ticks\r\n", pcTaskGetName(NULL), delay);
        vTaskDelay(delay);
    }
}
*/

extern UART_HandleTypeDef huart1;



h_shell_t shell;

int fonction(int argc, char ** argv)
{
	printf("Je suis une fonction bidon\r\n");

	printf("Nombre d'arguments : %d\r\n", argc);

	    for (int i = 0; i < argc; i++) {
	        printf("Arg[%d] = %s\r\n", i, argv[i]);
	    }



	return 0;
}

int addition(int argc, char ** argv)
{
    if (argc < 3) {
        printf("Erreur : 2 paramètres requis\r\n");
        return -1;
    }

    int a = atoi(argv[1]);
    int b = atoi(argv[2]);
    int result = a + b;

    printf("Addition : %d + %d = %d\r\n", a, b, result);

    return 0;
}

// allumer une led en dure, à améliorer pour choisir la led et le port, éteintre la led
int allume_led(int argc, char **argv) {
    HAL_GPIO_WritePin(GPIOI, GPIO_PIN_1, GPIO_PIN_SET);
    printf("LED PIN1 GPIOI est allumee !\r\n");
    return 0;
}

int eteint_led(int argc, char **argv) {
    HAL_GPIO_WritePin(GPIOI, GPIO_PIN_1, GPIO_PIN_RESET);
    printf("LED PIN1 GPIOI est eteinte !\r\n");
    return 0;
}


/// fonctions pour statistiques
void configureTimerForRunTimeStats(void) {
    HAL_TIM_Base_Start_IT(&htim6);  // Lancer TIM6 avec IT
    tim6_overflow_count = 0;
}

unsigned long getRunTimeCounterValue(void) {
    return (tim6_overflow_count << 16) + __HAL_TIM_GET_COUNTER(&htim6);
}


void TacheStats(void *pvParameters) {
    char buffer[512];

    for (;;) {
        vTaskGetRunTimeStats(buffer);
        printf("=== RunTime Stats ===\n%s\n", buffer);
        vTaskDelay(pdMS_TO_TICKS(2000)); // Toutes les 2 secondes
    }
}




void ShellTask(void *argument) {
    shell.io.drv_shell_transmit = drv_uart_transmit;
    shell.io.drv_shell_receive  = drv_uart_receive;

    drv_uart_init_IT();  // // Lancement de la première réception en IT

    shell_init(&shell);
    shell_add(&shell, 'f', fonction, "Une fonction inutile");
    shell_add(&shell, 'a', addition, "Addition de deux entiers");
    shell_add(&shell, 'l', allume_led, "Allumer LED (GPIOI_PIN1)");
    shell_add(&shell, 'e', eteint_led, "Eteindre LED (GPIOI_PIN1)");
    shell_add(&shell, 's', TacheStats, "Statitiques taches/CPU/MEMOIRE");

    shell_run(&shell);  // éxécute une boucle while(1)
}


////// ******************* Gestion du Tas ********************///////

/* Tache bidon */
void tache_bidon(void *pvParameters) {
    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/* Création répétée de tâches pour tester la limite mémoire */
void cree_plusieurs_taches(void) {
    BaseType_t result;
    int i = 1;

    while (1) {
        char name[16];
        snprintf(name, sizeof(name), "D%d", i);

        result = xTaskCreate(tache_bidon, name, 512, NULL, 1, NULL);
        if (result != pdPASS) {
            printf(" Tache %d : echec de creation (peut etre memoire epuisee)\n", i);
            Error_Handler();
            break;
        } else {
            printf(" Tache %d creee avec succes | Heap restant : %u octets\n",
                   i, (unsigned int)xPortGetFreeHeapSize());
        }

        i++;
    }
}


void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {

    printf(" Dépassement de pile detecte dans la tache : %s\n", pcTaskName);


    taskDISABLE_INTERRUPTS();
    while (1); //pour debbuger
}

void TacheOverflow(void *pvParameters) {
	printf("je rentre");
	UBaseType_t remaining = uxTaskGetStackHighWaterMark(NULL);
	printf("Pile restante avant recursion : %lu mots (%lu octets)\n",remaining, remaining * sizeof(StackType_t));

	vTaskDelay(pdMS_TO_TICKS(10));
	//Fonction récursive sans fin
    void Recursif(int niveau) {
        volatile char buffer[128]; // Alloue 128 octets à chaque appel
        buffer[0] = (char)niveau;  // Utilise la variable
        Recursif(niveau + 1);      // Appel récursif
        printf("recursif");
    }

    Recursif(0);

    vTaskDelete(NULL);
    printf("fin recursif");
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */


  //printf("Test UART transmission\n");

  // Pour les tâches avec le sémaphore

  /*	  sem = xSemaphoreCreateBinary();
      // Vérifier si le sémaphore a été créé avec succès
      if (sem == NULL)
      {
          printf("Erreur de création du sémaphore\n");
          while(1);
      }*/

  /* Création de la tâche */
    //xTaskCreate(LedCli, "LedTask", 128, NULL, 1, NULL);

  // Pour les tâches avec le sémaphore
  /*
  xTaskCreate(TaskGive, "TaskGive", 128, NULL, 1, NULL);
  xTaskCreate(TaskTake, "TaskTake", 128, NULL, 2, NULL);*/

   // Taches avec Notification
  /*xTaskCreate(TaskTake, "TaskTake", 128, NULL, 2, &taskTakeHandle);
  xTaskCreate(TaskGive, "TaskGive", 128, NULL, 1, NULL);*/

  //Tache avec Mutex
  /*BaseType_t ret;
  delay_mutex = xSemaphoreCreateMutex();
  configASSERT(delay_mutex != NULL);

  ret = xTaskCreate(task_bug, "Tache 1", STACK_SIZE, (void *) TASK1_DELAY, TASK1_PRIORITY, NULL);
  configASSERT(pdPASS == ret);
  ret = xTaskCreate(task_bug, "Tache 2", STACK_SIZE, (void *) TASK2_DELAY, TASK2_PRIORITY, NULL);
  configASSERT(pdPASS == ret);*/





  xTaskCreate(ShellTask, "Shell", 512, NULL, 1, NULL);

  //xTaskCreate(TacheStats,"TacheStats", 512, NULL, 1, NULL); // est appelable depuis Shell également avec "s"



  //printf("Hello\n");

  //Création de la tâche avec pile trop petite
     //xTaskCreate(TacheOverflow, "OVERFLOW", 128, NULL, 1, NULL);

  //printf("tache cree\n");

  //Pour la création plusieurs tache pour generer un dépassement de mémoire
  //cree_plusieurs_taches();



	vTaskStartScheduler();

  // Si le scheduler échoue, on arrive ici
	printf("Erreur : démarrage du scheduler echoue\n");





  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* Call init function for freertos objects (in cmsis_os2.c) */
  MX_FREERTOS_Init();

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  // Clignotement LED1
	  //HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_1); // Pour LED1


	  //HAL_Delay(500); // Délai de 500ms
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 432;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
    tim6_overflow_count++;
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
