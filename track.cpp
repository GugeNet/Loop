#include "daisy_seed.h"
#include "daisysp.h"
#include "track.h"

using namespace daisy;

Track::Track(int o, DualLedButton *btn)
{
    ordinal = o;
    button = btn;
    state = BLANK;
}

Track Track::New(int o, DualLedButton *btn)
{
    return Track(o, btn);
}

void Track::Audio(AudioHandle::InterleavingInputBuffer  in,
                  AudioHandle::InterleavingOutputBuffer out,
                  size_t                                size,
                  float *buffer)
{

}