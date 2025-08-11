#include "track.h"

struct _TrackWidget
{
  GtkWidget parent_type;

  GtkWidget *left;
  GtkWidget *right;
};

G_DEFINE_TYPE (TrackWidget, track_widget, GTK_TYPE_BOX)

static void track_widget_class_init (TrackWidgetClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  gtk_widget_class_set_template_from_resource (widget_class,"/org/gtk/MoreDaw/TrackWidget.ui");
  gtk_widget_class_bind_template_child (widget_class, TrackWidget, left);
  gtk_widget_class_bind_template_child (widget_class, TrackWidget, right);
}

static void track_widget_init (TrackWidget* self){
    gtk_widget_init_template (GTK_WIDGET (self));
    gtk_widget_add_css_class(self->left, "left_track");
    gtk_widget_add_css_class(self->right, "right_track");
    gtk_widget_set_size_request(self->left, 200,200);
}

GtkWidget* track_widget_get_left(TrackWidget *self) {
    return self->left;
}

GtkWidget* track_widget_get_right(TrackWidget *self) {
    return self->right;
}

int getRightWidth(TrackWidget* self){
  return gtk_widget_get_size(self->right,GTK_ORIENTATION_HORIZONTAL);
}
int getRightHeight(TrackWidget* self){
  return gtk_widget_get_size(self->right,GTK_ORIENTATION_VERTICAL);
}