#include "waveformManager.h"
int hzoom = 10;
double vzoom = 3;

int getHeight(int *sampleHeight, int index, int width, int imageHeight, WavInfo *w)
{
    int timeOfSample = (hzoom * index) - w->startTime;

    // Which sample number in the stream?
    long sampleIndex = (timeOfSample * w->sampleRate) / 1000;

    int bytesPerSample = w->sampleSize / 8;
    long byteOffset = sampleIndex * bytesPerSample * w->channels;

    if (byteOffset < 0 || byteOffset + bytesPerSample > w->dataSize) {
        return 1;
    }

    int halfway = imageHeight / 2;
    int sample = 0;

    if (w->sampleSize == 16) {
        short *samples = (short *)(w->bulkData + byteOffset);
        sample = samples[0];  // first channel
        int amplitude = (sample * halfway) / G_MAXINT16 * vzoom;
        if (amplitude > halfway) amplitude = halfway;
        *sampleHeight = amplitude + halfway;
        return 0;
    }
    else if (w->sampleSize == 24) {
        uint8_t *ptr = (uint8_t *)(w->bulkData + byteOffset);
        sample = convert24bitToInt(ptr);  // must return signed 32-bit
        int amplitude = ((double)sample / (double)G_MAXINT32) * (double)halfway * vzoom;
        if (amplitude > halfway) amplitude = halfway;
        *sampleHeight = amplitude + halfway;
        return 0;
    }
    else if (w->sampleSize == 32) {
        int *samples = (int *)(w->bulkData + byteOffset);
        sample = samples[0];
        int amplitude = ((double)sample / (double)G_MAXINT32) * (double)halfway * vzoom;
        if (amplitude > halfway) amplitude = halfway;
        *sampleHeight = amplitude + halfway;
        return 0;
    }

    return 1;
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
        if (getHeight(&currentHeight, i, width, height, (WavInfo *)data) == 1)
        {
            cairo_move_to(cr, i, height / 2);
        }
        else
        {
            cairo_line_to(cr, i, currentHeight);
        }
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