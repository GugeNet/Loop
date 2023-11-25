#include <string.h>
#include "daisy_seed.h"
#include "daisysp.h"
#include <math.h>
#include "track.h"
#include "button.h"
#include "dualLedButton.h"

#define NUM_TRACKS 4


using namespace daisy;
using namespace daisy::seed;
using namespace daisysp;

static DaisySeed hw;

static float DSY_SDRAM_BSS buffer[16 * 1024 * 1024];

static uint32_t head;

static GPIO loopTrig;
static bool previousLoopTrig;

static const uint32_t shortClick = 10;
static const uint32_t longClick = 800;

static DualLedButton dualLedBtns[] = {
    DualLedButton::New(D20, shortClick, longClick, D22, D21),
    DualLedButton::New(D24, shortClick, longClick, D25, D23),
    DualLedButton::New(D27, shortClick, longClick, D26, D28),
    DualLedButton::New( D8, shortClick, longClick, D7, D9)
};

static Track tracks[] = {
    Track::New(0, &dualLedBtns[0]),
    Track::New(1, &dualLedBtns[1]),
    Track::New(2, &dualLedBtns[2]),
    Track::New(3, &dualLedBtns[3]),
};

static Button recordBtn = Button::New(D13, shortClick, longClick);
static Button clearBtn = Button::New(D14, shortClick, longClick);

static int loopBlink = 0;

void Loop()
{
    head = 0;
    for(int j = 0; j < NUM_TRACKS; j++)
    {
        tracks[j].Loop();
    }
    loopBlink = 480;
}

void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                   AudioHandle::InterleavingOutputBuffer out,
                   size_t                                size)
{
    //start = System::GetTick();
    //memcpy(ram, in, sizeof(float) * size);
    //for(int i = 0; i < 300; i++)
    //{
    //    memcpy((i + 1) * size + ram, ram, sizeof(float) * size);
    //}
    //end = System::GetTick();
    //dur = (end - start) / 200; // us
    memcpy(out, in, sizeof(float) * size);

    if(previousLoopTrig != loopTrig.Read()) {
        previousLoopTrig = !previousLoopTrig;

        if(previousLoopTrig)
            Loop();
    }

    bool recordChanged = recordBtn.Check();

    bool clearChanged = clearBtn.Check();

    for(size_t i = 0; i < size; i += 2) {
        for(int j = 0; j < NUM_TRACKS; j++)
        {
            tracks[j].Audio(in[i], in[i+1], &out[i], &out[i+1], (float*)&buffer[head], (float*)&buffer[head+1]);
            head+=2;
            if(loopBlink > 0)
                tracks[j].GetButton()->Color(1 + loopBlink % 2);
        }
        if(loopBlink > 0)
            loopBlink--;
    }

    for(int j = 0; j < NUM_TRACKS; j++)
    {
        tracks[j].Check(clearBtn.GetPressed(), recordBtn.GetPressed());
    }

    //state = UpdateMainState(record, clear, remainingTime);

        //tracks[i].UpdateTrackState(state, remainingTime);

    if(head >= 16 * 1024 * 1024)
        Loop();
}

int main(void)
{
    // Initialize Hardware
    hw.Init();

    memset((float*)0xC0000000, 0, 64 * 1024 * 1024);

    loopTrig.Init(D15, GPIO::Mode::INPUT, GPIO::Pull::PULLDOWN);
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

        System::Delay(300);
    }

    hw.StartAudio(AudioCallback);

    for(;;) {}
}
