/*
 * altMain.cpp
 *
 *  Created on: Apr 5, 2025
 *      Author: rbauer
 */

#define USE_QUANTUM
//#define USE_HAL_DELAY
//#define USE_MOTOR
//#define USE_MOTOR_WITH_ENCODER
//#define USE_TIMER_INTERRUPT
//#define USE_UART_TX_INTERRUPT
#define USE_UART_TX_DATA
//#define USE_UART_RX_INTERRUPT
//#define USE_UART_RX_BLOCK
//#define USE_UART_RX
//#define USE_UART_DMA
//#define USE_POLLING

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <atomic>

#include "main.h"
#include "tim.h"
#include "gpio.h"
#include "usart.h"
#include "bsp.hpp"

#include "multiLed.hpp"
#include "digitalOut.hpp"
#include "console.h"

// Define functions for enabling/disabling HAL interrupts for critical sections
// and for setting/detecting Q system events.
#ifdef __cplusplus
extern "C" {
#endif

void QF_int_disable_(void)
{
	HAL_SuspendTick();
}

void QF_int_enable_(void)
{
	HAL_ResumeTick();
}

void QF_crit_entry_(void)
{
	HAL_SuspendTick();
}

void QF_crit_exit_(void)
{
	HAL_ResumeTick();
}

// The following set of functions assume the STM32 instructions
// are atomic for reading or writing a 16 bit variables.
// volatile static uint16_t s_sysAppInterrupt = 0;
std::atomic<uint16_t> s_sysAppInterrupt(0);

volatile void QF_setSysAppEvent()
{
	s_sysAppInterrupt = 1;
}

volatile void QF_clearSysAppEvent()
{
	s_sysAppInterrupt = 0;
}

volatile uint16_t QF_getSysAppEvent()
{
	return s_sysAppInterrupt;
}

#ifdef __cplusplus
}
#endif

#ifdef USE_QUANTUM
void appSysTickHandler()
{
	// Use this variable to communicate with QV::onIdle
	// to indicate that a critical interrupt from the app
	// has occurred and needs to be service.
	// Note Q_SysTick_Handler broadcasts the QP::QTimeEvt::TICK_X event.
	Q_SysTick_Handler();

	if ( !QF_getSysAppEvent() )
		QF_setSysAppEvent();
}
#endif

//kAnalogPin00,	//PA0	A0 - CN8 - IN1
//kAnalogPin01,	//PA1	A1 - CN8 - IN2
//kAnalogPin02,	//PA4	A2 - CN8 - IN3
//kAnalogPin03,	//PB0	A3 - CN8 - IN4
//kAnalogPin04,	//PC1	A4 - CN8
//kAnalogPin05,	//PC0	A5 - CN8

//kAnalogPin02,	//PA4	A2 - CN8 - IN1
//kAnalogPin03,	//PB0	A3 - CN8 - IN2
//kAnalogPin04,	//PC1	A4 - CN8 - IN3
//kAnalogPin05,	//PC0	A5 - CN8 - IN4

// Connections from nucleo board CN8 to motor controller
//purple - IN1	- A2
//gray	 - IN2	- A3
//white	 - IN3	- A4
//black	 - IN4	- A5

static uint8_t s_multiLedPins[] =
{
		kDigitalPin08, // PA9
		kDigitalPin09, // PC7
		kDigitalPin10, // PB6
		kDigitalPin11, // PA7
		kDigitalPin12, // PA6
};

static uint8_t s_numPins = sizeof(s_multiLedPins) / sizeof(s_multiLedPins[0]);
CMultiLed g_multiLed( s_multiLedPins, s_numPins );

void altMain()
{
	while ( 1 )
	{
#if defined(USE_POLLING)
		// Use this code for testing LEDS
#ifdef USE_HAL_DELAY
		for ( size_t pinIndex = 0; pinIndex < g_multiLed.MaxPins(); pinIndex++ )
		{
			consoleDisplayArgs("Toggling LED %d\r\n", pinIndex);
			g_multiLed.SetLed(pinIndex, 1);
			HAL_Delay(1000);
			g_multiLed.SetLed(pinIndex, 0);
			HAL_Delay(1000);
		}
#else
		// selecting MAX_LEDS index causes builtin LED to toggle
		timeNowUs = getMicros();

		// one second delay
		if ( (timeNowUs-startTimeUs) > 1000000 )
		{
			startTimeUs = timeNowUs;
			//consoleDisplay("Toggling LED\r\n");
			g_multiLed.ToggleLed(CMultiLed::MAX_LEDS);
		}
#endif
#else
		consoleDisplay("Invoking Quantum BSP_Main\r\n");
		BSP::bspMain();
#endif
	}
}

