#ifndef MULTI_LED_H
#define MULTI_LED_H
#include <stdint.h>

class CMultiLed
{
public:
    static const uint8_t MAX_LEDS = 5;
private:
    uint8_t m_maxNumber;
    uint8_t m_ledPins[MAX_LEDS];
    uint8_t m_numPins;
    uint8_t m_number;
public:
    explicit CMultiLed( uint8_t ledPins[], uint8_t numPins );
    explicit CMultiLed();

    static uint8_t MaxPins();
    uint8_t MaxNumber();
    void SetLed(uint8_t index, uint8_t state);
    void SetNumber(uint8_t number);
    void ShowState();
    void ToggleLed(uint8_t index);
};
#endif
