#include "recordAudio.h"
#include "wavHandler.h"


/* This routine will be called by the PortAudio engine when audio is available.
 */
int paRecordCallback(const void *inputBuffer, void *outputBuffer,
                   unsigned long framesPerBuffer,
                   const PaStreamCallbackTimeInfo *timeInfo,
                   PaStreamCallbackFlags statusFlags, void *userData)
{
    /* Cast data passed through stream to our structure. */
    WavInfo *data = (WavInfo *)userData;
    short *out = (short *)inputBuffer;
    unsigned int i;
    (void)outputBuffer; // Prevent unused variable warning.

    // Loop for the whole of the buffer.
    for (i = 0; i < framesPerBuffer * data->channels * data->sampleSize / 16; i++)
    {
        // Write 2 bytes to the output.
        //*out = *(short *)data->currentPointer;
        short* currentLocation = (short*)data->currentPointer;
        *currentLocation = *out;
        // Increment the pointers.
        data->currentPointer += 2;
        out++;
        if (data->currentPointer - data->bulkData + 2 > data->dataSize){
            printf("Buffer full, returning!\n");
            data->currentPointer = data->bulkData;
            return 1;
        }
    }
    return 0;
}