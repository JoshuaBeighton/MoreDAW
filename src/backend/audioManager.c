#include "audioManager.h"

float cursorPosition;
/**
 * Initialise the stream.
 */
PaStream *initialise(WavInfo *w)
{
    // Initialise Port Audio
    PaError err = Pa_Initialize();

    // Get a pointer to a stream.
    PaStream *stream;

    // If there was an error, print it out.
    if (err != paNoError)
        printf("\n\n\nPortAudio error: %s\n", Pa_GetErrorText(err));

    // Store the sample format.
    PaSampleFormat sf;

    // Set the sample format.
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
                               paFramesPerBufferUnspecified,            /* frames per buffer, i.e. the number
                                                      of sample frames that PortAudio will
                                                      request from the callback. Many apps
                                                      may want to use
                                                      paFramesPerBufferUnspecified, which
                                                      tells PortAudio to pick the best,
                                                      possibly changing, buffer size.*/
                               paPlayCallback, /* this is your callback function */
                               w);             /*This is a pointer that will be passed to
                                                             your callback*/
    printf("%p\n",stream);
    return stream;
}

/**
 * Play a file given a stream and a pointer to a wav file.
 */
void playFile(PaStream *stream, WavInfo *w)
{
    // Initialise Port Audio
    PaError err = Pa_Initialize();
    printf("%p\n", stream);
    // If Audio is still being played, do nothing.
    if (Pa_IsStreamActive(stream) == 1)
    {
        printf("Stream is active so returning!\n");
        return;
    }

    // Stop the stream.
    err = Pa_StopStream(stream);
    
    // If the audio is finished.
    if (w->currentPointer - w->bulkData == w->dataSize)
    {
        printf("%p\n", w);
        w->currentPointer = w->bulkData;
    }
    // Reset the pointer to the start of the wav file.

    err = Pa_StartStream(stream);
    if (err != paNoError)
        printf("\n\n\nPortAudio error in playback: %s\n", Pa_GetErrorText(err));
}

/**
 * Pause running audio.
 */
void pauseAudio(PaStream *stream, WavInfo *w)
{
    // If the stream is active.
    if (Pa_IsStreamActive(stream) == 1)
    {
        PaError err = Pa_Initialize();
        // Stop the stream.
        err = Pa_StopStream(stream);
        // Print out an error message if there was one.
        if (err != paNoError)
            printf("\n\n\nPortAudio error in pausing: %s\n", Pa_GetErrorText(err));
    }
}

/**
 * Stop running audio.
 */
void stopAudio(PaStream *stream, WavInfo *w)
{
    // If the stream is active.
    if (Pa_IsStreamActive(stream) == 1)
    {
        // Abort the stream.
        PaError err = Pa_AbortStream(stream);
        // Reset the pointer.
        w->currentPointer = w->bulkData;
        // Print any error that occured.
        if (err != paNoError)
            printf("\n\n\nPortAudio error in stopping: %s\n", Pa_GetErrorText(err));
    }
}

// Close port audio.
void closeAudioManager()
{
    // Close portAudio.
    PaError err = Pa_Terminate();

    // If there was an error, print info.
    if (err != paNoError)
        printf("\n\n\nPortAudio error closing driver: %s\n", Pa_GetErrorText(err));
}

/**
 * Start a recording.
 */
void startRecording(PaStream **stream, WavInfo *w)
{
    // Initialise the stream.
    PaError err = Pa_Initialize();

    // If there was an error print it.
    if (err != paNoError)
        printf("\n\n\nPortAudio error: %s\n", Pa_GetErrorText(err));

    // Store a sample format.
    PaSampleFormat sf;

    // Set the sample format.
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
    err = Pa_OpenDefaultStream(stream,
                               2,  /* no input channels */
                               0,  /* stereo output */
                               sf, /* 16 bit int output */
                               w->sampleRate,
                               256,              /* frames per buffer, i.e. the number
                                                        of sample frames that PortAudio will
                                                        request from the callback. Many apps
                                                        may want to use
                                                        paFramesPerBufferUnspecified, which
                                                        tells PortAudio to pick the best,
                                                        possibly changing, buffer size.*/
                               paRecordCallback, /* this is your callback function */
                               w);               /*This is a pointer that will be passed to
                                                               your callback*/

    err = Pa_StartStream(*stream);
    if (err != paNoError)
        printf("\n\n\nPortAudio error in recording: %s\n", Pa_GetErrorText(err));
}