/*
 * console.h
 *
 *  Created on: Dec 5, 2024
 *      Author: rbauer
 */

#ifndef CONSOLE_H_
#define CONSOLE_H_

#include <stdint.h>
#include <stdarg.h>

#ifdef __cplusplus
    extern "C" {
#endif

void consoleInit();
void consoleDisplayArgs(const char * format, ... );
void consoleDisplayPromptArgs(const char * format, ... );
void consoleDisplayPrompt(const char* pMsg);
void consoleDisplay(const char* pMsg);
uint8_t consoleWaitForKey();
uint8_t consoleInputReady();
uint8_t consoleReadByte();

#ifdef __cplusplus
}
#endif

#endif /* CONSOLE_H_ */
