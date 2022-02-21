#include "lvaudiosession.h"
#include "chardwaresound.h"
#include "ccodec.h"
#include <QtCore>
#include <QDebug>

int main()
{
    QQueue<float>* mic = new QQueue<float>;
    QQueue<float>* speaker = new QQueue<float>;
    QQueue<char>* cdc_buf = new QQueue<char>;
    CHardwareSound hwsound(mic, speaker);
    CCodec opus_cdc(mic, speaker, cdc_buf, cdc_buf);
    //hwsound.GetAvailableDevices();
    hwsound.start();
    // Main loop for playback
    qDebug() << "loop";
    while(true)
    {
        qDebug() << "loop";
        opus_cdc.EncodeFromHardware();
        opus_cdc.DecodeToHardware();
        Pa_Sleep(100);
    }
    free(mic);
    free(speaker);
    return 0;
}

// Stacking in speaker
