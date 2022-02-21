#include "chardwaresound.h"
#include <iostream>
#include <QDebug>
#ifdef WIN32
#include <windows.h>
#endif


/*!
 * \brief MyPaStreamCallback This function is used to avoid C3867 error when passing the callback function to PaOpenStream
 */
TEST_LIB_EXPORT int MyPaStreamCallback (const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
{
     CHardwareSound *myClass = reinterpret_cast<CHardwareSound*>(userData);
     return myClass->PACallback(input, output, frameCount, timeInfo, statusFlags, myClass->GetUserData());
}

/*!
 * \brief CHardwareSound::CHardwareSound Default construtctor
 */
CHardwareSound::CHardwareSound()
{
    errCode = Pa_Initialize();
    checkError();
    errCode = Pa_OpenDefaultStream(&ioStream, 1, 2, paFloat32, 44100, paFramesPerBufferUnspecified, MyPaStreamCallback, this);
    checkError();
}

/*!
 * \brief CHardwareSound::CHardwareSound Constructor integrating shared buffers
 * \param p_in  Recorded data queue pointer
 * \param p_out Played data queue pointer
 */
CHardwareSound::CHardwareSound(QQueue<float>* p_in, QQueue<float>* p_out)
{
    errCode = Pa_Initialize();
    checkError();
    errCode = Pa_OpenDefaultStream(&ioStream, 1, 2, paFloat32, 44100, paFramesPerBufferUnspecified, MyPaStreamCallback, this);
    checkError();
    inHardwareBuffer = p_in;
    outHardwareBuffer = p_out;
}

/*!
 * \brief CHardwareSound::~CHardwareSound Default destructor
 */
CHardwareSound::~CHardwareSound()
{
    if(Pa_IsStreamActive(ioStream))
    {
        errCode = Pa_StopStream(ioStream);
        checkError();
    }
    errCode = Pa_CloseStream(ioStream);
    checkError();
    errCode = Pa_Terminate();
    checkError();
    return;
}

/*!
 * \brief CHardwareSound::checkError This function checks wether the operation was done correctly, prints the error otherwise
 */
void CHardwareSound::checkError()
{
    if(errCode != paNoError)
    {
        std::cout << "Error with Port Audio : " << Pa_GetErrorText(errCode);
        exit(1);
    }
}

/*!
 * \brief CHardwareSound::start This function is used to start the actual stream
 */
void CHardwareSound::start()
{
    errCode = Pa_StartStream(ioStream);
    checkError();
}

/*!
 * \brief CHardwareSound::PACallback This function is a routine (called periodically) that writes and reads 'directly' to and from the hardware devices
 * \param input This parametre is a pointer that points 'directly' to the microphone, this means that whatever your microphone records gets written to this pointer
 * \param output    This parametre is a pointer that points 'directly' to the speaker, this means that whatever you are putting in this memory location is going to be played
 * \param frameCount    This parametre defines the number of frames that
 * \param timeInfo  Structure containing current time and internal audio ADC/DAC timestamps
 * \param statusFlags   Flags provided by PortAudio to indicate status of the engine's internal buffers
 * \param userData  Field for custom data stuctures, this is where we will be able to access the external buffers
 * \return
 */
int CHardwareSound::PACallback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo *timeInfo, unsigned long statusFlags, void *userData) // Silences in case no data is received
{
        (void) output; /*Prevent unused variable warnings.*/
        (void) timeInfo;
        (void) statusFlags;
        (void) frameCount;
        (void) userData;

        SAMPLE *rptr = (SAMPLE*)input;      //Read pointer
        SAMPLE *wptr = (SAMPLE*)output;  //Write pointer


        /* Send and retreive samples to and from the shared buffers */
        ToInputStream(rptr, paFramesPerBufferUnspecified);
        FromOutStream(wptr, OPUS_BUFFER_SIZE);

        return 0;
}

/*!
 * \brief CHardwareSound::GetAvailableDevices This function prints the different available devices
 */
void CHardwareSound::GetAvailableDevices()
{
        int i, numDevices, defaultDisplayed;
        const PaDeviceInfo *deviceInfo;
        PaStreamParameters inputParameters, outputParameters;
        PaError err;

        printf( "PortAudio version: 0x%08X\n", Pa_GetVersion());

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

        //Pa_Terminate();

        printf("----------------------------------------------\n");
        return;
}

/*!
 * \brief CHardwareSound::PrintSupportedStandardSampleRate This function prints the supported standard sample rates (i.e.8000.0, 9600.0, 11025.0, 12000.0, 16000.0, 22050.0, 24000.0, 32000.0,
        44100.0, 48000.0, 88200.0, 96000.0, 192000)
 * \param inputParameters   Pointer to the input stream parameter
 * \param outputParameters Pointer to the output stream parameter
 */
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

/*!
 * \brief CHardwareSound::FromOutStream This function is used to grab data from the shared memory and give it to the callback function
 * \param p_out Pointer to the actual headphone buffer
 * \param p_size Size of the data to process
 */
void CHardwareSound::FromOutStream(SAMPLE* p_out, int p_size)
{
    if(outHardwareBuffer->size() < p_size)
        p_size = outHardwareBuffer->size();
    qDebug() << "Out hardware : " << outHardwareBuffer->size();
    for(int i = 0 ; i < p_size ; i++)
    {
        p_out[i] = outHardwareBuffer->dequeue();
    }
    return;
}

/*!
 * \brief CHardwareSound::ToInputStream This function is used to insert the microphone data into the shared buffer
 * \param p_in  Pointer to the actual microphone buffer
 * \param p_size Size of the data to process
 */
void CHardwareSound::ToInputStream(SAMPLE* p_in, int p_size)
{
    if(p_in)
        for(int i = 0 ; i < p_size ; i++)
        {
            userData->micBuffer->push_back(p_in [i]);
        }
    return;
}

/*!
 * \brief CHardwareSound::GetUserData
 * \return Returns the user data
 */
paUserData* CHardwareSound::GetUserData()
{
    return userData;
}
