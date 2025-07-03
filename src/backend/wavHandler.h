#ifndef WAVHANDLER
#define WAVHANDLER
#include <stdio.h>

typedef struct {
    int channels;
    int sampleRate;
    int sampleSize;
    void* bulkData;
    void* currentPointer;
    int dataSize;
} WavInfo;

void freeWavInfo(WavInfo*);

int readWavFile(char*, WavInfo*);
int readDataBlock(FILE*, WavInfo*);
int readHeaderBlock(FILE*, WavInfo*);
int readOtherBlock(FILE*, WavInfo*);
int getTotalDuration(WavInfo*);
int getElapsedDuration(WavInfo*);
int writeWavFile(char*, WavInfo*);

#endif