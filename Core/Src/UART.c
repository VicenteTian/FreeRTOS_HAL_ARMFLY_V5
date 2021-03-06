#include "UART.h"
#include "usart.h"

uint8_t *OpenString[LEDNUM] = {"OpenLED1", "OpenLED2", "OpenLED3"};
uint8_t *CloseString[LEDNUM] = {"CloseLED1", "CloseLED2", "CloseLED3"};
/*!
 *  @brief      串口发送一个字节
 *  @param      UART_HandleTypeDef *huart   串口指针（如&huart1）
 *  @param      ch          需要发送的字节
 *  Sample usage:       uart_putchar(&huart1,'5');通过UART1发送字符5
 */
void uart_putchar(UART_HandleTypeDef *huart, char ch)
{
  uint8_t *addr = (uint8_t *)&ch;
  HAL_UART_Transmit(huart, addr, 1, 100);
  while (__HAL_UART_GET_FLAG(huart, UART_FLAG_TC) != 1)
    ; //等待发送完成
}
/*!
 *  @brief      串口接收一个字节
 *  @param      UART_HandleTypeDef *huart   串口指针（如&huart1）
 *  @param      ch          接收数据存放的地址
 *  Sample usage:       uart_getchar(&huart2,&c);;通过UART12接收一个字符存放至c字符变量中
 */
void uart_getchar(UART_HandleTypeDef *huart, char *ch)
{
  if (HAL_UART_Receive(huart, (uint8_t *)ch, 1, 5) == HAL_ERROR)
  {
    Error_Handler();
  }
}
/*!
 *  @brief      查询接收1个字符
 *  @param      UART_HandleTypeDef *huart   串口指针（如&huart1）
 *  @param      ch          接收地址
 *  @return     1为接收成功，0为接收失败
 *  Sample usage:       char ch ;
                        if( uart_querychar (&huart2,&ch))     //查询接收1个字符，保存到 ch里
                        {
                            printf("成功接收到一个字节");
                        }
 */
uint8_t uart_querychar(UART_HandleTypeDef *huart, char *ch)
{
  char str = 0;
  uart_getchar(huart, &str);
  if (str) //查询是否接受到数据
  {
    *ch = str;
    return 1; //返回 1 表示接收成功
  }
  *ch = 0;
  return 0; //返回0表示接收失败
}
/*!
 *  @brief      查询接收buff
 *  @param      UART_HandleTypeDef *huart   串口指针（如&huart1）
 *  @param      str         接收地址
 *  @param      max_len     最大接收长度
 *  @return     接收到的字节数目
 *  Sample usage:       char buff[100];
                        uint32 num;
                        num = uart_pendbuff (UART3,&buff,100);
                        if( num != 0 )
                        {
                            printf("成功接收到%d个字节:%s",num,buff);
                        }
 */
uint8_t uart_querybuff(UART_HandleTypeDef *huart, char *buff, uint32_t max_len)
{
  uint32_t i = 0;
  while (uart_querychar(huart, buff + i))
  {
    i++;
    if (i >= max_len)
      return i;
  }
  return i;
}

/**
 *  @brief      发送字符串
 *  @param      huart       模块号（UART0~UART5）
 *  @param      str         字符串地址
 *  @since      v5.0
 *  Sample usage:       uart_putstr (&huart1,"1234567"); //实际发送了7个字节
 */
void uart_putstr(UART_HandleTypeDef *huart, char str[])
{
  while (*str)
  {
    uart_putchar(huart, *str++);
  }
}
/*!
 *  @brief      发送指定len个字节长度数组 （包括 NULL 也会发送）
 *  @param      UARTn_e       模块号（UART0~UART5）
 *  @param      buff        数组地址
 *  @param      len         发送数组的长度
 *  @since      v5.0
 *  Sample usage:       uart_putbuff (&huart1,"1234567", 3); //实际发送了3个字节'1','2','3'
 */
void uart_putbuff(UART_HandleTypeDef *huart, uint8_t *buff, uint32_t len)
{
  while (len--)
  {
    uart_putchar(huart, (char)*buff);
    buff++;
  }
}
/*!
 *  @brief      山外多功能调试助手上位机，虚拟示波器显示函数
 *  @param      wareaddr    波形数组起始地址
 *  @param      waresize    波形数组占用空间的大小
 *  @since      v5.0
*  Sample usage:
首先定义波形数组，如uint16_t var[2];
然后给数组成员赋值，如var[0]=1;
最后调用函数   vcan_sendware((uint8_t *)var, sizeof(var));
 */
