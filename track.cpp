#include "daisy_seed.h"
#include "daisysp.h"
#include "track.h"

using namespace daisy;

Track::Track(int o, DualLedButton *btn) {
    ordinal = o;
    button = btn;
    state = BLANK;
    previousButtonState = false;
    nextState = BLANK;
}

Track Track::New(int o, DualLedButton *btn) {
    return Track(o, btn);
}

void Track::Audio(
    const float leftIn, const float rightIn, 
    float *leftOut, float *rightOut, 
    float *bufferLeft, float *bufferRight) {

    if(state == RECORDING) {
        *bufferLeft = leftIn;
        *bufferRight = rightIn;
    } 
    else if(state == MIXING) {
        *leftOut += *bufferLeft;
        *rightOut += *bufferRight;
        *bufferLeft += leftIn;
        *bufferRight += rightIn;
    } 
    else if(state == PLAYING) {
        *leftOut += *bufferLeft;
        *rightOut += *bufferRight;
    }
}

void Track::Loop() {
    state = nextState;
    if(state == RECORDING || state == MIXING)
        nextState = PLAYING;
}

DualLedButton *Track::GetButton() {
    return button;
}

void Track::Check(bool clear, bool record)
{
    bool clicked = button->Pressed();
    if(!clicked && clicked != previousButtonState) {
        if(!clear && !record) {
            if(state == PLAYING)
                nextState = MUTED;
            else if(state == MUTED)
                nextState = PLAYING;
            else if(state == BLANK)
                nextState = RECORDING;
        } 
        else if(!clear && record)
        {
            if(state == BLANK)
                nextState = RECORDING;
            else
                nextState = MIXING;
        }
        else if(clear && !record)
        {
            nextState = BLANK;
        }
        else if(clear && record)
        {
            nextState = RECORDING;
        }
    }
    previousButtonState = clicked;
}

#define THRESHOLD 0.5

void Track::Lights(float *level)
{
    switch(state)
    {
        case RECORDING:
        case MIXING:
            if(*level > THRESHOLD)
            {
                button -> Red();
                *level -= THRESHOLD * 2;
            }
            else
                button -> Off();
            break;
        case PLAYING:
            if(*level > THRESHOLD)
            {
                button -> Green();
                *level -= THRESHOLD * 2;
            }
            else
                button -> Off();
            break;
        case BLANK:
            button -> Off();
            break;
        case MUTED:
            button -> Off();
            break;
        case LOOPING:
        case CLEARING:
        case SUGGESTING:
            button -> Off();
            break;
    }
}
