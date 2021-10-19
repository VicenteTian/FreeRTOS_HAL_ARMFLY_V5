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

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
static void AppObjCreate(void);
/* USER CODE END FunctionPrototypes */

void StartTaskLED(void const *argument);
void StartMsgProTask(void const *argument);
void StartTaskUserIF(void const *argument);
void StartTaskStart(void const *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize);

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

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void)
{
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

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of LED */
  osThreadDef(LED, StartTaskLED, osPriorityBelowNormal, 0, 128);
  LEDHandle = osThreadCreate(osThread(LED), NULL);

  /* definition and creation of MsgPro */
  osThreadDef(MsgPro, StartMsgProTask, osPriorityNormal, 0, 128);
  MsgProHandle = osThreadCreate(osThread(MsgPro), NULL);

  /* definition and creation of UserIF */
  osThreadDef(UserIF, StartTaskUserIF, osPriorityLow, 0, 128);
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
void StartTaskLED(void const *argument)
{
  /* USER CODE BEGIN StartTaskLED */
  //TickType_t xLastWakeTime;
  //const TickType_t xFrequency = 200;
  /* ��ȡ��ǰ��ϵͳʱ��*/
  //xLastWakeTime = xTaskGetTickCount();
  /* Infinite loop */
  for (;;)
  {
    /* �����ٽ���*/
    //taskENTER_CRITICAL();
    //vTaskSuspendAll();
    //printf("���� vTaskLED ��������\r\n");
    /* �˳��ٽ���*/
    //taskEXIT_CRITICAL();

    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
    osDelay(200);
    /* vTaskDelayUntil �Ǿ����ӳ٣� vTaskDelay ������ӳ١�*/
    //vTaskDelayUntil(&xLastWakeTime, xFrequency);
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
void StartMsgProTask(void const *argument)
{
  /* USER CODE BEGIN StartMsgProTask */
  EventBits_t uxBits;
  const TickType_t xTicksToWait = 100 / portTICK_PERIOD_MS; /* ����ӳ�100ms */
  /*   uint8_t var[2] = {0, 255}; */
  /* Infinite loop */
  for (;;)
  {
    /*     vcan_sendware((uint8_t *)var, sizeof(var)); */
    /* �����ٽ���*/
    //taskENTER_CRITICAL();
    //vTaskSuspendAll(); /* ����������*/
    //printf("���� vTaskMsgPro ��������\r\n");
    /* �˳��ٽ���*/
    //taskEXIT_CRITICAL();
    //if (!xTaskResumeAll()) /* �رյ������������Ҫ�����л����˺������� pdTRUE �����򷵻� pdFALSE */
    //taskYIELD();
    /* ��K2������������bit0��K3������������bit1 */
    uxBits = xEventGroupWaitBits(xCreatedEventGroup, /* �¼���־���� */
                                 BIT_ALL,            /* �ȴ�bit0��bit1������ */
                                 pdTRUE,             /* �˳�ǰbit0��bit1�������������bit0��bit1�������òű�ʾ���˳���*/
                                 pdTRUE,             /* ����ΪpdTRUE��ʾ�ȴ�bit1��bit0��������*/
                                 xTicksToWait);      /* �ȴ��ӳ�ʱ�� */
    if ((uxBits & BIT_ALL) == BIT_ALL)
    {
      /* ���յ�bit1��bit0�������õ���Ϣ */
      printf("MsgProTask Run\r\n");
    }
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
void StartTaskUserIF(void const *argument)
{
  /* USER CODE BEGIN StartTaskUserIF */
  uint8_t ucKeyCode;
  uint8_t pcWriteBuffer[150]; //���������ᵼ�³�����
  EventBits_t uxBits;
  /* Infinite loop */
  for (;;)
  {
    ucKeyCode = bsp_GetKey();

    if (ucKeyCode != KEY_NONE)
    {
      switch (ucKeyCode)
      {
      //K1���£���ӡ����ִ�����
      case KEY_DOWN_K1:
        printf("=================================================\r\n");
        printf("������       ����״̬ ���ȼ�  ʣ��ջ �������\r\n");
        vTaskList((char *)&pcWriteBuffer);
        printf("%s\r\n", pcWriteBuffer);
        printf("������       ���м���         ʹ����\r\n");
        vTaskGetRunTimeStats((char *)&pcWriteBuffer);
        printf("%s\r\n", pcWriteBuffer);
        break;
        //K2���£�ɾ��vTaskLED����
      case KEY_DOWN_K2:
        uxBits = xEventGroupSetBits(xCreatedEventGroup, BIT_0);
        printf("K2������%d\r\n", uxBits);
        break;
        /* K2 �����£�ֱ�ӷ����¼���־������ vTaskMsgPro ������ bit0 */

        /*         if (LEDHandle != NULL)
        {
          printf("K2 ���£�ɾ������ vTaskLED\r\n");
          vTaskDelete(LEDHandle);
          LEDHandle = NULL;
        }
        else
        {
          printf("vTaskLED ������\r\n");
        }
 
        break;*/

        //K3���£����´���vTaskLED����
      case KEY_DOWN_K3:
        /* K3�����£�ֱ�ӷ����¼���־������vTaskMsgPro������bit1 */
        uxBits = xEventGroupSetBits(xCreatedEventGroup, BIT_1);
        printf("K3������%d\r\n", uxBits);
        break;
        /*         if (LEDHandle == NULL)
        {
          printf("K3 ���£����´������� vTaskLED\r\n");
          osThreadDef(LED, StartTaskLED, osPriorityBelowNormal, 0, 128);
          LEDHandle = osThreadCreate(osThread(LED), NULL);
        }
        else
        {
          printf("vTaskLED �Ѵ���\r\n");
        }
        break;
 */
        /* ҡ��UP�����£���������vTaskLED */
      case JOY_DOWN_U:
        if (LEDHandle == NULL)
        {
          printf("������ vTaskLED\r\n");
        }
        else
        {
          printf("UP ���£��������� vTaskLED\r\n");
          vTaskSuspend(LEDHandle);
        }
        break;

      /* ҡ��DOWN�����£��ָ�����vTaskLED */
      case JOY_DOWN_D:
        if (LEDHandle == NULL)
        {
          printf("������ vTaskLED\r\n");
        }
        else
        {
          printf("DOWN ���£��ָ����� vTaskLED\r\n");
          vTaskResume(LEDHandle);
        }
        break;

      /* �����ļ�ֵ������ */
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
void StartTaskStart(void const *argument)
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
  printf("\n���� ��%s ����ջ���\n", pcTaskName);
}
/**
 * @description: ��������ͨ�Ż���
 * @param {*}
 * @return {*}
 */
static void AppObjCreate(void)
{ /* �����¼���־��*/
  xCreatedEventGroup = xEventGroupCreate();
  if (xCreatedEventGroup == NULL)
  {
    /* û�д��� �ɹ����û�������������봴��ʧ�ܵĴ������*/
    printf("�¼���־�鴴��ʧ��\n");
  }
  else
  {
    printf("�¼���־�鴴���ɹ�\n");
  }
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
