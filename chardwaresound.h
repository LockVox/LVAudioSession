#ifndef CHARDWARESOUND_H
#define CHARDWARESOUND_H
#include "pa_hostapi.h"
#include "config.h"
#include "portaudio.h"
#include "pa_asio.h"

class TEST_LIB_EXPORT CHardwareSound
{
public:
    CHardwareSound();
    ~CHardwareSound();
    void start();
    void checkError();
    static int PACallback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData );
    void GetAvailableDevices();
    static void PrintSupportedStandardSampleRate(const PaStreamParameters *inputParameters, const PaStreamParameters *outputParameters);
private:
    PaStream *ioStream;
    PaError errCode;
};

#endif // CHARDWARESOUND_H
