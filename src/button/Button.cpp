#include "Button.h"

Button::Button(uint8_t pin, uint32_t debounceDelay)
    : _pin(pin), _debounceDelay(debounceDelay), _currentState(HIGH), _lastState(HIGH), _released(false), _lastDebounceTime(0) {}

void Button::begin(uint8_t mode)
{
    Serial.println("Button begin");
    pinMode(_pin, mode); // Set the button pin as an input with internal pull-up resistor
}

bool Button::isPressed()
{
    return _currentState == LOW;
}

bool Button::updateAndCheckPeleased()
{
    bool _currentState = digitalRead(_pin);
    _released = false;

    if (_currentState == LOW && _lastState == HIGH)
    {
        // Serial.printf("first current %d last %d release %d _lastDebounceTime %d \n", _currentState, _lastState, _released, _lastDebounceTime);
        _lastDebounceTime = millis();
        _lastState = LOW;
    }

    if ((millis() - _lastDebounceTime) > _debounceDelay && _lastState == LOW)
    {
        //  Serial.printf("second current %d last %d release %d _lastDebounceTime %d \n", _currentState, _lastState, _released, _lastDebounceTime);
        if (_currentState == HIGH && _lastState == LOW)
        {
            //  Serial.printf("last current %d last %d release %d _lastDebounceTime %d \n", _currentState, _lastState, _released, _lastDebounceTime);
            _released = true;
            _lastState = HIGH;
        }
    }
    return _released;
}