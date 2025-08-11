#include "waveformManager.h"
#include <stdio.h>

int getHeight(int index, int width, int height, WavInfo *w)
{
    long totalSamples = w->dataSize / sizeof(short);
    long sampleIndex = totalSamples * index / width;

    short *samples = (short *)w->bulkData;
    short sample = samples[sampleIndex];

    int halfway = height / 2;
    int amplitude = (sample * halfway) / SHRT_MAX;

    return amplitude + halfway;
}

GdkPaintable *makeWaveform(int width, int height, WavInfo *w)
{
    guchar *data = malloc(3 * (width * height * sizeof(guchar) + 1));
    int halfway = height / 2;
    for (int h = 0; h < height; h++)
    {
        for (int j = 0; j < width; j++)
        {
            // Set the background for all the different pixels.
            data[3 * (width * h + j) + 0] = 255;
            data[3 * (width * h + j) + 1] = 255;
            data[3 * (width * h + j) + 2] = 255;
        }
    }
    for (int j = 0; j < width; j++)
    {
        int pixelHeight = getHeight(j, width, height, w);
        if (pixelHeight > halfway){
            for (int i = halfway; i < pixelHeight; i++){
                data[3 * (width * i + j) + 0] = 255;
                data[3 * (width * i + j) + 1] = 100;
                data[3 * (width * i + j) + 2] = 30;
            }
        }
        else{
            for (int i = pixelHeight; i < halfway; i++){
                data[3 * (width * i + j) + 0] = 255;
                data[3 * (width * i + j) + 1] = 100;
                data[3 * (width * i + j) + 2] = 30;
            }
        }
    }
    GdkPaintable* image = gdk_paintable_new_empty(width,height);

    return image;
}