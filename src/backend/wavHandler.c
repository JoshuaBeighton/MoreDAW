#include "wavHandler.h"

int readWavFile(char *fileName, WavInfo *output)
{
    // Open the file for reading.
    FILE *file = fopen(fileName, "rb");
    if (!file)
    {
        return 1;
    }
    // Create buffers to read the data into.
    int size = 0;
    char *chunkIdentifier = malloc(4);
    char *fileType = malloc(4);
    int consumed = 4;
    fread(chunkIdentifier, 1, 4, file);
    // Check that the first 4 bytes equal buf.
    if (strncmp("RIFF", chunkIdentifier, 4) != 0)
    {
        fprintf(stderr, "Invalid first 4 bytes: %s\n", chunkIdentifier);
        goto TIDY;
    }
    fread(&size, 4, 1, file);
    fread(fileType, 1, 4, file);
    if (strncmp("WAVE", fileType, 4) != 0)
    {
        fprintf(stderr, "Invalid File Type: %s\n", fileType);
        goto TIDY;
    }

    while (consumed < size)
    {
        // Read the chunk identifier.
        fread(chunkIdentifier, 4, 1, file);
        consumed += 4;
        int n = 0;
        // If the next block is a data block, read in the data.
        if (strncmp(chunkIdentifier, "data", 4) == 0)
        {
            n = readDataBlock(file, output);
            if (n == 0)
                fprintf(stderr, "Error reading data chunk!\n");

            // If the next block is irrelevant, skip it.
        }
        else if (strncmp(chunkIdentifier, "LIST", 4) == 0 || strncmp(chunkIdentifier, "JUNK", 4) == 0)
        {
            n = readOtherBlock(file, output);
            if (n == 0)
                fprintf(stderr, "Error reading list chunk!\n");

            // If the next chunk is a format chunk, read in the data.
        }
        else if (strncmp(chunkIdentifier, "fmt ", 4) == 0)
        {
            n = readHeaderBlock(file, output);
            if (n == 0)
                fprintf(stderr, "Error reading format chunk!\n");
        }
        // If the chunk is unrecognised, print an error message and quit.
        else
        {
            fprintf(stderr, "Unidentified Chunk: %s\n", chunkIdentifier);
            return 1;
        }
        consumed += n;
    }

TIDY:
    fclose(file);
    free(chunkIdentifier);
    free(fileType);
    output->name = fileName;
    output->startTime = 0;
    return 0;
}

/*
  Read a format block of a wav file.
*/
int readHeaderBlock(FILE *file, WavInfo *output)
{
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
    if (chunkSize > 16)
    {
        // Skip rest of bytes
        buf = realloc(buf, chunkSize - 16);
        fread(buf, chunkSize - 16, 1, file);
    }

    // Free the buffer with spare data.
    free(buf);

    // Return the amount of bytes consumed.
    return chunkSize + 4;
}

int readDataBlock(FILE *file, WavInfo *output)
{
    int chunkSize;
    fread(&chunkSize, 4, 1, file);
    void *bulkData = malloc(chunkSize);
    fread(bulkData, chunkSize, 1, file);
    output->bulkData = bulkData;
    output->currentPointer = bulkData;
    output->renderPointer = bulkData;
    output->dataSize = chunkSize;

    // Return the size of the chunk with the 4 bytes for the size included but not
    // the identifier.
    return chunkSize + 4;
}

int readOtherBlock(FILE *file, WavInfo *output)
{
    int chunkSize;
    fread(&chunkSize, 4, 1, file);
    void *bulkData = malloc(chunkSize);
    fread(bulkData, chunkSize, 1, file);
    // Return the size of the chunk with the 4 bytes for the size included but not
    // the identifier.
    free(bulkData);
    return chunkSize + 4;
}

void freeWavInfo(WavInfo *w)
{
    free(w->bulkData);
    free(w);
}

int getTotalDuration(WavInfo *w)
{
    int divisor = w->sampleRate * w->sampleSize / 8 * w->channels;
    return w->dataSize / divisor;
}

int getElapsedDuration(WavInfo *w)
{
    int divisor = w->sampleRate * w->sampleSize / 8 * w->channels;
    return (int)(w->currentPointer - w->bulkData) / divisor;
}

int writeWavFile(char *filePath, WavInfo *w)
{
    FILE *file = fopen(filePath, "wb");
    if (!file)
    {
        return 1;
    }
    // Print file info.
    fprintf(file, "RIFF");
    int size = w->dataSize + 20;
    fwrite(&size, 4, 1, file);
    fprintf(file, "WAVE");

    // Print format chunk.
    fprintf(file, "fmt ");

    int formatSize = 16;
    short fileType = 1;
    short channels = w->channels;
    int sampleRate = w->sampleRate;
    int sampleSize = w->sampleSize;
    int bytesPerBlock = channels * sampleSize / 8;
    int bytesPerSecond = bytesPerBlock * sampleRate;

    fwrite(&formatSize, 4, 1, file);
    fwrite(&fileType, 2, 1, file);
    fwrite(&channels, 2, 1, file);
    fwrite(&sampleRate, 4, 1, file);
    fwrite(&bytesPerSecond, 4, 1, file);
    fwrite(&bytesPerBlock, 2, 1, file);
    fwrite(&sampleSize, 2, 1, file);

    // Print Data chunk
    fprintf(file, "data");
    fwrite(&(w->dataSize), 4, 1, file);
    fwrite(w->bulkData, w->dataSize, 1, file);
    fclose(file);
    return 0;
}

