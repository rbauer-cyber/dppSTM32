/*
 * CDigitalOut.hpp
 *
 *  Created on: Apr 17, 2025
 *      Author: rbauer
 */

#ifndef SRC_DIGITALOUT_HPP_
#define SRC_DIGITALOUT_HPP_

// MCU pin assignments for arduino UNO headers on STM32 nucleo-F446RE board
// D00-D16 reference pins with the same name on the connectors CN9 and CN5
// CN9 is the inside connector on the lower right
// CN5 is the inside connector on the upper right
enum EDigitalPin
{
	kDigitalPin00,	//PA3	D00 - CN9 - UART2_RX (pre-assigend to usb connector)
	kDigitalPin01,	//PA2	D01 - CN9 - UART2_TX (pre-assigend to usb connector)
	kDigitalPin02,	//PA10	D02 - CN9
	kDigitalPin03,	//PB3	D03 - CN9
	kDigitalPin04,	//PB5	D04 - CN9
	kDigitalPin05,	//PB4	D05 - CN9
	kDigitalPin06,	//PB10	D06 - CN9
	kDigitalPin07,	//PA8	D07 - CN9
	kDigitalPin08,	//PA9	D08 - CN5
	kDigitalPin09,	//PC7	D09 - CN5
	kDigitalPin10,	//PB6	D11 - CN5
	kDigitalPin11,	//PA7	D12 - CN5
	kDigitalPin12,	//PA6	D13 - CN5
	kDigitalPin13,	//PA5	D14 - CN5 - (pre-assigend to builtin LED)
	kDigitalPin14,	//PB9	D15 - CN5
	kDigitalPin15,	//PB8	D16 - CN5
	kDigitalPinInvalid
};

enum EAnalogPin
{
	kAnalogPin00,	//PA0	A0 - CN8
	kAnalogPin01,	//PA1	A1 - CN8
	kAnalogPin02,	//PA4	A2 - CN8
	kAnalogPin03,	//PB0	A3 - CN8
	kAnalogPin04,	//PC1	A4 - CN8
	kAnalogPin05,	//PC0	A5 - CN8
	kAnalogPinInvalid
};

class CDigitalOut
{
public:
	CDigitalOut();
	virtual ~CDigitalOut();

	static int Write(EDigitalPin pin, bool state);
	static int Toggle(EDigitalPin pin);
	static int Read(EDigitalPin pin);
	static int Write(EAnalogPin pin, bool state);
	static int Toggle(EAnalogPin pin);
};

#endif /* SRC_DIGITALOUT_HPP_ */
