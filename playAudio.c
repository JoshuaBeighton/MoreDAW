#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "playAudio.h"

int main(int argc, void** argv){
    readHeader("test.wav");
    return 0;
}


int readHeader(char* file){
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
    uint bps = 0;

    fread(buf,1,4,testFile);
    // Check that the first 4 bytes equal buf.
    if (strcmp("RIFF",buf) != 0){
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
    if (strncmp(buf,"LIST",4) == 0){
        fread(&chunkSize, 4, 1, testFile);
        void* scratch = malloc(chunkSize);
        fread(scratch,chunkSize, 1, testFile);
        free(scratch);
        fread(buf, 1,4,testFile);
    }
    if (strncmp("data", buf,4) == 0) {
        printf("reading data!\n");
    }
    

    printf("Size: %i Bytes\n", size);
    printf("Channel Count: %i\n", channelCount);
    printf("Sample Rate: %i\n", sampleRate);
    printf("Sample Size: %i\n", bps);

    printf("%p\n", testFile);

TIDY:
    fclose(testFile);
    free(buf);
    free(fileType);
}