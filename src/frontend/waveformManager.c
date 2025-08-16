#include "waveformManager.h"
#include <stdio.h>

/**
 * Get the height of the wave for a specific time.
 */
int getHeight(int index, int width, int height, WavInfo *w)
{
    // Get the total amount of samples in the wav file.
    long totalSamples = w->dataSize / w->sampleSize * 8;

    // Get the index in the samples.
    long sampleIndex = totalSamples * index / width / w->sampleSize * 8;

    // Get the height of halfway up the image.
    int halfway = height / 2;
    if (w->sampleSize == 16)
    {
        // Get a pointer to the start of the samples.
        short *samples = (short *)w->bulkData;

        // Get the sample at the required index.
        short sample = samples[sampleIndex];

        // Get the height above / below the index that it should be.
        int amplitude = ((sample << 4) * halfway) / G_MAXINT16;

        // Return the height the wave should be.
        return amplitude + halfway;
    }
    else if (w->sampleSize == 24)
    {
        // Get a pointer to the start of the samples.
        u_int8_t *samples = (u_int8_t *)(w->bulkData+ sampleIndex);

        // Get the sample at the required index.
        int sample = convert24bitToInt(samples);

        // Get the height above / below the index that it should be.
        int amplitude = height * (double)((double)(sample) / (double)G_MAXINT32);

        // Return the height the wave should be.
        return amplitude + halfway;
    }
    else if (w->sampleSize == 32)
    {
        // Get a pointer to the start of the samples.
        int *samples = (int *)w->bulkData;

        // Get the sample at the required index.
        int sample = samples[sampleIndex];

        // Get the height above / below the index that it should be.
        int amplitude = height * (double)((double)(sample << 4) / (double)G_MAXINT32);

        // Return the height the wave should be.
        return amplitude + halfway;
    }
    return 0;
}

/**
 * The draw function for the GtkDrawable.
 */
static void draw_function(GtkDrawingArea *area,
                          cairo_t *cr,
                          int width,
                          int height,
                          gpointer data)
{
    // The colour of the wave.
    GdkRGBA foreground;
    foreground.red = 1;
    foreground.green = 0.1;
    foreground.blue = 0.4;
    foreground.alpha = 0.5;

    // Store the current height.
    int currentHeight;

    // Set the colour.
    gdk_cairo_set_source_rgba(cr, &foreground);

    // Move to the start of the image.
    cairo_move_to(cr, 0, height / 2);

    // Loop through the image and draw all the lines.
    for (int i = 0; i < width; i++)
    {
        currentHeight = getHeight(i, width, height, (WavInfo *)data);
        cairo_line_to(cr, i, currentHeight);
    }

    // Render the lines.
    cairo_stroke(cr);
}

/**
 * Draw a waveform in a GtkDrawing area given the width, height and wave file.
 */
GtkDrawingArea *makeWaveform(int width, int height, WavInfo *w)
{
    // Get a new drawing area.
    GtkDrawingArea *area = (GtkDrawingArea *)gtk_drawing_area_new();

    // Set the dimensions.
    gtk_drawing_area_set_content_width(GTK_DRAWING_AREA(area), width);
    gtk_drawing_area_set_content_height(GTK_DRAWING_AREA(area), height);

    // Set the drawing function.
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(area),
                                   draw_function,
                                   w, NULL);

    // Return the drawing area.
    return area;
}