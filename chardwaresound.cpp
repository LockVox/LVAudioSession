#include "chardwaresound.h"
#include <iostream>
#ifdef WIN32
#include <windows.h>
#endif

CHardwareSound::CHardwareSound()
{
    errCode = Pa_Initialize();
    checkError();
    errCode = Pa_OpenDefaultStream(&ioStream, 1, 2, paFloat32, 44100, paFramesPerBufferUnspecified, PACallback, NULL);
    checkError();
}

CHardwareSound::~CHardwareSound()
{
    errCode = Pa_StopStream(ioStream);
    checkError();
    errCode = Pa_CloseStream(ioStream);
    checkError();
    errCode = Pa_Terminate();
    checkError();
    return;
}

void CHardwareSound::checkError()
{
    if(errCode != paNoError)
    {
        std::cout << "Error with Port Audio : " << Pa_GetErrorText(errCode);
        exit(1);
    }
}

void CHardwareSound::start()
{
    errCode = Pa_StartStream(ioStream);
    checkError();
}

int CHardwareSound::PACallback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
{
    return 0;
}

void CHardwareSound::GetAvailableDevices()
{
        int i, numDevices, defaultDisplayed;
        const PaDeviceInfo *deviceInfo;
        PaStreamParameters inputParameters, outputParameters;
        PaError err;

        printf( "PortAudio version: 0x%08X\n", Pa_GetVersion());
        printf( "Version text: '%s'\n", Pa_GetVersionInfo()->versionText );

        numDevices = Pa_GetDeviceCount();
        if( numDevices < 0 )
        {
            printf( "ERROR: Pa_GetDeviceCount returned 0x%x\n", numDevices );
            errCode = numDevices;
            checkError();
        }

        printf( "Number of devices = %d\n", numDevices );
        for( i=0; i<numDevices; i++ )
        {
            deviceInfo = Pa_GetDeviceInfo( i );
            printf( "--------------------------------------- device #%d\n", i );

        /* Mark global and API specific default devices */
            defaultDisplayed = 0;
            if( i == Pa_GetDefaultInputDevice() )
            {
                printf( "[ Default Input" );
                defaultDisplayed = 1;
            }
            else if( i == Pa_GetHostApiInfo( deviceInfo->hostApi )->defaultInputDevice )
            {
                const PaHostApiInfo *hostInfo = Pa_GetHostApiInfo( deviceInfo->hostApi );
                printf( "[ Default %s Input", hostInfo->name );
                defaultDisplayed = 1;
            }

            if( i == Pa_GetDefaultOutputDevice() )
            {
                printf( (defaultDisplayed ? "," : "[") );
                printf( " Default Output" );
                defaultDisplayed = 1;
            }
            else if( i == Pa_GetHostApiInfo( deviceInfo->hostApi )->defaultOutputDevice )
            {
                const PaHostApiInfo *hostInfo = Pa_GetHostApiInfo( deviceInfo->hostApi );
                printf( (defaultDisplayed ? "," : "[") );
                printf( " Default %s Output", hostInfo->name );
                defaultDisplayed = 1;
            }

            if( defaultDisplayed )
                printf( " ]\n" );

        /* print device info fields */
    #ifdef WIN32
            {   /* Use wide char on windows, so we can show UTF-8 encoded device names */
                wchar_t wideName[260];
                MultiByteToWideChar(CP_UTF8, 0, deviceInfo->name, -1, wideName, MAX_PATH-1);
                wprintf( L"Name                        = %s\n", wideName );
            }
    #else
            printf( "Name                        = %s\n", deviceInfo->name );
    #endif
            printf( "Host API                    = %s\n",  Pa_GetHostApiInfo( deviceInfo->hostApi )->name );
            printf( "Max inputs = %d", deviceInfo->maxInputChannels  );
            printf( ", Max outputs = %d\n", deviceInfo->maxOutputChannels  );

            printf( "Default low input latency   = %8.4f\n", deviceInfo->defaultLowInputLatency  );
            printf( "Default low output latency  = %8.4f\n", deviceInfo->defaultLowOutputLatency  );
            printf( "Default high input latency  = %8.4f\n", deviceInfo->defaultHighInputLatency  );
            printf( "Default high output latency = %8.4f\n", deviceInfo->defaultHighOutputLatency  );

    #ifdef WIN32
    #ifdef PA_USE_ASIO
    /* ASIO specific latency information */
            if( Pa_GetHostApiInfo( deviceInfo->hostApi )->type == paASIO ){
                long minLatency, maxLatency, preferredLatency, granularity;

                err = PaAsio_GetAvailableLatencyValues( i,
                        &minLatency, &maxLatency, &preferredLatency, &granularity );

                printf( "ASIO minimum buffer size    = %ld\n", minLatency  );
                printf( "ASIO maximum buffer size    = %ld\n", maxLatency  );
                printf( "ASIO preferred buffer size  = %ld\n", preferredLatency  );

                if( granularity == -1 )
                    printf( "ASIO buffer granularity     = power of 2\n" );
                else
                    printf( "ASIO buffer granularity     = %ld\n", granularity  );
            }
    #endif /* PA_USE_ASIO */
    #endif /* WIN32 */

            printf( "Default sample rate         = %8.2f\n", deviceInfo->defaultSampleRate );

        /* poll for standard sample rates */
            inputParameters.device = i;
            inputParameters.channelCount = deviceInfo->maxInputChannels;
            inputParameters.sampleFormat = paInt16;
            inputParameters.suggestedLatency = 0; /* ignored by Pa_IsFormatSupported() */
            inputParameters.hostApiSpecificStreamInfo = NULL;

            outputParameters.device = i;
            outputParameters.channelCount = deviceInfo->maxOutputChannels;
            outputParameters.sampleFormat = paInt16;
            outputParameters.suggestedLatency = 0; /* ignored by Pa_IsFormatSupported() */
            outputParameters.hostApiSpecificStreamInfo = NULL;

            if( inputParameters.channelCount > 0 )
            {
                printf("Supported standard sample rates\n for half-duplex 16 bit %d channel input = \n",
                        inputParameters.channelCount );
                PrintSupportedStandardSampleRate( &inputParameters, NULL );
            }

            if( outputParameters.channelCount > 0 )
            {
                printf("Supported standard sample rates\n for half-duplex 16 bit %d channel output = \n",
                        outputParameters.channelCount );
                PrintSupportedStandardSampleRate( NULL, &outputParameters );
            }

            if( inputParameters.channelCount > 0 && outputParameters.channelCount > 0 )
            {
                printf("Supported standard sample rates\n for full-duplex 16 bit %d channel input, %d channel output = \n",
                        inputParameters.channelCount, outputParameters.channelCount );
                PrintSupportedStandardSampleRate( &inputParameters, &outputParameters );
            }
        }

        Pa_Terminate();

        printf("----------------------------------------------\n");
        return;
}

void CHardwareSound::PrintSupportedStandardSampleRate(const PaStreamParameters *inputParameters, const PaStreamParameters *outputParameters )
{
    static double standardSampleRates[] = {
        8000.0, 9600.0, 11025.0, 12000.0, 16000.0, 22050.0, 24000.0, 32000.0,
        44100.0, 48000.0, 88200.0, 96000.0, 192000.0, -1 /* negative terminated  list */
    };
    int     i, printCount;
    PaError err;

    printCount = 0;
    for( i=0; standardSampleRates[i] > 0; i++ )
    {
        err = Pa_IsFormatSupported( inputParameters, outputParameters, standardSampleRates[i] );
        if( err == paFormatIsSupported )
        {
            if( printCount == 0 )
            {
                printf( "\t%8.2f", standardSampleRates[i] );
                printCount = 1;
            }
            else if( printCount == 4 )
            {
                printf( ",\n\t%8.2f", standardSampleRates[i] );
                printCount = 1;
            }
            else
            {
                printf( ", %8.2f", standardSampleRates[i] );
                ++printCount;
            }
        }
    }
    if( !printCount )
        printf( "None\n" );
    else
        printf( "\n" );
}
