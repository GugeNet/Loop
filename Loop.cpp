#include <string.h>
#include "daisy_seed.h"
#include "daisysp.h"
#include <math.h>
#include "track.h"
#include "dualLedButton.h"

#define NUM_TRACKS 4

using namespace daisy;
using namespace daisy::seed;
using namespace daisysp;

static DaisySeed hw;

static float DSY_SDRAM_BSS buffer[16 * 1024 * 1024];

static uint32_t head = 0;

static Switch clear, record;

static GPIO loopTrig;
static bool previousLoopTrig;

static const uint32_t shortClick = 10;
static const uint32_t longClick = 800;

static DualLedButton dualLedBtns[4];

static Track tracks[] = {
    Track::New(0, &dualLedBtns[0]),
    Track::New(1, &dualLedBtns[1]),
    Track::New(2, &dualLedBtns[2]),
    Track::New(3, &dualLedBtns[3]),
};

static int loopBlink = 0;

void Loop()
{
    head = 0;
    for(int j = 0; j < NUM_TRACKS; j++)
    {
        tracks[j].Loop();
    }
    loopBlink = 64;
}

void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                   AudioHandle::InterleavingOutputBuffer out,
                   size_t                                size)
{
    memcpy(out, in, sizeof(float) * size);

    for(size_t i = 0; i < size; i += 2) {
        for(int j = 0; j < NUM_TRACKS; j++)
        {
            tracks[j].Audio(in[i], in[i+1], &out[i], &out[i+1], (float*)&buffer[head], (float*)&buffer[head+1]);
            head+=2;
        }
    }

    if(head >= 16 * 1024 * 1024)
        Loop();
}

int main(void)
{
    // Initialize Hardware
    hw.Configure();
    hw.Init();

    float samplerate = hw.AudioSampleRate();

    clear.Init(D14, samplerate/48.f, Switch::TYPE_MOMENTARY, Switch::POLARITY_INVERTED, Switch::PULL_UP);
    record.Init(D15, samplerate/48.f, Switch::TYPE_MOMENTARY, Switch::POLARITY_INVERTED, Switch::PULL_UP);

    dualLedBtns[0].Init(D19, samplerate/48.f, D17, D13);
    dualLedBtns[1].Init(D20, samplerate/48.f, D22, D21);
    dualLedBtns[2].Init(D27, samplerate/48.f, D26, D28);
    dualLedBtns[3].Init(D1,  samplerate/48.f, D3,  D2);

    memset((float*)0xC0000000, 0, 64 * 1024 * 1024);

    loopTrig.Init(D16, GPIO::Mode::INPUT, GPIO::Pull::PULLDOWN);
    previousLoopTrig = loopTrig.Read();

    hw.SetAudioBlockSize(4);

    for(int j = 0; j < 6; j++)
    {
        for(int i = 0; i < 4; i++)
        {
            if(i == j)
                dualLedBtns[i].Red();
            else if(i == j-1)
                dualLedBtns[i].Green();
            else
                dualLedBtns[i].Off();
        }

        System::Delay(200);
    }

    hw.StartAudio(AudioCallback);

    while(1)
    {
        if(previousLoopTrig != loopTrig.Read()) {
            previousLoopTrig = !previousLoopTrig;

            if(previousLoopTrig)
                Loop();
        }

        record.Debounce();
        clear.Debounce();

        for(int i=0; i<NUM_TRACKS; i++)
            dualLedBtns[i].Debounce();
        
        for(int i=0; i<NUM_TRACKS; i++)
        {
            tracks[i].Check(clear.Pressed(), record.Pressed());
            if(loopBlink > 0 && loopBlink % 3 == 0)
                tracks[i].GetButton()->Color(1 + loopBlink % 2);
            else
                tracks[i].Lights();
        }

        if(loopBlink > 0)
            loopBlink--;

        System::Delay(1);
    }
}

