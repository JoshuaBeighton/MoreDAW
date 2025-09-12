#pragma once

#include <gtk/gtk.h>
#include "../backend/wavHandler.h"
#include <stdio.h>

GtkDrawingArea *makeWaveform(int width, int height, WavInfo *w);