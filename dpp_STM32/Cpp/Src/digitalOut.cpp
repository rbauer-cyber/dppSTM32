/*
 * CDigitalOut.cpp
 *
 *  Created on: Apr 17, 2025
 *      Author: rbauer
 */

#include "gpio.h"
#include "../Inc/digitalOut.hpp"

// Each output data pin on the STM32 is selected by a combination of port and pin numbers
// Defines all the ports available on an STM32
// Data pins have same assignments and position as arduino UNO
enum EGpioPort
{
	kGpioPortA = 0,
	kGpioPortB = 1,
	kGpioPortC = 2
};

// Defines maximum number of digital/analog pins on a port
// Defines all the pins available on an STM32 port.
enum EGpioPin
{
	kGpioPin00 = 0,
	kGpioPin01,
	kGpioPin02,
	kGpioPin03,
	kGpioPin04,
	kGpioPin05,
	kGpioPin06,
	kGpioPin07,
	kGpioPin08,
	kGpioPin09,
	kGpioPin10,
};

// List of HAL port selectors
GPIO_TypeDef* s_digitalPort[] = { GPIOA, GPIOB, GPIOC };

#define GPIO_PIN_INVALID 255

// List of HAL pin selectors
uint16_t s_digitalPin[] =
{
	GPIO_PIN_0,
	GPIO_PIN_1,
	GPIO_PIN_2,
	GPIO_PIN_3,
	GPIO_PIN_4,
	GPIO_PIN_5,
	GPIO_PIN_6,
	GPIO_PIN_7,
	GPIO_PIN_8,
	GPIO_PIN_9,
	GPIO_PIN_10,
	GPIO_PIN_11,
	GPIO_PIN_12,
	GPIO_PIN_13,
	GPIO_PIN_14,
	GPIO_PIN_15,
};

struct DigitalOut {
	uint8_t   validPin;
	uint8_t   gpioPort;
	uint8_t   gpioPin;
};

// Index array s_digitalOut by digital pin enum value kDigitalPin00-kDigitalPin15
// kDigitalPin defined in header file.
static DigitalOut s_digitalOut[] =
{
		{ 0, kGpioPortA, kGpioPin03 },	//D00 - PA_3 - reserved uart2 rx
		{ 0, kGpioPortA, kGpioPin02 },	//D01 - PA_2 - reserved uart2 tx
		{ 1, kGpioPortA, kGpioPin10 },	//D02 - PA_10
		{ 1, kGpioPortB, kGpioPin03 },	//D03 - PB_3
		{ 1, kGpioPortB, kGpioPin05 },	//D04 - PB_5
		{ 1, kGpioPortB, kGpioPin04 },	//D05 - PB_4
		{ 1, kGpioPortB, kGpioPin10 },	//D06 - PB_10
		{ 1, kGpioPortA, kGpioPin08 },	//D07 - PA_8
		{ 1, kGpioPortA, kGpioPin09 },	//D08 - PA_9
		{ 1, kGpioPortC, kGpioPin07 },	//D09 - PC_7
		{ 1, kGpioPortB, kGpioPin06 },	//D10 - PB_6
		{ 1, kGpioPortA, kGpioPin07 },	//D11 - PA_7
		{ 1, kGpioPortA, kGpioPin06 },	//D12 - PA_6
		{ 1, kGpioPortA, kGpioPin05 },	//D13 - PA_5 - mapped to builtin LED
		{ 1, kGpioPortB, kGpioPin09 },	//D14 - PB_9
		{ 1, kGpioPortB, kGpioPin08 },	//D15 - PB_8
};

uint16_t s_analogPin[] =
{
	GPIO_PIN_0,
	GPIO_PIN_1,
	GPIO_PIN_4,
	GPIO_PIN_0,
	GPIO_PIN_1,
	GPIO_PIN_0,
};

struct AnalogOut {
	uint8_t   validPin;
	uint8_t   gpioPort;
	uint8_t   gpioPin;
};

// Index array s_analogIn by analog pin enum value kAnalogPin00-kAnalogPin05
// kAnalogPin defined in header file.
static AnalogOut s_analogOut[] =
{
		{ 1, kGpioPortA, kGpioPin00 },	//A00 - PA_0
		{ 1, kGpioPortA, kGpioPin01 },	//A01 - PA_1
		{ 1, kGpioPortA, kGpioPin02 },	//A02 - PA_4
		{ 1, kGpioPortB, kGpioPin03 },	//A03 - PB_0
		{ 1, kGpioPortC, kGpioPin04 },	//A04 - PC_1
		{ 1, kGpioPortC, kGpioPin05 },	//A05 - PC_0
};

//static DigitalOut s_digitalOut[] =
//{
//		{ GPIOA, GPIO_PIN_9 }, //D08 - PA_9
//		{ GPIOC, GPIO_PIN_7 }, //D09 - PC_7
//		{ GPIOB, GPIO_PIN_6 }, //D10 - PB_6
//		{ GPIOA, GPIO_PIN_7 }, //D11 - PA_7
//		{ GPIOA, GPIO_PIN_6 }, //D12 - PA_6
//};

static void GetHALPortAndPin(GPIO_TypeDef*& halPort, uint16_t& halPin, EDigitalPin pin)
{
	uint8_t portIndex = s_digitalOut[pin].gpioPort;
	GPIO_TypeDef* port = s_digitalPort[portIndex];
    uint16_t pinIndex = s_digitalOut[pin].gpioPin;
    halPin = s_digitalPin[pinIndex];
    halPort = port;
}

static void GetHALPortAndPin(GPIO_TypeDef*& halPort, uint16_t& halPin, EAnalogPin pin)
{
	uint8_t portIndex = s_analogOut[pin].gpioPort;
	GPIO_TypeDef* port = s_digitalPort[portIndex];
    uint16_t pinIndex = s_analogOut[pin].gpioPin;
    halPin = s_analogPin[pinIndex];
    halPort = port;
}

CDigitalOut::CDigitalOut()
{
	// TODO Auto-generated constructor stub

}

CDigitalOut::~CDigitalOut()
{
	// TODO Auto-generated destructor stub
}

int CDigitalOut::Write(EDigitalPin pin, bool state)
{
	GPIO_TypeDef* ledPort;
	uint16_t ledPin;
	GetHALPortAndPin(ledPort, ledPin, pin);
    GPIO_PinState pinState = (state == true) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    HAL_GPIO_WritePin(ledPort, ledPin, pinState);
	return 0;
}

int CDigitalOut::Toggle(EDigitalPin pin)
{
	GPIO_TypeDef* ledPort;
	uint16_t ledPin;
	GetHALPortAndPin(ledPort, ledPin, pin);
    HAL_GPIO_TogglePin(ledPort, ledPin);
	return 0;
}

int CDigitalOut::Read(EDigitalPin pin)
{
	GPIO_TypeDef* halPort;
	uint16_t halPin;
	GetHALPortAndPin(halPort, halPin, pin);
	GPIO_PinState pinState;
	pinState = HAL_GPIO_ReadPin(halPort, halPin);
	return pinState;
}

int CDigitalOut::Write(EAnalogPin pin, bool state)
{
	GPIO_TypeDef* ledPort;
	uint16_t ledPin;
	GetHALPortAndPin(ledPort, ledPin, pin);
    GPIO_PinState pinState = (state == true) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    HAL_GPIO_WritePin(ledPort, ledPin, pinState);
	return 0;
}

int Toggle(EAnalogPin ePin)
{
	return 0;
}

