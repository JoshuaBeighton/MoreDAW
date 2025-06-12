#include "playAudio.h"
#include "audioManager.h"
#include <stdio.h>

static paTestData data;

PaStream *initialise(WavInfo *w)
{
    PaError err = Pa_Initialize();

    PaStream *stream;

    if (err != paNoError)
        printf("\n\n\nPortAudio error: %s\n", Pa_GetErrorText(err));
    PaSampleFormat sf;

    if (w->sampleSize == 16)
    {
        sf = paInt16;
    }
    else if (w->sampleSize == 24)
    {
        sf = paInt24;
    }
    else if (w->sampleSize == 32)
    {
        sf = paInt32;
    }
    /* Open an audio I/O stream. */
    err = Pa_OpenDefaultStream(&stream,
                               0,           /* no input channels */
                               w->channels, /* stereo output */
                               sf,          /* 16 bit int output */
                               w->sampleRate,
                               256,            /* frames per buffer, i.e. the number
                                                      of sample frames that PortAudio will
                                                      request from the callback. Many apps
                                                      may want to use
                                                      paFramesPerBufferUnspecified, which
                                                      tells PortAudio to pick the best,
                                                      possibly changing, buffer size.*/
                               patestCallback, /* this is your callback function */
                               w);             /*This is a pointer that will be passed to
                                                             your callback*/
    return stream;
}

void playFile(PaStream *stream, WavInfo *w)
{
    PaError err = Pa_Initialize();

    // If Audio is still being played, do nothing.
    if (Pa_IsStreamActive(stream) == 1)
    {
        printf("Stream is active so returning!\n");
        return;
    }

    err = Pa_StopStream(stream);
    if (w->currentPointer - w->bulkData == w->dataSize)
        w->currentPointer = w->bulkData;
    err = Pa_StartStream(stream);
    if (err != paNoError)
        printf("\n\n\nPortAudio error in playback: %s\n", Pa_GetErrorText(err));
}

void pauseAudio(PaStream *stream, WavInfo *w)
{
    if (Pa_IsStreamActive(stream) == 1)
    {
        PaError err = Pa_Initialize();
        err = Pa_StopStream(stream);
        if (err != paNoError)
            printf("\n\n\nPortAudio error in pausing: %s\n", Pa_GetErrorText(err));
    }
}

void stopAudio(PaStream *stream, WavInfo *w)
{
    if (Pa_IsStreamActive(stream) == 1)
    {
        PaError err = Pa_Initialize();
        err = Pa_StopStream(stream);
        w->currentPointer = w->bulkData;
        if (err != paNoError)
            printf("\n\n\nPortAudio error in stopping: %s\n", Pa_GetErrorText(err));
    }
}

void closeAudioManager()
{
    // Close portAudio.
    PaError err = Pa_Terminate();
    if (err != paNoError)
        printf("\n\n\nPortAudio error closing driver: %s\n", Pa_GetErrorText(err));
}