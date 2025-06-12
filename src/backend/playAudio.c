#include "playAudio.h"
#include "wavHandler.h"

/* This routine will be called by the PortAudio engine when audio is needed.
 */
int patestCallback(const void *inputBuffer, void *outputBuffer,
                   unsigned long framesPerBuffer,
                   const PaStreamCallbackTimeInfo *timeInfo,
                   PaStreamCallbackFlags statusFlags, void *userData)
{
    /* Cast data passed through stream to our structure. */
    WavInfo *data = (WavInfo *)userData;
    short *out = (short *)outputBuffer;
    unsigned int i;
    (void)inputBuffer; // Prevent unused variable warning.

    // Loop for the whole of the buffer.
    for (i = 0; i < framesPerBuffer * data->channels * data->sampleSize / 16; i++)
    {
        // If overrun the end, return PA_COMPlETE.
        if ((void *)data->currentPointer - data->bulkData >= data->dataSize)
        {
            printf("Finished audio!\n");
            return paComplete;
        }

        // Write 2 bytes to the output stream.
        *out = *(short *)data->currentPointer;

        // Increment the pointers.
        data->currentPointer += 2;
        out++;
    }
    return 0;
}