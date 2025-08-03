#include "track.h"

struct _TrackWidget
{
  GtkWidget parent_type;

  GtkWidget *left;
  GtkWidget *right;
};

G_DEFINE_TYPE (TrackWidget, track_widget, GTK_TYPE_WIDGET)

static void track_widget_class_init (TrackWidgetClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  gtk_widget_class_set_template_from_resource (widget_class,"src/frontend/ui/templates/track.ui");
  gtk_widget_class_bind_template_child (widget_class, TrackWidget, left);
  gtk_widget_class_bind_template_child (widget_class, TrackWidget, right);
}

static void track_widget_init (TrackWidget* self){
    gtk_widget_init_template (GTK_WIDGET (self));
}