#include "waveformManager.h"
#include <stdio.h>

int getHeight(int index, int width, int height, WavInfo *w)
{
    int totalSamples = w->dataSize / sizeof(short);
    int sampleIndex = totalSamples * index / width;

    short *samples = (short *)w->bulkData;
    short sample = samples[sampleIndex];

    int halfway = height / 2;
    int amplitude = (sample * halfway) / SHRT_MAX;

    printf("%i\n", amplitude);

    return amplitude + halfway;
}

GdkPixbuf *makeWaveform(int width, int height, WavInfo *w)
{
    guchar *data = malloc(3 * (width * height * sizeof(guchar) + 1));
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
        data[3 * (width * pixelHeight + j) + 0] = 255;
        data[3 * (width * pixelHeight + j) + 1] = 100;
        data[3 * (width * pixelHeight + j) + 2] = 30;
        data[3 * (width * (pixelHeight + 1) + j) + 0] = 255;
        data[3 * (width * (pixelHeight + 1) + j) + 1] = 100;
        data[3 * (width * (pixelHeight + 1) + j) + 2] = 30;
        data[3 * (width * (pixelHeight - 1) + j) + 0] = 255;
        data[3 * (width * (pixelHeight - 1) + j) + 1] = 100;
        data[3 * (width * (pixelHeight - 1) + j) + 2] = 30;
    }

    return gdk_pixbuf_new_from_data(
        data,
        GDK_COLORSPACE_RGB,
        0,
        8,
        width,
        height,
        width * 3,
        NULL,
        NULL);
}