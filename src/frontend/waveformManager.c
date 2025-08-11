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

static void draw_function(GtkDrawingArea *area,
                          cairo_t *cr,
                          int width,
                          int height,
                          gpointer data)
{
    GdkRGBA foreground;
    foreground.red = 1;
    foreground.green = 0.1;
    foreground.blue = 0.4;
    foreground.alpha = 0.5;
    int currentHeight;
    //gtk_widget_get_color(GTK_WIDGET(area),&background);
    gdk_cairo_set_source_rgba(cr, &foreground);
    cairo_move_to(cr,0,height / 2);
    for (int i = 0; i < width; i++){
        currentHeight = getHeight(i,width,height,(WavInfo*) data);
        cairo_line_to(cr,i,currentHeight);
    }
    cairo_stroke(cr);
}

GtkDrawingArea *makeWaveform(int width, int height, WavInfo *w)
{
    GtkDrawingArea *area = (GtkDrawingArea *)gtk_drawing_area_new();
    gtk_drawing_area_set_content_width(GTK_DRAWING_AREA(area), width);
    gtk_drawing_area_set_content_height(GTK_DRAWING_AREA(area), height);
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(area),
                                   draw_function,
                                   w, NULL);
    return area;
}