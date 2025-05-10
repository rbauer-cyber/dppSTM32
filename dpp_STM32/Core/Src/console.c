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
#include "main.h"

#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))

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

uint8_t s_rxKeyBuf[2] = {0};
uint8_t s_rxBuffer[20] = {0};
uint8_t s_rxBufferSize = ARRAY_SIZE(s_rxBuffer);
uint8_t s_rxIndex = 0;
uint8_t s_inputReady = 0;
uint8_t s_inputDone = 0;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	s_inputReady = 1;
}

uint8_t copyString(char cbuf[], uint8_t ubuf[], uint8_t maxSize)
{
	uint8_t index = 0;

	while ( index < maxSize && ubuf[index] )
	{
		cbuf[index] = (char)ubuf[index];
		index += 1;
	}

	cbuf[index++] = '\r';
	cbuf[index++] = '\n';
	cbuf[index++] = 0;

	return index;
}

uint8_t consoleReadAsyncInit()
{
	memset(s_rxBuffer, 0, sizeof s_rxBuffer);
	s_inputReady = 0;
	s_inputDone = 0;
	s_rxIndex = 0;
	return 0;
}

uint8_t consoleReadAsync( char outBuf[], uint8_t maxSize )
{
	memset(s_rxBuffer, 0, sizeof s_rxBuffer);
	s_inputReady = 0;
	s_inputDone = 0;
	s_rxIndex = 0;

	// Leave enough size to append CRLF and null bytes.
	HAL_UART_Receive_IT( &huart2, s_rxKeyBuf, maxSize-4 );
	return 0;
}

uint8_t consoleReadByteAsync()
{
	s_inputReady = 0;
	HAL_UART_Receive_IT( &huart2, s_rxKeyBuf, 1 );
	return 0;
}

uint8_t consoleInputReady()
{
	return s_inputReady;
}

uint8_t consoleInputDone()
{
	// Carriage return explicitly terminates the input.
	if ( s_inputReady )
	{
		if ( s_rxKeyBuf[0] == '\r' )
		{
			s_inputDone = 1;
		}
		else
		{
			s_rxBuffer[s_rxIndex++] = s_rxKeyBuf[0];
		}
	}

	return s_inputDone;
}

uint8_t consoleReadString( char cbuf[], uint8_t maxSize )
{
	uint8_t size = copyString( cbuf, s_rxBuffer, maxSize-4 );
	return size;
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

