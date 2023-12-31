#include "daisy_seed.h"
#include "daisysp.h"
#include "button.h"

using namespace daisy;

Button Button::New(Pin pin, uint32_t shortTime, uint32_t longTime) {
    return Button(pin, shortTime, longTime);
}

Button::Button(Pin pin, uint32_t shortTime, uint32_t longTime) {
    gpio.Init(pin, GPIO::Mode::INPUT, GPIO::Pull::PULLDOWN);
    previousState = false;
}

bool Button::Check()
{
    bool pressed = gpio.Read();
    if(pressed == previousState)
        return false;
    previousState = pressed;
    if(pressed)
    {
        pressedTime = System::GetNow();
    }
    else
    {
        uint32_t duration = System::GetNow() - pressedTime;
        if(duration < shortTime) // bounce
            return false;
        longPress = duration > longTime;
        return true;
    }
    return false;
}

bool Button::GetPressed()
{
    return previousState;
}

bool Button::GetLong()
{
    return longPress;
}