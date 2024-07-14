#ifndef Button_h
#define Button_h

#include <Arduino.h>

class Button
{
public:
    Button(uint8_t pin, uint32_t debounceDelay = 100);
    void begin(uint8_t mode = INPUT_PULLUP); //INPUT_PULLUP - default.
    bool isPressed();
    bool updateAndCheckPeleased();

private:
    uint8_t _pin;
    bool _currentState;
    bool _lastState;
    bool _released;
    uint32_t _lastDebounceTime;
    uint32_t _debounceDelay;
};

#endif