void addTrack_File(TrackList *tl, char *fileName)
{
    WavInfo *track = malloc(sizeof(WavInfo));
    // If there was a problem reading the file, print an error and exit.
    if (readWavFile(fileName, track) != 0)
    {
        fprintf(stderr, "file not found!\n");
        exit(1);
    }
    track->name = fileName;
    addTrack_WavInfo(tl, track);
}

void removeTrackByIndex(TrackList *tl, int index)
{
    // Ensure index is valid.
    if (index > tl->trackCount || index < 0)
    {
        fprintf(stderr, "Attempted to delete invalid track, index: %i\n", index);
        return;
    }
    printf("Deleting track %i!\n", index);
    freeWavInfo(tl->tracks[index]);
    for (int i = index; i < tl->trackCount - 1; i++)
    {
        tl->tracks[i] = tl->tracks[i + 1];
    }
    tl->trackCount -= 1;
}

void addTrack_WavInfo(TrackList *tl, WavInfo *w)
{
    tl->trackCount++;
    WavInfo **newbuffer = realloc(tl->tracks, sizeof(WavInfo *) * tl->trackCount);
    if (newbuffer == NULL)
    {
        fprintf(stderr, "Failed to add new track.\n");
        return;
    }
    tl->tracks = newbuffer;
    tl->tracks[tl->trackCount - 1] = w;
}

WavInfo *createRenderTarget(TrackList *tl)
{
    WavInfo *result = malloc(sizeof(WavInfo));
    int largestFileSize = 0;
    int sampleSizeInLargestFile = 0;
    for (int i = 0; i < tl->trackCount; i++)
    {
        tl->tracks[i]->renderPointer = tl->tracks[i]->bulkData - getSampleOffset(tl->tracks[i]);
        if (tl->tracks[i]->dataSize + getSampleOffset(tl->tracks[i]) > largestFileSize)
        {
            largestFileSize = tl->tracks[i]->dataSize + getSampleOffset(tl->tracks[i]);
            sampleSizeInLargestFile = tl->tracks[i]->sampleSize;
        }
    }
    result->sampleSize = 32;
    result->dataSize = largestFileSize / sampleSizeInLargestFile * 32;
    result->bulkData = malloc(result->dataSize);
    result->currentPointer = result->bulkData;
    result->renderPointer = result->bulkData;
    result->sampleRate = tl->tracks[0]->sampleRate;
    result->channels = 2;
    printf("Result data size: %i\n", result->dataSize);

    return result;
}

/**
 * Fill the buffer, rendering audio ahead of the playhead.
 * If reached end of track this will return 1, otherwise it will return 0.
 */
int fillBuffer(TrackList *tl, WavInfo *w, int bufferSize)
{
    int sum = 0;
    int returnVal = 0;

    // Calculate how many ints we can write into the buffer
    int toFill = (bufferSize - (w->renderPointer - w->currentPointer)) / sizeof(int);

    // Clamp toFill to minimum 0
    if (toFill < 0)
    {
        toFill = 0;
    }

    // Remaining space in buffer (in ints)
    int remainingInts = ((w->bulkData + w->dataSize) - w->renderPointer) / sizeof(int);

    // Avoid writing past the end of allocated memory
    if (toFill > remainingInts)
    {
        toFill = remainingInts;
        printf("\n\nReached end so only filling %i ints (%lu bytes).\n\n", toFill, toFill * sizeof(int));
        returnVal = 1;
    }

    // Fill the buffer
    for (int i = 0; i < toFill; i++)
    {
        for (int j = 0; j < tl->trackCount; j++)
        {
            sum += getIntRepresentation(tl->tracks[j]);
        }
        *((int *)w->renderPointer) = sum;
        w->renderPointer += sizeof(int);
        sum = 0;
    }

    return returnVal;
}

int getIntRepresentation(WavInfo *w)
{
    int total = 0;

    if (w->renderPointer >= w->bulkData + w->dataSize || w->renderPointer < w->bulkData)
        return 0;

    switch (w->sampleSize)
    {
    case 16:
        total = ((int)(*((int16_t *)w->renderPointer))) << 16;
        break;

    case 24:
        total = convert24bitToInt(w->renderPointer);
        break;

    case 32:
        total = *((int32_t *)w->renderPointer);
        break;

    default:
        fprintf(stderr, "Invalid Sample Size\n");
        break;
    }

    w->renderPointer += (w->sampleSize / 8);
    return total;
}

int getSampleOffset(WavInfo *w)
{
    return w->startTime * w->sampleRate * w->sampleSize / 8;
}

int32_t convert24bitToInt(uint8_t *bytes)
{
    // WAV: little-endian order: [low][mid][high]
    int32_t value = (bytes[0]) | (bytes[1] << 8) | (bytes[2] << 16);

    // Sign extend manually (if bit 23 is set, fill the high 8 bits with 1s)
    if (value & 0x00800000)
        value |= 0xFF000000;

    return value << 8;
}
