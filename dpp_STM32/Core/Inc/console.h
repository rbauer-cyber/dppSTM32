/*
 * console.h
 *
 *  Created on: Dec 5, 2024
 *      Author: rbauer
 */
#define NO_CONSOLE

#ifndef CONSOLE_H_
#define CONSOLE_H_

#include <stdint.h>
#include <stdarg.h>

#ifdef __cplusplus
    extern "C" {
#endif

#ifdef NO_CONSOLE
#define CONSOLE_DISPLAY_ARGS(a, ...)
#define CONSOLE_DISPLAY(a)
#else
#define CONSOLE_DISPLAY_ARGS(a, ...) consoleDisplayArgs(a, __VA_ARGS__)
#define CONSOLE_DISPLAY(a) consoleDisplay(a)
#endif

void consoleInit();
void consoleDisplayArgs(const char * format, ... );
void consoleDisplayPromptArgs(const char * format, ... );
void consoleDisplayPrompt(const char* pMsg);
void consoleDisplay(const char* pMsg);
uint8_t consoleReadAsyncInit();
uint8_t consoleReadAsync( char outBuf[], uint8_t maxSize );
uint8_t consoleReadByteAsync();
uint8_t consoleReadString( char cbuf[], uint8_t maxSize );
uint8_t consoleWaitForKey();
uint8_t consoleInputReady();
uint8_t consoleInputDone();
uint8_t consoleReadByte();

#ifdef __cplusplus
}
#endif

#endif /* CONSOLE_H_ */
