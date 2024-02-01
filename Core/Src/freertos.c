/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "CommunicationProtocol/gpio.h"
#include "CommunicationProtocol/i2c.h"
#include "CommunicationProtocol/tim.h"
#include "Components/MPU6050.h"
#include "Components/Motor.h"
#include "stm32f4xx_hal_tim.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "CommunicationProtocol/usart.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
TaskHandle_t defaultTaskHandle = NULL;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  xTaskCreate(
    StartDefaultTask,          // Task function
    "DefaultTask",             // Name of the task (for debugging purposes)
    128,                       // Stack size (in words, not bytes!)
    NULL,                      // Task input parameter
    tskIDLE_PRIORITY + 1,      // Priority of the task
    &defaultTaskHandle         // Task handle
  );

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(const void* argument) {
  MX_USART2_UART_Init();
  MX_I2C1_Init();

  MX_GPIO_Init();
  while (true)
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
    
  MX_TIM3_Init();
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);

  MX_TIM2_Init();
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);

  MX_TIM4_Init();
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);

  MX_TIM5_Init();
  HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);

  for(;;)
  {
    tuple thermo = ReadAccel();
    char buffer[32]; // Adjust the size as needed

    // Convert thermo.x to string and send
    snprintf(buffer, sizeof(buffer), "%d\r\n", thermo.x);
    HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);

    // Convert thermo.y to string and send
    snprintf(buffer, sizeof(buffer), "%d\r\n", thermo.y);
    HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);

    // Convert thermo.z to string and send
    snprintf(buffer, sizeof(buffer), "%d\r\n\n", thermo.z);
    HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);

    osDelay(1);
    HAL_Delay(1000);
  }
}
