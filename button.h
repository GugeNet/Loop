#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisy::seed;
using namespace daisysp;

#ifndef BUTTON_H
#define BUTTON_H

class Button {

    private:
        GPIO gpio;
        bool previousState, longPress;
        uint32_t pressedTime, shortTime, longTime;

    public:
        static Button New(Pin pin, uint32_t shortTime, uint32_t longTime);
        Button(Pin pin, uint32_t shortTime, uint32_t longTime);
        bool Check();
        bool GetPressed();
        bool GetLong();
};

#endif