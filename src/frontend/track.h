#pragma once

#include <gtk/gtk.h>

G_BEGIN_DECLS

G_DECLARE_FINAL_TYPE(TrackWidget, track_widget, TRACK, WIDGET, GtkBox)

G_END_DECLS

GtkWidget* track_widget_get_left(TrackWidget *self);
GtkWidget* track_widget_get_right(TrackWidget *self);
GtkWidget* track_widget_get_header(TrackWidget *self);
GtkWidget* track_widget_get_delete_button(TrackWidget *self);
int getRightHeight(TrackWidget* self);
int getRightWidth(TrackWidget* self);