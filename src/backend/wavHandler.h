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
    char* name;
} WavInfo;

typedef struct {
    int trackCount;
    WavInfo** tracks;
} TrackList;

void freeWavInfo(WavInfo*);

void addTrack_File(TrackList*, char*);
void addTrack_WavInfo(TrackList* tl, WavInfo* w);
WavInfo* render(TrackList*);
int readWavFile(char*, WavInfo*);
int readDataBlock(FILE*, WavInfo*);
int readHeaderBlock(FILE*, WavInfo*);
int readOtherBlock(FILE*, WavInfo*);
int getTotalDuration(WavInfo*);
int getElapsedDuration(WavInfo*);
int writeWavFile(char*, WavInfo*);
int convert24bitToInt(__uint8_t *bytes);

#endif