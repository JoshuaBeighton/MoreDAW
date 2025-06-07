typedef struct {
    int channels;
    int sampleRate;
    int sampleSize;
    void* bulkData;
    short* currentPointer;
    int dataSize;
} WavInfo;

int readHeader(char*, WavInfo*);

