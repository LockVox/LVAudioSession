#ifndef LVAUDIOSESSION_H
#define LVAUDIOSESSION_H
#include "config.h"

#include "opus.h"
#include "portaudio.h"

class TEST_LIB_EXPORT LVAudioSession
{
public:
    LVAudioSession();
    void GetInputSound();
};

#endif // LVAUDIOSESSION_H
