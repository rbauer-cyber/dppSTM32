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

#include "main.h"
#include "tim.h"
#include "gpio.h"
#include "usart.h"
#include "bsp.hpp"

#include "multiLed.hpp"
#include "digitalOut.hpp"
#include "Stepper.hpp"
#include "console.h"

#ifdef USE_QUANTUM
void appSysTickHandler()
{
	Q_SysTick_Handler();
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

#ifdef USE_MOTOR
int16_t s_stepsPerRevolution = 2048;
int16_t s_increment = 2048;
int16_t s_motorSpeed = 15;
static int16_t s_motorPosition = 0;

// Initialize with pin sequence IN1-IN3-IN2-IN4 on the stepper motor controller board
// s_stepper(s_stepsPerRevolution, A3, A1, A2, A0);
Stepper s_stepper(s_stepsPerRevolution,
		kAnalogPin02, kAnalogPin04,
		kAnalogPin03, kAnalogPin05);

void initMotor()
{
    s_stepper.setSpeed(s_motorSpeed);
    s_motorPosition = 0;
}

int16_t moveMotor(int16_t increment)
{
    s_stepper.step(increment);
    s_motorPosition += increment;
    return s_motorPosition;
}
#endif

void altMain()
{
	//int32_t encoderPosition = 0;
	// getMicros() defined in main using TIM2 at 50MHz
#ifdef USE_MOTOR
	uint32_t startTimeUs = getMicros();
	uint32_t timeNowUs = 0;

#ifdef USE_MOTOR_WITH_ENCODER
	int32_t newEncoderPosition = 0;
	int32_t encoderPosition = 0;
	int motorPositionDelta = 0;
	encoderPosition = encoderUpdate();
#endif
	int motorPosition = 0;
	int motorIncrement = 128;
	//int motorIndex = 0;
	//bool state = false;
	initMotor();
#endif

	while ( 1 )
	{
#if defined(USE_POLLING)
#ifdef USE_HAL_DELAY
		for ( size_t pinIndex = 0; pinIndex < g_multiLed.MaxPins(); pinIndex++ )
		{
			consoleDisplayArgs("Toggling LED %d\r\n", pinIndex);
			g_multiLed.SetLed(pinIndex, 1);
			HAL_Delay(1000);
			g_multiLed.SetLed(pinIndex, 0);
			HAL_Delay(1000);

			//encoderPosition = encoderUpdate();
			//consoleDisplayArgs("encoderPosition = %d\r\n", encoderPosition);
			//g_multiLed.SetNumber(static_cast<uint8_t>(encoderPosition));
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
#ifdef USE_MOTOR_WITH_ENCODER
			newEncoderPosition = encoderUpdate();

			if ( newEncoderPosition != encoderPosition )
			{
				motorPositionDelta = (newEncoderPosition - encoderPosition) * motorIncrement;
				s_stepper.step(motorPositionDelta);
				motorPosition = motorPosition + motorPositionDelta;
				encoderPosition = newEncoderPosition;
			}

			g_multiLed.SetNumber(encoderPosition);

			consoleDisplayArgs("Encoder position %d, Motor Position %d\r\n",
					encoderPosition, motorPosition);
#else
#ifdef USE_MOTOR
			//motorIndex = motorPosition / abs(motorIncrement);
			s_stepper.step(motorIncrement);
			motorPosition += motorIncrement;

			if ( motorPosition >= s_stepsPerRevolution || motorPosition < 0 )
			{
				motorIncrement = motorIncrement * -1;
			}
#endif
#endif
//			consoleDisplayArgs("Encoder position %d\r\n", encoderPosition);
		}
#endif
#elif defined(USE_QUANTUM)
		consoleDisplay("Invoking Quantum BSP_Main\r\n");
		BSP::bspMain();
#else
		encoderPosition = encoderUpdate();
		consoleDisplayArgs("encoderPosition = %d\r\n", encoderPosition);
		g_multiLed.SetNumber(static_cast<uint8_t>(encoderPosition));
		HAL_Delay(1000);
#endif
	}
}

