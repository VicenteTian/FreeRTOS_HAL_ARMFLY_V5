/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "UART.h"
#include "key.h"
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
extern uint32_t g_osRuntimeCounter;
/* USER CODE END Variables */
/* Definitions for LED */
osThreadId_t LEDHandle;
const osThreadAttr_t LED_attributes = {
    .name = "LED",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityLow2,
};
/* Definitions for MsgPro */
osThreadId_t MsgProHandle;
const osThreadAttr_t MsgPro_attributes = {
    .name = "MsgPro",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityLow5,
};
/* Definitions for UserIF */
osThreadId_t UserIFHandle;
const osThreadAttr_t UserIF_attributes = {
    .name = "UserIF",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityLow3,
};
/* Definitions for Start */
osThreadId_t StartHandle;
const osThreadAttr_t Start_attributes = {
    .name = "Start",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartTaskLED(void *argument);
void StartMsgProTask(void *argument);
void StartTaskUserIF(void *argument);
void StartTaskStart(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void configureTimerForRunTimeStats(void);
unsigned long getRunTimeCounterValue(void);

/* USER CODE BEGIN 1 */
/* Functions needed when configGENERATE_RUN_TIME_STATS is on */
__weak void configureTimerForRunTimeStats(void)
{
  g_osRuntimeCounter = 0;
}

__weak unsigned long getRunTimeCounterValue(void)
{
  return g_osRuntimeCounter;
}
/* USER CODE END 1 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void)
{
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
  /* creation of LED */
  LEDHandle = osThreadNew(StartTaskLED, NULL, &LED_attributes);

  /* creation of MsgPro */
  MsgProHandle = osThreadNew(StartMsgProTask, NULL, &MsgPro_attributes);

  /* creation of UserIF */
  UserIFHandle = osThreadNew(StartTaskUserIF, NULL, &UserIF_attributes);

  /* creation of Start */
  StartHandle = osThreadNew(StartTaskStart, NULL, &Start_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */
}

/* USER CODE BEGIN Header_StartTaskLED */
/**
  * @brief  Function implementing the LED thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartTaskLED */
void StartTaskLED(void *argument)
{
  /* USER CODE BEGIN StartTaskLED */
  /* Infinite loop */
  for (;;)
  {
    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
    osDelay(200);
  }
  /* USER CODE END StartTaskLED */
}

/* USER CODE BEGIN Header_StartMsgProTask */
/**
* @brief Function implementing the MsgPro thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartMsgProTask */
void StartMsgProTask(void *argument)
{
  /* USER CODE BEGIN StartMsgProTask */
  uint8_t var[2] = {0, 255};
  /* Infinite loop */
  for (;;)
  {
    vcan_sendware((uint8_t *)var, sizeof(var));
    osDelay(200);
    var[0]++;
    var[1]--;
  }
  /* USER CODE END StartMsgProTask */
}

/* USER CODE BEGIN Header_StartTaskUserIF */
/**
* @brief Function implementing the UserIF thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskUserIF */
void StartTaskUserIF(void *argument)
{
  /* USER CODE BEGIN StartTaskUserIF */
  uint8_t ucKeyCode;
  uint8_t pcWriteBuffer[200]; //该数组过大会导致程序卡死
  /* Infinite loop */
  for (;;)
  {
    ucKeyCode = bsp_GetKey();

    if (ucKeyCode != KEY_NONE)
    {
      switch (ucKeyCode)
      {
      //K1按下，打印任务执行情况
      case KEY_DOWN_K1:
        printf("=================================================\n");
        printf("任务名             任务状态  优先级    剩余栈    任务序号\n");
        vTaskList((char *)&pcWriteBuffer);
        printf("%s\n", pcWriteBuffer);
        printf("任务名             运行计数       使用率\n");
        vTaskGetRunTimeStats((char *)&pcWriteBuffer);
        printf("%s\n", pcWriteBuffer);
        break;
        //K2按下，删除vTaskLED任务
      case KEY_DOWN_K2:
        printf("K2 按下，删除任务 vTaskLED\n");
        if (LEDHandle != NULL)
        {
          vTaskDelete(LEDHandle);
          LEDHandle = NULL;
        }
        break;

        //K3按下，重新创建vTaskLED任务
      case KEY_DOWN_K3:

        if (LEDHandle == NULL)
        {
          printf("K3 按下，重新创建任务 vTaskLED\n");
          LEDHandle = osThreadNew(StartTaskLED, NULL, &LED_attributes);
        }
        else
        {
          printf("vTaskLED 已存在\n");
        }
        break;

        /* 摇杆UP键按下，挂起任务vTaskLED */
      case JOY_DOWN_U:
        if (LEDHandle == NULL)
        {
          printf("无任务 vTaskLED\n");
        }
        else
        {
          printf("UP 按下，挂起任务 vTaskLED\n");
          vTaskSuspend(LEDHandle);
        }
        break;

      /* 摇杆DOWN键按下，恢复任务vTaskLED */
      case JOY_DOWN_D:
        if (LEDHandle == NULL)
        {
          printf("无任务 vTaskLED\n");
        }
        else
        {
          printf("DOWN 按下，恢复任务 vTaskLED\n");
          vTaskResume(LEDHandle);
        }
        break;

      /* 其他的键值不处理 */
      default:
        break;
      }
    }
    osDelay(20);
  }
  /* USER CODE END StartTaskUserIF */
}

/* USER CODE BEGIN Header_StartTaskStart */
/**
* @brief Function implementing the Start thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskStart */
void StartTaskStart(void *argument)
{
  /* USER CODE BEGIN StartTaskStart */
  /* Infinite loop */
  for (;;)
  {
    bsp_KeyScan();
    osDelay(10);
  }
  /* USER CODE END StartTaskStart */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
