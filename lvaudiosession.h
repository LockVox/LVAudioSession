#ifndef LVAUDIOSESSION_H
#define LVAUDIOSESSION_H
#include "config.h"
#include "opus.h"
#include "portaudio.h"
#include <QQueue>

class TEST_LIB_EXPORT LVAudioSession
{
public:
    LVAudioSession();
    void GetInputSound();
private:
    QQueue<float> inputBuffer;
    QQueue<float> outputBuffer;
};

#endif // LVAUDIOSESSION_H
