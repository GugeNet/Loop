#include <string.h>
#include "daisy_seed.h"
#include "daisysp.h"
#include <math.h>

// 1MB test
#define TEST_BUFF_SIZE (1024 * 1024)

using namespace daisy;
using namespace daisy::seed;
using namespace daisysp;

static enum TrackState {

    Blank = 0,
    Playing = 1,
    Recording = 2,
    Looping = 3,
    Clearing = 4,
    SuggestingClear = 5,
    SuggestingRecord = 6
} trackState[4];

static uint32_t DSY_SDRAM_BSS test_buff[TEST_BUFF_SIZE];

static DaisySeed hw;

static int32_t *ram = (int32_t *)0xC0000000; // SDRAM


static GPIO loopTrig;
static bool previousLoopTrig;

static GPIO record, clear, track1, track2, track3, track4;
static bool previousRecord = false, 
            previousClear = false, 
            previousTrack1 = false, 
            previousTrack2 = false, 
            previousTrack3 = false,
            previousTrack4 = false;

static GPIO track1greenLed, track1redLed, 
            track2greenLed, track2redLed,
            track3greenLed, track3redLed,
            track4greenLed, track4redLed;

static uint32_t  start, end, dur;



void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                   AudioHandle::InterleavingOutputBuffer out,
                   size_t                                size)
{
    start = System::GetTick();
    //	int32_t* ram = (int32_t*)0x20000000; // DTCM
    //	int32_t* ram = (int32_t*)0x38000000; // D3 RAM
    int32_t *ram = (int32_t *)0xC0000000; // SDRAM
    memcpy(ram, in, sizeof(int32_t) * size);
    for(int i = 0; i < 300; i++)
    {
        memcpy((i + 1) * size + ram, ram, sizeof(int32_t) * size);
    }
    end = System::GetTick();
    dur = (end - start) / 200; // us
    memcpy(out, in, sizeof(float) * size);



}

void passthru(AudioHandle::InterleavingInputBuffer  in,
              AudioHandle::InterleavingOutputBuffer out,
              size_t                                size)
{
    memcpy(out, in, sizeof(float) * size);
}

int main(void)
{
    // Initialize Hardware
    hw.Init();

    record.Init(D13, GPIO::Mode::INPUT, GPIO::Pull::PULLDOWN);
    clear.Init(D14, GPIO::Mode::INPUT, GPIO::Pull::PULLDOWN);
    track1.Init(D20, GPIO::Mode::INPUT, GPIO::Pull::PULLDOWN);
    track2.Init(D24, GPIO::Mode::INPUT, GPIO::Pull::PULLDOWN);
    track3.Init(D27, GPIO::Mode::INPUT, GPIO::Pull::PULLDOWN);
    track4.Init(D8, GPIO::Mode::INPUT, GPIO::Pull::PULLDOWN);

    track1greenLed.Init(D21, GPIO::Mode::OUTPUT);
    track1redLed.Init(D22, GPIO::Mode::OUTPUT);
    track2greenLed.Init(D25, GPIO::Mode::OUTPUT);
    track2redLed.Init(D23, GPIO::Mode::OUTPUT);
    track3greenLed.Init(D28, GPIO::Mode::OUTPUT);
    track3redLed.Init(D26, GPIO::Mode::OUTPUT);
    track4greenLed.Init(D9, GPIO::Mode::OUTPUT);
    track4redLed.Init(D7, GPIO::Mode::OUTPUT);

    loopTrig.Init(D15, GPIO::Mode::INPUT, GPIO::Pull::PULLDOWN);

    hw.SetAudioBlockSize(4);
    for(uint32_t i = 0; i < TEST_BUFF_SIZE; i++)
    {
        test_buff[i] = i;
    }
    for(uint32_t i = 0; i < TEST_BUFF_SIZE; i++)
    {
        if(test_buff[i] != i)
        {
            asm("bkpt 255");
        }
    }

    bool red = false;

    while(1) {

        if(record.Read() != previousRecord)
        {
            track1greenLed.Write(false);
            track1redLed.Write(false);

            System::Delay(500);

            track1greenLed.Write(previousRecord);
            track1redLed.Write(!previousRecord);


            previousRecord = !previousRecord;
        }

    }
}
