#include "daisy_seed.h"
#include "daisysp.h"

#ifndef DUALLEDBUTTON_H
#define DUALLEDBUTTON_H

using namespace daisy;

class DualLedButton {

    private:
        GPIO redGpio, greenGpio;
        Switch button;

    public:
        void Init(Pin pin, float rate, Pin greenLedPin, Pin redLedPin);

        void Debounce();
        bool Pressed();
        bool RisingEdge();

        void Off();
        void Red();
        void Green();
        void Color(int color);
};


#endif
