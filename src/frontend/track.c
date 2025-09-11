#include "track.h"

// Represent the elements in a track.
struct _TrackWidget
{
    GtkWidget parent_type;

    GtkWidget *left;
    GtkWidget *right;
    GtkWidget *header;
    GtkWidget *delete_button;
};

G_DEFINE_TYPE(TrackWidget, track_widget, GTK_TYPE_BOX)

// Initialise the widget class.
static void track_widget_class_init(TrackWidgetClass *klass)
{
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);

    // Set the template from the resource.
    gtk_widget_class_set_template_from_resource(widget_class, "/org/gtk/MoreDaw/TrackWidget.ui");

    // Bind the children of the widget.
    gtk_widget_class_bind_template_child(widget_class, TrackWidget, left);
    gtk_widget_class_bind_template_child(widget_class, TrackWidget, right);
    gtk_widget_class_bind_template_child(widget_class, TrackWidget, header);
    gtk_widget_class_bind_template_child(widget_class, TrackWidget, delete_button);
}

/**
 * Initialise a specific track widget.
 */
static void track_widget_init(TrackWidget *self)
{
    // Initialise the template.
    gtk_widget_init_template(GTK_WIDGET(self));
    // Add necessary CSS classes.
    gtk_widget_add_css_class(self->left, "left_track");
    gtk_widget_add_css_class(self->right, "right_track");
    // Set a size request for the left box
    gtk_widget_set_size_request(self->left, 200, 0);
}

/**
 * Get a pointer to the left child of the track.
 */
GtkWidget *track_widget_get_left(TrackWidget *self)
{
    return self->left;
}

/**
 * Get a pointer to the right child of the track.
 */
GtkWidget *track_widget_get_right(TrackWidget *self)
{
    return self->right;
}

/**
 * Get a pointer to the right child of the track.
 */
GtkWidget *track_widget_get_header(TrackWidget *self)
{
    return self->header;
}

/**
 * Get a pointer to the right child of the track.
 */
GtkWidget *track_widget_get_delete_button(TrackWidget *self)
{
    return self->delete_button;
}