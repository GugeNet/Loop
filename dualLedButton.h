#include "daisy_seed.h"
#include "daisysp.h"
#include "button.h"

#ifndef DUALLEDBUTTON_H
#define DUALLEDBUTTON_H

class DualLedButton : public Button {

    private:
        GPIO redGpio, greenGpio;

    public:
        static DualLedButton New(Pin pin, uint32_t shortTime, uint32_t longTime, Pin greenLedPin, Pin redLedPin);
        DualLedButton(Pin pin, uint32_t shortTime, uint32_t longTime, Pin greenLedPin, Pin redLedPin);

        void Off();
        void Red();
        void Green();
        void Color(int color);
};


#endif
