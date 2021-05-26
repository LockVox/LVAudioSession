#include "ccodec.h"

CCodec::CCodec()    //Should never be called
{
    errCode=0;
    opusenc = opus_encoder_create(48000, 1, OPUS_APPLICATION_VOIP, &errCode);
    opusdec = opus_decoder_create(48000, 2, &errCode);
    /*inBuffer = new QQueue<float>;
    outBuffer = new QQueue<char>;*/
    //Should never be called.

}

CCodec::~CCodec()
{

}

/*
 *  @brief  This function is used to encode raw audio received from PA Engine
 *  @params none
*/
void CCodec::EncodeFromHardware()
{
    static  unsigned char* out = NULL;
    int outsize = 0;
    if(!out)
        out = (unsigned char*)calloc(max_outsize, sizeof(char));
    outsize = opus_encode_float(opusenc,  from_pFloat(), bufsize, out, max_outsize);
    to_pChar(out, outsize);
}

void CCodec::DecodeToHardware()
{
    static float* out = NULL;
    int outsize = 0;
    if(!out)
        out = (float*)calloc(max_outsize, sizeof(float));
    outsize = opus_decode_float(opusdec, from_pChar(), bufsize, out, max_outsize/2, 0);
}

const float* CCodec::from_pFloat()
{
    return NULL;
}

void CCodec::to_pFloat(float *p_in, int size)
{
    for(int i=0 ; i< size; i++)
    {
        (*outHardwareBuffer).append(p_in[(i+out_index)%max_outsize]);
    }
}

const unsigned char* CCodec::from_pChar()
{
    return NULL;
}


void CCodec::to_pChar(unsigned char* p_in, int size)
{
    for(int i=0 ; i<size ; i++)
    {
        (*outNetBuffer).append(p_in[(i+out_index)%max_outsize]);    //Append at the right index in the ring buffer
        out_index++;
        if(out_index >= max_outsize)
            out_index = 0;
    }
}


