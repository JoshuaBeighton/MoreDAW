#include "portaudio.h"
typedef struct
{
    float left_phase;
    float right_phase;
}   
paTestData;

typedef struct{
    unsigned int data : 24;
} int24;

int patestCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData );