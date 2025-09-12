#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct {
    int channels;
    int sampleRate;
    int sampleSize;
    void* bulkData;
    void* currentPointer;
    void* renderPointer;
    int dataSize;
    char* name;
    float startTime;
} WavInfo;

typedef struct {
    int trackCount;
    WavInfo** tracks;
} TrackList;

void freeWavInfo(WavInfo*);
void addTrack_File(TrackList*, char*);
void addTrack_WavInfo(TrackList* tl, WavInfo* w);
WavInfo* createRenderTarget(TrackList*);
int readWavFile(char*, WavInfo*);
int readDataBlock(FILE*, WavInfo*);
int readHeaderBlock(FILE*, WavInfo*);
int readOtherBlock(FILE*, WavInfo*);
int getTotalDuration(WavInfo*);
int getElapsedDuration(WavInfo*);
int writeWavFile(char*, WavInfo*);
int convert24bitToInt(__uint8_t *bytes);
int fillBuffer(TrackList* tl, WavInfo* w, int bufferSize);
void removeTrackByIndex(TrackList* tl, int index);
int getSampleOffset(WavInfo* w);
int getIntRepresentation(WavInfo *w);