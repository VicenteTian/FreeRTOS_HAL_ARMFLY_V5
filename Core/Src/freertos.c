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
#define BIT_0 (1 << 0)
#define BIT_1 (1 << 1)
#define BIT_ALL (BIT_0 | BIT_1)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
extern uint32_t g_osRuntimeCounter;
EventGroupHandle_t xCreatedEventGroup = NULL;
/* USER CODE END Variables */
osThreadId LEDHandle;
osThreadId MsgProHandle;
osThreadId UserIFHandle;
osThreadId StartHandle;
osMessageQId CmdQueueHandle;
/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
static void AppObjCreate(void);
/* USER CODE END FunctionPrototypes */

void StartTaskLED(void const * argument);
void StartMsgProTask(void const * argument);
void StartTaskUserIF(void const * argument);
void StartTaskStart(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* GetTimerTaskMemory prototype (linked to static allocation support) */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize );

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
/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize)
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/* USER CODE BEGIN GET_TIMER_TASK_MEMORY */
static StaticTask_t xTimerTaskTCBBuffer;
static StackType_t xTimerStack[configTIMER_TASK_STACK_DEPTH];

void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize)
{
  *ppxTimerTaskTCBBuffer = &xTimerTaskTCBBuffer;
  *ppxTimerTaskStackBuffer = &xTimerStack[0];
  *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
  /* place for user code */
}
/* USER CODE END GET_TIMER_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  AppObjCreate();
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

  /* Create the queue(s) */
  /* definition and creation of CmdQueue */
  osMessageQDef(CmdQueue, 20, uint8_t);
  CmdQueueHandle = osMessageCreate(osMessageQ(CmdQueue), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of LED */
  osThreadDef(LED, StartTaskLED, osPriorityBelowNormal, 0, 128);
  LEDHandle = osThreadCreate(osThread(LED), NULL);

  /* definition and creation of MsgPro */
  osThreadDef(MsgPro, StartMsgProTask, osPriorityLow, 0, 128);
  MsgProHandle = osThreadCreate(osThread(MsgPro), NULL);

  /* definition and creation of UserIF */
  osThreadDef(UserIF, StartTaskUserIF, osPriorityNormal, 0, 128);
  UserIFHandle = osThreadCreate(osThread(UserIF), NULL);

  /* definition and creation of Start */
  osThreadDef(Start, StartTaskStart, osPriorityAboveNormal, 0, 128);
  StartHandle = osThreadCreate(osThread(Start), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartTaskLED */
/**
 * @brief  Function implementing the LED thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTaskLED */
void StartTaskLED(void const * argument)
{
  /* USER CODE BEGIN StartTaskLED */
  RTCDateTime mRTCDateTime;
  /*   uint8_t uResult = pdFALSE;
    uint8_t uQueueMessage = 0; */
  // TickType_t xLastWakeTime;
  // const TickType_t xFrequency = 200;
  /* ??????????????????*/
  // xLastWakeTime = xTaskGetTickCount();
  /* Infinite loop */
  for (;;)
  {
    /* ??????????*/
    // taskENTER_CRITICAL();
    // vTaskSuspendAll();
    // printf("???? vTaskLED ????????\r\n");
    /* ??????????*/
    // taskEXIT_CRITICAL();
    /*    uResult = xQueueReceive(CmdQueueHandle, (void *)&uQueueMessage, 1000);
       if (uResult == pdTRUE)
       {
         printf("????%d??????\n", uQueueMessage);
       } */
    mRTCDateTime = GetRTCDateTime();
    printf("%d-%d-%d,%d:%d:%d",
           mRTCDateTime.RTCDate.Year + 2000,
           mRTCDateTime.RTCDate.Month,
           mRTCDateTime.RTCDate.Date,
           mRTCDateTime.RTCTime.Hours,
           mRTCDateTime.RTCTime.Minutes,
           mRTCDateTime.RTCTime.Seconds);
    osDelay(1000);
    /*     HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
        osDelay(200); */
    /* vTaskDelayUntil ???????????? vTaskDelay ????????????*/
    // vTaskDelayUntil(&xLastWakeTime, xFrequency);
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
void StartMsgProTask(void const * argument)
{
  /* USER CODE BEGIN StartMsgProTask */
  // EventBits_t uxBits;
  // const TickType_t xTicksToWait = 100 / portTICK_PERIOD_MS; /* ????????100ms */
  /*   uint8_t var[2] = {0, 255}; */
  /* Infinite loop */
  for (;;)
  {
    /*     vcan_sendware((uint8_t *)var, sizeof(var)); */
    /* ??????????*/
    // taskENTER_CRITICAL();
    // vTaskSuspendAll(); /* ??????????*/
    // printf("???? vTaskMsgPro ????????\r\n");
    /* ??????????*/
    // taskEXIT_CRITICAL();
    // if (!xTaskResumeAll()) /* ???????????????????????????????????????? pdTRUE ?????????? pdFALSE */
    // taskYIELD();
    /* ??K2????????????bit0??K3????????????bit1 */
    if (recv_end_flag == 1) //????????????
    {
      // HAL_UART_Transmit_DMA(&huart1, rx_buffer, rx_len);
      xQueueSend(CmdQueueHandle, (void *)&rx_len, 10);
      vRtcCmd(rx_buffer);
      memset(rx_buffer, 0, rx_len);
      rx_len = 0;                                            //????????
      recv_end_flag = 0;                                     //??????????????????
      HAL_UART_Receive_DMA(&huart1, rx_buffer, BUFFER_SIZE); //????????DMA????
    }
    osDelay(20);
    /*     var[0]++;
    var[1]--; */
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
void StartTaskUserIF(void const * argument)
{
  /* USER CODE BEGIN StartTaskUserIF */
  uint8_t ucKeyCode;
  uint8_t pcWriteBuffer[150]; //????????????????????????
  EventBits_t uxBits;
  /* Infinite loop */
  for (;;)
  {
    ucKeyCode = bsp_GetKey();

    if (ucKeyCode != KEY_NONE)
    {
      switch (ucKeyCode)
      {
      // K1??????????????????????
      case KEY_DOWN_K1:
        printf("=================================================\r\n");
        printf("??????       ???????? ??????  ?????? ????????\r\n");
        vTaskList((char *)&pcWriteBuffer);
        printf("%s\r\n", pcWriteBuffer);
        printf("??????       ????????         ??????\r\n");
        vTaskGetRunTimeStats((char *)&pcWriteBuffer);
        printf("%s\r\n", pcWriteBuffer);
        break;
        // K2??????????vTaskLED????
      case KEY_DOWN_K2:
        uxBits = xEventGroupSetBits(xCreatedEventGroup, BIT_0);
        printf("K2??????%d\r\n", uxBits);
        break;
        /* K2 ?????????????????????????????? vTaskMsgPro ?????? bit0 */

        /*         if (LEDHandle != NULL)
        {
          printf("K2 ?????????????? vTaskLED\r\n");
          vTaskDelete(LEDHandle);
          LEDHandle = NULL;
        }
        else
        {
          printf("vTaskLED ??????\r\n");
        }

        break;*/

        // K3??????????????vTaskLED????
      case KEY_DOWN_K3:
        /* K3??????????????????????????????vTaskMsgPro??????bit1 */
        uxBits = xEventGroupSetBits(xCreatedEventGroup, BIT_1);
        printf("K3??????%d\r\n", uxBits);
        break;
        /*         if (LEDHandle == NULL)
        {
          printf("K3 ?????????????????? vTaskLED\r\n");
          osThreadDef(LED, StartTaskLED, osPriorityBelowNormal, 0, 128);
          LEDHandle = osThreadCreate(osThread(LED), NULL);
        }
        else
        {
          printf("vTaskLED ??????\r\n");
        }
        break;
 */
        /* ????UP????????????????vTaskLED */
      case JOY_DOWN_U:
        if (LEDHandle == NULL)
        {
          printf("?????? vTaskLED\r\n");
        }
        else
        {
          printf("UP ?????????????? vTaskLED\r\n");
          vTaskSuspend(LEDHandle);
        }
        break;

      /* ????DOWN????????????????vTaskLED */
      case JOY_DOWN_D:
        if (LEDHandle == NULL)
        {
          printf("?????? vTaskLED\r\n");
        }
        else
        {
          printf("DOWN ?????????????? vTaskLED\r\n");
          vTaskResume(LEDHandle);
        }
        break;

      /* ???????????????? */
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
void StartTaskStart(void const * argument)
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
void vApplicationStackOverflowHook(TaskHandle_t xTask, signed char *pcTaskName)
{
  printf("\n?????? ??%s ??????????\n", pcTaskName);
}
/**
 * @description: ????????????????
 * @param {*}
 * @return {*}
 */
static void AppObjCreate(void)
{ /* ??????????????*/
  xCreatedEventGroup = xEventGroupCreate();
  if (xCreatedEventGroup == NULL)
  {
    /* ???????? ??????????????????????????????????????????*/
    printf("??????????????????\n");
  }
  else
  {
    printf("??????????????????\n");
  }
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
