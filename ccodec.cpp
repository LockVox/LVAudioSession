#include "ccodec.h"

CCodec::CCodec()    //Should never be called, because queues won't be linked to other part of the program
{
    errCode=0;
    opusenc = opus_encoder_create(48000, 1, OPUS_APPLICATION_VOIP, &errCode);
    opusdec = opus_decoder_create(48000, 2, &errCode);
    inHardwareBuffer = NULL;
    outHardwareBuffer = NULL;
    inNetBuffer = NULL;
    outNetBuffer = NULL;
    //Should never be called.

}

/*!
 * \brief CCodec::CCodec    Overloaded function allowing shared buffer linking
 * \param p_mic             Pointer to the hardware mic
 * \param p_speaker         Pointer to the hardware speaker
 * \param p_toInet          Pointer to outgoing voice
 * \param p_fromInet        Pointer to incoming voice
 */
CCodec::CCodec(QQueue<float>* p_mic, QQueue<float>* p_speaker, QQueue<char>* p_toInet,QQueue<char>* p_fromInet)
{
    inHardwareBuffer = p_mic;
    outHardwareBuffer = p_speaker;
    outNetBuffer = p_toInet;
    inNetBuffer = p_fromInet;
    errCode=0;
    opusenc = opus_encoder_create(48000, 1, OPUS_APPLICATION_VOIP, &errCode);
    opusdec = opus_decoder_create(48000, 2, &errCode);

}

/*!
 * \brief CCodec::~CCodec Default class destructor
 */
CCodec::~CCodec()
{
    opus_encoder_destroy(opusenc);
    opus_decoder_destroy(opusdec);
    if(inHardwareBuffer)
        free(inHardwareBuffer);
    if(outHardwareBuffer)
        free(outHardwareBuffer);
    if(outNetBuffer)
        free(outNetBuffer);
    if(inNetBuffer)
        free(inNetBuffer);
}

/*!
 * \brief CCodec::EncodeFromHardware This function is used to encode raw data to opus format
 */
void CCodec::EncodeFromHardware()
{
    static unsigned char* out = NULL;
    int outsize = 0;
    if(!out)    // Allocate if not already done
        out = (unsigned char*)calloc(max_outsize, sizeof(unsigned char));
    if(inHardwareBuffer->size() < bufsize)
        bufsize = inHardwareBuffer->size();
    outsize = opus_encode_float(opusenc,  from_pFloat(bufsize), bufsize, out, max_outsize);
    to_pChar(out, outsize);     //Push to the RTP shared buffer
}

/*!
 * \brief CCodec::DecodeToHardware This function is used to decode opus data to raw format
 */
void CCodec::DecodeToHardware()
{
    static float* out = NULL;
    int outsize = 0;
    if(!out)
    {
        out = (float*)calloc(max_outsize, sizeof(float));
        return;
    }
    outsize = opus_decode_float(opusdec, from_pChar(bufsize), bufsize, out, max_outsize/2, 1);
    for(int i = 0 ; i < outsize ; i++)
    {
        outHardwareBuffer->append(out[i]);
    }
}

/*!
 * \brief CCodec::from_pFloat This function is used to grab data from the shared buffer with PA
 * \param size Size of the data to return
 * \return Data grabbed from the shared buffer
 */
float* CCodec::from_pFloat(int size)
{
    static float* retval = NULL;
    if(!retval)
        retval = (float*)calloc(size, sizeof(float));
    if(size > inHardwareBuffer->size())
        size = inHardwareBuffer->size();
    for(int i = 0 ; i < size ; i++)
        retval[i] = inHardwareBuffer->dequeue();
    return retval;
}

/*!
 * \brief CCodec::to_pFloat This function is used to insert data to the PA shared buffer
 * \param p_in Pointer to the local data
 * \param size Size of the data to move
 */
void CCodec::to_pFloat(float *p_in, int size)
{
    for(int i=0 ; i< size; i++)
    {
        outHardwareBuffer->append(p_in[(i+out_index)%max_outsize]);
    }
}


/*!
 * \brief CCodec::from_pChar This function is used to grab data from the RTP shared buffer
 * \return Returns a pointer to the data
 */
const unsigned char* CCodec::from_pChar(int size)
{
    if(!inNetBuffer)
        return NULL;
    static unsigned char* in = NULL;
    if(!in)
        in = (unsigned char *)calloc(bufsize, sizeof(unsigned char));
    if(size > inNetBuffer->size())
        size = inNetBuffer->size();
    for(int i=0;i<size;i++)
        in[i] = inNetBuffer->dequeue();
    return NULL;
}

/*!
 * \brief CCodec::to_pChar This function is used to insert data to the RTP shared buffer
 * \param p_in  Pointer to the local buffer
 * \param size Size of the data insert
 */
void CCodec::to_pChar(unsigned char* p_in, int size)
{
    if(!outNetBuffer)
        return;
    for(int i=0 ; i<size ; i++)
    {
        outNetBuffer->append(p_in[(i+out_index)%max_outsize]);    //Append at the right index in the ring buffer
        out_index++;
        if(out_index >= max_outsize)
            out_index = 0;
    }
}


