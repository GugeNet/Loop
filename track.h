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
        PLAYING,
        RECORDING,
        LOOPING,
        CLEARING,
        SUGGESTING
    };

    private:
        State state;
        int ordinal;
        DualLedButton *button;

    public:
        Track(int ordinal, DualLedButton *btn);
        static Track New(int ordinal, DualLedButton *button);
        void Audio(AudioHandle::InterleavingInputBuffer  in,
                           AudioHandle::InterleavingOutputBuffer out,
                           size_t                                size,
                           float *buffer);
};

#endif