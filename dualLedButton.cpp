#include "daisy_seed.h"
#include "daisysp.h"
#include "dualLedButton.h"

using namespace daisy;

void DualLedButton::Init(Pin pin, float rate, Pin greenLedPin, Pin redLedPin)
{
    button.Init(pin, rate, Switch::TYPE_MOMENTARY, Switch::POLARITY_INVERTED, Switch::PULL_UP);
    redGpio.Init(redLedPin, GPIO::Mode::OUTPUT);
    greenGpio.Init(greenLedPin, GPIO::Mode::OUTPUT);
}

void DualLedButton::Debounce()
{
    button.Debounce();
}

bool DualLedButton::Pressed()
{
    return button.Pressed();
}

bool DualLedButton::RisingEdge()
{
    return button.RisingEdge();
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

void DualLedButton::Color(int color)
{
    redGpio.Write(color == 1);
    greenGpio.Write(color == 2);
}