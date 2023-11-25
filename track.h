#include "daisy_seed.h"
#include "daisysp.h"
#include "dualLedButton.h"

#ifndef TRACK_H  // Include guards to prevent multiple inclusion
#define TRACK_H

using namespace daisy;

class Track {
    public:
    enum State {
        BLANK,
        MUTED,
        PLAYING,
        RECORDING,
        LOOPING,
        CLEARING,
        SUGGESTING
    };

    private:
        State state, nextState;
        int ordinal;
        DualLedButton *button;
        uint heartbeat;

    public:
        Track(int ordinal, DualLedButton *btn);
        static Track New(int ordinal, DualLedButton *button);
        void Audio(
            const float leftIn, const float rightIn, 
            float *leftOut, float *rightOut, 
            float *bufferLeft, float *bufferRight);
        void Loop();
        DualLedButton *GetButton();
        void Check(bool clear, bool record);
};

#endif