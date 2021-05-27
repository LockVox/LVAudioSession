#ifndef CHARDWARESOUND_H
#define CHARDWARESOUND_H
#include "pa_hostapi.h"
#include "config.h"
#include "portaudio.h"
#include "pa_asio.h"
#include <QQueue>


/**
  * This structure will be passed to the PortAudio callback function and allows to communicate with the LVAudioSession class
  * we do integrate these members because the PACallback function has to have a given format, refer to PortAudio documentation for further information
*/
typedef struct {
    QQueue<float>* micBuffer;       //Pointer to the shared buffer with OPUS
    QQueue<float>* headsetBuffer;//Pointer to the shared buffer with OPUS
     int micIndex;                          //Index to tell where we are in the local mic buffer
     int headsetIndex;                   //Index to tell where we are in the local headset buffer
     int maxFrameIndex;              //Number of maximum frames in local buffers
     SAMPLE* recordedSamples;   //An array of recorded samples
     SAMPLE* receivedSamples;   //an array of samples received from the network
}paUserData;

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

    void FromOutStream(SAMPLE* p_out);

    void ToInputStream(SAMPLE* p_in, int index);

private:
    PaStream *ioStream;
    PaError errCode;
    paUserData* userData;
};

#endif // CHARDWARESOUND_H
