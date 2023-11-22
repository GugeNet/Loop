#include <string.h>
#include "daisy_seed.h"
#include "daisysp.h"
#include <math.h>
#include "track.h"
#include "button.h"
#include "dualLedButton.h"

// 1MB test
#define TEST_BUFF_SIZE (1024 * 1024)

using namespace daisy;
using namespace daisy::seed;
using namespace daisysp;


//static uint32_t DSY_SDRAM_BSS test_buff[TEST_BUFF_SIZE];

static DaisySeed hw;

static float *ram = (float *)0xC0000000; // SDRAM


static GPIO loopTrig;
static bool previousLoopTrig;

static uint32_t  start, end, dur;

static const uint32_t shortClick = 50;
static const uint32_t longClick = 400;


static DualLedButton dualLedBtns[] = {
    DualLedButton::New(D20, shortClick, longClick, D22, D21),
    DualLedButton::New(D24, shortClick, longClick, D23, D25),
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

void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                   AudioHandle::InterleavingOutputBuffer out,
                   size_t                                size)
{
    start = System::GetTick();
    memcpy(ram, in, sizeof(float) * size);
    for(int i = 0; i < 300; i++)
    {
        memcpy((i + 1) * size + ram, ram, sizeof(float) * size);
    }
    end = System::GetTick();
    dur = (end - start) / 200; // us
    memcpy(out, in, sizeof(float) * size);

    bool recordChanged = recordBtn.Check();

    bool clearChanged = clearBtn.Check();

    //state = UpdateMainState(record, clear, remainingTime);

    for(int i = 0; i < 4; i++)
    {
        //tracks[i].UpdateTrackState(state, remainingTime);

        tracks[i].Audio(in, out, size, ram);
    }

}

int main(void)
{
    // Initialize Hardware
    hw.Init();

    loopTrig.Init(D15, GPIO::Mode::INPUT, GPIO::Pull::PULLDOWN);

    hw.SetAudioBlockSize(4);

    while(1) {

        if(recordBtn.Check())
        {

            dualLedBtns[0].Off();

            System::Delay(500);


            if(recordBtn.GetPressed())
                dualLedBtns[0].Red();
            else
                dualLedBtns[0].Green();
        }

    }
}
