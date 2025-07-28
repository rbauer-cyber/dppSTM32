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
//#define USE_UART_TX_DATA
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
#include "QFsys.h"


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
#else
void appSysTickHandler()
{
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

extern CMultiLed g_multiLed;

void altMain()
{
	while ( 1 )
	{
		uint32_t startTimeUs = getMicros();
		uint32_t timeNowUs = 0;
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

