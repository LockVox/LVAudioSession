#ifndef CCODEC_H
#define CCODEC_H

#include "config.h"
#include "opus.h"

class TEST_LIB_EXPORT CCodec
{
public:

    CCodec();
    ~CCodec();

private:
    OpusEncoder* opusenc;
};

#endif // CCODEC_H
