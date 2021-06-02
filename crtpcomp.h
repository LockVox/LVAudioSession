#ifndef CRTPCOMP_H
#define CRTPCOMP_H

#include "config.h"
#include "rtpsession.h"
#include "rtpsessionparams.h"
#include <QQueue>

class CRTPComp
{
public:
    CRTPComp();

private:
    QQueue<float*> toInternet;
    QQueue<float*> fromInternet;
};

#endif // CRTPCOMP_H
