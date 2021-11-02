/*
 * @Author: Tian Wei
 * @Date: 2020-03-14 11:21:19
 * @Description: 
 * @Version: V1.0
 */
#ifndef __UART_H
#define __UART_H

#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include "usart.h"
#include "string.h"
#include "rtc.h"


#define REALTIME "realtime"
#define ALARMTIME "alarmtime"
#define LEDNUM 3
#define husart_debug     huart1        //printf���ô��ں�
void vParseString(uint8_t *buffer);
void vRtcCmd(uint8_t *buff);
void ParseRealTimeString(uint8_t *buff);
void uart_putchar (UART_HandleTypeDef *huart,char ch);//����һ���ַ�
void uart_putstr (UART_HandleTypeDef *huart,char str[]);//�����ַ���
void uart_putbuff (UART_HandleTypeDef *huart,uint8_t *buff, uint32_t len);//uart_putbuff (&huart1,(uint8_t *)"1234567", 7);
void vcan_sendware(void *wareaddr, uint32_t waresize);//�������ݲ�����ɽ����λ������ʾ����
void uart_getchar(UART_HandleTypeDef *huart,char *ch);
uint8_t uart_querychar (UART_HandleTypeDef *huart, char *ch);//��ѯ�Ƿ���ܵ�һ���ֽ�
uint8_t uart_querybuff (UART_HandleTypeDef *huart, char *buff, uint32_t max_len);
void show_array(uint8_t *buf,uint8_t len);
#endif
