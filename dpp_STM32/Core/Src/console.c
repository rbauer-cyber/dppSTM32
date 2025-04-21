/*
 * console.cpp
 *
 *  Created on: Dec 5, 2024
 *      Author: rbauer
 */
#include <console.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "main.h"
#include "usart.h"

void consoleInit()
{
	//Serial.begin(115200);
}

static char s_buffer[80];

void consoleDisplayArgs(const char * format, ... )
{
	va_list args;
	va_start (args, format);
	vsnprintf (s_buffer, 60, format, args);
	s_buffer[59] = 0;
	va_end (args);

	HAL_UART_Transmit(&huart2, (const uint8_t*)s_buffer, strlen(s_buffer), 1000);
}

void consoleDisplayPrompt(const char* pMsg)
{
	HAL_UART_Transmit(&huart2, (const uint8_t*)pMsg, strlen(pMsg), 1000);
}

void consoleDisplayPromptArgs(const char * format, ... )
{
	va_list args;
	va_start (args, format);
	vsnprintf (s_buffer, 60, format, args);
	s_buffer[59] = 0;
	va_end (args);

	HAL_UART_Transmit(&huart2, (const uint8_t*)s_buffer, strlen(s_buffer), 1000);
}

void consoleDisplay(const char* pMsg)
{
	HAL_UART_Transmit(&huart2, (const uint8_t*)pMsg, strlen(pMsg), 1000);
}

uint8_t consoleInputReady()
{
    //return( Serial.available() );
	return 0;
}

uint8_t consoleReadByte()
{
    //return( Serial.read() );
	return 0;
}

uint8_t consoleWaitForKey()
{
    uint8_t key = 0;

//    while ( !consoleInputReady() )
//        ;
//
//    key = consoleReadByte();
    return key;
}