void vcan_sendware(void *wareaddr, uint32_t waresize)
{
#define CMD_WARE 3
  uint8_t cmdf[2] = {CMD_WARE, ~CMD_WARE}; //串口调试 使用的前命令
  uint8_t cmdr[2] = {~CMD_WARE, CMD_WARE}; //串口调试 使用的后命令

  uart_putbuff(&husart_debug, cmdf, sizeof(cmdf));            //先发送前命令
  uart_putbuff(&husart_debug, (uint8_t *)wareaddr, waresize); //发送数据
  uart_putbuff(&husart_debug, cmdr, sizeof(cmdr));            //发送后命令
}
/**
 * 函数功能: 重定向c库函数printf到DEBUG_USARTx
 * 输入参数: 无
 * 返 回 值: 无
 * 说    明：无
 */
int fputc(int ch, FILE *f)
{
  // HAL_UART_Transmit_DMA(&husart_debug, (uint8_t *)&ch, 1);
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 5);
  // while (__HAL_UART_GET_FLAG(&husart_debug, UART_FLAG_TC) != 1); //等待发送完成
  return ch;
}

/**
 * 函数功能: 重定向c库函数getchar,scanf到DEBUG_USARTx
 * 输入参数: 无
 * 返 回 值: 无
 * 说    明：改变串口号&huart1即可改变printf所用的串口
 */
int fgetc(FILE *f)
{
  uint8_t ch = 0;
  HAL_UART_Receive(&husart_debug, &ch, 1, 5);
  return ch;
}
void show_array(uint8_t *buf, uint8_t len)
{
  uint8_t i = 0;

  for (i = 0; i < len; i++)
    printf("%x ", buf[i]);
  printf("\n");
}
void vParseString(uint8_t *buffer)
{
  uint8_t i = 0;
  for (i = 0; i < LEDNUM; i++)
  {
    if (strncmp((const char *)buffer, (const char *)OpenString[i], strlen((const char *)OpenString[i])) == 0)
    {
      printf("LED %d on\r\n", i + 1);
      return;
    }
  }
  for (i = 0; i < LEDNUM; i++)
  {
    if (strncmp((const char *)buffer, (const char *)CloseString[i], strlen((const char *)CloseString[i])) == 0)
    {
      printf("LED %d Off\r\n", i + 1);
      return;
    }
  }
  printf("Wrong cmd\r\n");
}

void ParseRealTimeString(uint8_t *buff)
{
  char *pBuffDate;
  char *pBuffTime;
  RTCDateTime mRTCDateTime;
  //获取日期字符串指针
  pBuffDate = strstr((const char *)buff, ":");
  pBuffTime = strstr((const char *)buff, ",");
  if (pBuffDate != NULL)
  {
    //指针+1，取出正确的头指针
    pBuffDate++;
    pBuffDate = strtok(pBuffDate, ",");
    mRTCDateTime.RTCDate.Year = atoi(strtok(pBuffDate, "-")) - 2000;
    mRTCDateTime.RTCDate.Month = atoi(strtok(NULL, "-"));
    mRTCDateTime.RTCDate.Date = atoi(strtok(NULL, "-"));
  }
  if (pBuffTime != NULL)
  {
    //指针+1，取出正确的头指针
    pBuffTime++;
    mRTCDateTime.RTCTime.Hours = atoi(strtok(pBuffTime, ":"));
    mRTCDateTime.RTCTime.Minutes = atoi(strtok(NULL, ":"));
    mRTCDateTime.RTCTime.Seconds = atoi(strtok(NULL, ":"));
  }
  printf("set Time: %d-%d-%d,%d:%d:%d\n",
         mRTCDateTime.RTCDate.Year + 2000,
         mRTCDateTime.RTCDate.Month,
         mRTCDateTime.RTCDate.Date,
         mRTCDateTime.RTCTime.Hours,
         mRTCDateTime.RTCTime.Minutes,
         mRTCDateTime.RTCTime.Seconds);
  SetRTCDateTime(&mRTCDateTime);
}
void ParseAlarmTimeString(uint8_t *buff)
{
  char *pBuffTime;
  uint32_t AlarmTick = 0;
  pBuffTime = strstr((const char *)buff, ":");
  if (pBuffTime != NULL)
  {
    pBuffTime++;
    AlarmTick = atoi(strtok(pBuffTime, ";"));
  }
  printf("%d",AlarmTick);
  TimerHandle_t xTimer = xTimerCreate("ALARM", AlarmTick, pdTRUE, (void *)5, vTimerCallback);
  if (xTimer != NULL)
  {
    xTimerStart(xTimer, 0);
    printf("start alarm");
  }
}
void vRtcCmd(uint8_t *buff)
{
  if (strncmp((const char *)buff, REALTIME, strlen(REALTIME)) == 0)
  {
    ParseRealTimeString(buff);
  }
  if (strncmp((const char *)buff, ALARMTIME, strlen(ALARMTIME)) == 0)
  {
    ParseAlarmTimeString(buff);
  }
}
void vTimerCallback(TimerHandle_t pxTimer)
{
  uint8_t ulTimerID = (uint8_t)pvTimerGetTimerID(pxTimer);
  printf("%d alarm on", ulTimerID);
}
