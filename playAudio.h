#include "portaudio.h"
typedef struct
{
    float left_phase;
    float right_phase;
}   
paTestData;

short swapEndian(short);

int patestCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData );