#include "daisy_seed.h"
#include "daisysp.h"
#include "button.h"
#include "dualLedButton.h"

DualLedButton::DualLedButton(Pin pin, uint32_t shortTime, uint32_t longTime, Pin redLedPin, Pin greenLedPin)
: Button(pin, shortTime, longTime)
{
    redGpio.Init(redLedPin, GPIO::Mode::OUTPUT);
    greenGpio.Init(greenLedPin, GPIO::Mode::OUTPUT);
}

DualLedButton DualLedButton::New(Pin pin, uint32_t shortTime, uint32_t longTime, Pin redLedPin, Pin greenLedPin)
{
    return DualLedButton(pin, shortTime, longTime, redLedPin, greenLedPin);
}

void DualLedButton::Off()
{
    redGpio.Write(false);
    greenGpio.Write(false);
}

void DualLedButton::Red()
{
    redGpio.Write(true);
    greenGpio.Write(false);
}

void DualLedButton::Green()
{
    redGpio.Write(false);
    greenGpio.Write(true);
}