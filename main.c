#include "playAudio.h"
#include "portaudio.h"
#include "wavHandler.h"
#include <stdio.h>
#include <stdlib.h>

static paTestData data;

int main(int argc, char** argv){
    PaError err = Pa_Initialize();
    if( err != paNoError ) printf(  "\n\n\nPortAudio error: %s\n", Pa_GetErrorText( err ) );
    
    // List available devices.
    int numDevices = Pa_GetDeviceCount();
    if (numDevices < 0) {
        fprintf(stderr, "ERROR: Pa_CountDevices returned 0x%x\n", numDevices);
    }
    for (int i = 0; i < numDevices; ++i) {
        const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(i);
        printf("Device %d: %s\n", i, deviceInfo->name);
    }

    WavInfo* w = malloc(sizeof(WavInfo));
    readHeader("./test.wav", w);

    printf("\n\n\n%i\n\n\n",w->sampleSize);

    PaStream *stream;
    
    /* Open an audio I/O stream. */
    err = Pa_OpenDefaultStream( &stream,
                                0,          /* no input channels */
                                w->channels,          /* stereo output */
                                paInt16,  /* 16 bit int output */
                                w->sampleRate,
                                256,        /* frames per buffer, i.e. the number
                                                   of sample frames that PortAudio will
                                                   request from the callback. Many apps
                                                   may want to use
                                                   paFramesPerBufferUnspecified, which
                                                   tells PortAudio to pick the best,
                                                   possibly changing, buffer size.*/
                                patestCallback, /* this is your callback function */
                                w ); /*This is a pointer that will be passed to
                                                   your callback*/
    err = Pa_StartStream( stream );
    Pa_Sleep(3000);
    err = Pa_StopStream( stream );
    // Close portAudio.
    err = Pa_Terminate();
    if( err != paNoError) printf(  "\n\n\nPortAudio error closing driver: %s\n", Pa_GetErrorText( err ) );
}
