#include "daisy_seed.h"
#include "daisysp.h"
#include "track.h"

using namespace daisy;

Track::Track(int o, DualLedButton *btn) {
    ordinal = o;
    button = btn;
    state = BLANK;
    heartbeat = 0;

    if(o == 0) {
        state = BLANK;
        nextState = RECORDING;
    }
}

Track Track::New(int o, DualLedButton *btn) {
    return Track(o, btn);
}

void Track::Audio(
    const float leftIn, const float rightIn, 
    float *leftOut, float *rightOut, 
    float *bufferLeft, float *bufferRight) {

    if(state == RECORDING) {
        *leftOut += *bufferLeft;
        *rightOut += *bufferRight;
        *bufferLeft = leftIn;
        *bufferRight = rightIn;
    } 
    else if(state == PLAYING) {
        *leftOut += *bufferLeft;
        *rightOut += *bufferRight;
    }
}

void Track::Loop() {
    state = nextState;
    if(state == RECORDING)
        nextState = PLAYING;
}

DualLedButton *Track::GetButton() {
    return button;
}

void Track::Check(bool clear, bool record)
{
    bool clicked = button->Pressed();
    if(clicked) {
        if(true || (!clear && !record)) {
            if(state == PLAYING)
                nextState = MUTED;
            else if(state == MUTED)
                nextState = PLAYING;
            else if(state == BLANK)
            {
                nextState = RECORDING;
            }
        } 
        else if(!clear && record)
        {
            if(state != RECORDING)
                nextState = RECORDING;
            else
                nextState = state;
        }
    }
}

void Track::Lights()
{
    if(state == RECORDING || (state == BLANK && button->Pressed()))
        button->Red();
    if(state == MUTED || state == BLANK)
        button->Off();
    if(state == PLAYING)
        button->Green();
}
