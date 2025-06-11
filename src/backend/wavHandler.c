#include "wavHandler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int readWavFile(char *fileName, WavInfo *output) {
  // Open the file for reading.
  FILE *file = fopen(fileName, "rb");
  if (!file){
    return 1;
  }
  // Create buffers to read the data into.
  int size = 0;
  char *chunkIdentifier = malloc(4);
  char *fileType = malloc(4);
  int consumed = 4;
  printf("test2\n");
  fread(chunkIdentifier, 1, 4, file);
  // Check that the first 4 bytes equal buf.
  printf("test3\n");
  if (strncmp("RIFF", chunkIdentifier, 4) != 0) {
    printf("Invalid first 4 bytes: %s\n", chunkIdentifier);
    goto TIDY;
  }
  printf("test4\n");
  fread(&size, 4, 1, file);
  fread(fileType, 1, 4, file);
  printf("test1\n");
  if (strncmp("WAVE", fileType, 4) != 0) {
    fprintf(stderr, "Invalid File Type: %s\n", fileType);
    goto TIDY;
  }

  while (consumed < size) {
    // Read the chunk identifier.
    fread(chunkIdentifier, 4, 1, file);
    consumed += 4;
    int n = 0;
    // If the next block is a data block, read in the data.
    if (strncmp(chunkIdentifier, "data", 4) == 0) {
      n = readDataBlock(file, output);
      if (n ==0) fprintf(stderr, "Error reading data chunk!\n");

    // If the next block is irrelevant, skip it.
    } else if (strncmp(chunkIdentifier, "LIST", 4) == 0 || strncmp(chunkIdentifier, "JUNK", 4) == 0) {
      n = readOtherBlock(file, output);
      if (n ==0) fprintf(stderr, "Error reading list chunk!\n");
    
    // If the next chunk is a format chunk, read in the data.
    } else if (strncmp(chunkIdentifier, "fmt ", 4) == 0) {
      n = readHeaderBlock(file, output);
      if (n ==0) fprintf(stderr, "Error reading format chunk!\n");
    }
    // If the chunk is unrecognised, print an error message and quit.
    else{
     fprintf(stderr, "Unidentified Chunk: %s\n", chunkIdentifier);
     return 1;
    }
    consumed += n;
  }

TIDY:
  fclose(file);
  free(chunkIdentifier);
  free(fileType);
  return 0;
}

/*
  Read a format block of a wav file.
*/
int readHeaderBlock(FILE *file, WavInfo *output) {
  printf("Reading Header Block!\n");
  char *buf = malloc(6);
  int chunkSize = 0;
  int formatType = 0;
  int channelCount = 0;
  int sampleRate = 0;
  // Bits per sample.
  int bps = 0;

  // Read the values from the file.
  fread(&chunkSize, 4, 1, file);
  fread(&formatType, 2, 1, file);
  fread(&channelCount, 2, 1, file);
  fread(&sampleRate, 4, 1, file);
  fread(buf, 6, 1, file);
  fread(&bps, 2, 1, file);

  // Set the values in the output struct.
  output->channels = channelCount;
  output->sampleRate = sampleRate;
  output->sampleSize = bps;
  
  // If there are extra bytes, skip them before returning.
  if (chunkSize > 16){
    // Skip rest of bytes
    buf = realloc(buf, chunkSize - 16);
    fread(buf, chunkSize-16, 1, file);
  }

  // Free the buffer with spare data.
  free(buf);

  // Return the amount of bytes consumed.
  return chunkSize + 4;
}

int readDataBlock(FILE *file, WavInfo *output) {
  printf("Reading Data Block!\n");
  int chunkSize;
  fread(&chunkSize, 4, 1, file);
  void *bulkData = malloc(chunkSize);
  fread(bulkData, chunkSize, 1, file);
  output->bulkData = bulkData;
  output->currentPointer = bulkData;
  output->dataSize = chunkSize;

  // Return the size of the chunk with the 4 bytes for the size included but not
  // the identifier.
  return chunkSize + 4;
}

int readOtherBlock(FILE *file, WavInfo *output) {
  printf("Reading Unknown Block!\n");
  int chunkSize;
  fread(&chunkSize, 4, 1, file);
  void *bulkData = malloc(chunkSize);
  fread(bulkData, chunkSize, 1, file);
  // Return the size of the chunk with the 4 bytes for the size included but not
  // the identifier.
  free(bulkData);
  return chunkSize + 4;
}

void freeWavInfo(WavInfo * w){
    free(w->bulkData);
    free(w);
}