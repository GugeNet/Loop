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

    heartbeat++;

    if(state == RECORDING) {
        *leftOut += *bufferLeft;
        *rightOut += *bufferRight;
        *bufferLeft = leftIn;
        *bufferRight = rightIn;
        if(leftIn + rightIn > 0.5 || ((heartbeat >> 8) & 1) == 1)
            button->Red();
        else
            button->Off();
    } 
    else if(state == PLAYING) {
        *leftOut += *bufferLeft;
        *rightOut += *bufferRight;
        if(*bufferLeft + *bufferRight > 0.5 || ((heartbeat >> 8) & 1) == 1)
            button->Green();
        else
            button->Off();
    }
    else if(state == BLANK || state == MUTED) {
        button->Off();
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
    bool clicked = button->Check();
    if(clicked) {
        bool longClick = button->GetLong();
        if(longClick) {
            if(!clear && !record) {
                if(state == BLANK)
                    nextState = RECORDING;
                else if(state == PLAYING || state == RECORDING)
                    nextState = MUTED;
                else if(state == MUTED)
                    nextState = PLAYING;
            }
        } else {
            if(!clear && !record) {
                if(state == PLAYING)
                    nextState = state = MUTED;
                else if(state == MUTED)
                    nextState = state = PLAYING;
                else if(state == BLANK)
                {
                    state = RECORDING;
                    nextState = PLAYING;
                }
            } else if(!clear && record)
            {
                if(state != RECORDING)
                    nextState = RECORDING;
                else
                    nextState = state;
            }
        }
    }
}
