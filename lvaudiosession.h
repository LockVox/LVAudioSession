#ifndef LVAUDIOSESSION_H
#define LVAUDIOSESSION_H
#include "config.h"

#include "chardwaresound.h"
#include "ccodec.h"
#include <QQueue>

class TEST_LIB_EXPORT LVAudioSession
{
public:
    LVAudioSession();
    void GetInputSound();

    CCodec m_codec;
    CHardwareSound m_periph;
    /* Shared buffers */
    QQueue<float>* micBuffer;
    QQueue<float>* headphoneBuffer;

    QQueue<unsigned char>* toInetBuffer;
    QQueue<unsigned char>* fromInetBuffer;
};

#endif // LVAUDIOSESSION_H
