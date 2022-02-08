#include "lvaudiosession.h"
#include "chardwaresound.h"
#include <QtCore>

int main(int argc, char** argv)
{
    CHardwareSound hwsound;
    hwsound.GetAvailableDevices();
    hwsound.start();
    argc=0;
    argv = NULL;
    return 0;
}
