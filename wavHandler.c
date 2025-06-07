#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "wavHandler.h"

int readHeader(char* file, WavInfo* output){
    // Open the file for reading.
    FILE* testFile = fopen("./test.wav","rb");

    // Create buffers to read the data into.
    int size = 0;
    char* buf = malloc(4);
    char* fileType = malloc(4);
    uint chunkSize = 0;
    uint formatType = 0;
    uint channelCount = 0;
    uint sampleRate = 0;
    // Bits per sample.
    uint bps = 0;

    fread(buf,1,4,testFile);
    // Check that the first 4 bytes equal buf.
    if (strncmp("RIFF",buf,4) != 0){
        printf("Invalid first 4 bytes: %s\n", buf);
        goto TIDY;
    }

    fread(&size,4,1,testFile);
    fread(fileType,1,4,testFile);
    fread(buf,1,4,testFile);
    // Check that the first 4 bytes equal the format marker.
    if (strncmp("fmt",buf,3) != 0){
        printf("Expected fmt but got: %s\n", buf);
        goto TIDY;
    }
    fread(&chunkSize,4,1,testFile);
    fread(&formatType,2,1,testFile);
    fread(&channelCount,2,1,testFile);
    fread(&sampleRate,4,1,testFile);

    buf = realloc(buf,6);

    fread(buf, 6,1,testFile);
    
    fread(&bps,2,1,testFile);

    buf = realloc(buf,4);

    fread(buf, 1,4,testFile);

    // Skip list block.
    if (strncmp(buf,"LIST",4) == 0){
        fread(&chunkSize, 4, 1, testFile);
        void* scratch = malloc(chunkSize);
        fread(scratch,chunkSize, 1, testFile);
        free(scratch);
        fread(buf, 1,4,testFile);
    }
    if (strncmp("data", buf,4) == 0) {
        fread(&chunkSize, 4, 1, testFile);
        void* bulkData = malloc(chunkSize);
        fread(bulkData, chunkSize, 1, testFile);
        output->channels = channelCount;
        output->sampleRate = sampleRate;
        output->sampleSize = bps;
        output->bulkData = bulkData;
        output->currentPointer = bulkData;
        output->dataSize = chunkSize;
    }

    
    

TIDY:
    fclose(testFile);
    free(buf);
    free(fileType);
    return 0;
}