#ifndef CCODEC_H
#define CCODEC_H

#include "config.h"
#include "opus.h"
#include "opus_defines.h"
#include "opus_types.h"
#include <QQueue>

/*!
 * \brief The CCodec class is used to encode and decode+mix audio streams
 */
class TEST_LIB_EXPORT CCodec
{
public:

    CCodec();
    ~CCodec();

    void EncodeFromHardware();   //Encode data to unsigned char format and empties queue buffer
    void DecodeToHardware();      //Decode data to float format and fill queue buffer


    float* from_pFloat(int size);     //From local float pointer
    void to_pFloat(float* p_out, int size);  //Writes to the outHardwareBuffer
    const unsigned char* from_pChar(int size);
    void to_pChar(unsigned char* p_in, int size); //From local  char pointer

private:
    OpusEncoder* opusenc;
    OpusDecoder* opusdec;
    int errCode;
    QQueue<float>* inHardwareBuffer;         //Shared buffer with PA component
    QQueue<float>* outHardwareBuffer;       //Shared buffer with PA component
    QQueue<char>* inNetBuffer;                  //Shared buffer with RTP component
    QQueue<char>* outNetBuffer;                //Shared buffer with RTP component
    const int bufsize = OPUS_BUFFER_SIZE;
    int max_outsize = OPUS_BUFFER_SIZE;
    int in_index;
    int out_index;
};

#endif // CCODEC_H
