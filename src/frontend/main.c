#include "main.h"
#define recordBufferSize 1000000

// Globals
PaStream *stream = (PaStream *)0;
WavInfo *w = (WavInfo *)0;
State currentState = STOPPED;

// Handler for the play button.
static void playAudio(GtkWidget *widget, gpointer data)
{
    // If the audio is currently not playing, play audio and set the current state.
    if (currentState == STOPPED)
    {
        playFile(stream, w);
        currentState = PLAYING;
    }
}

/**
 * Handler for the pause button.
 */
static void pauseSound(GtkWidget *widget, gpointer data)
{
    // If the music is not stopped, pause it.
    if (currentState != STOPPED)
    {
        pauseAudio(stream, w);
        currentState = STOPPED;
    }
}

/**
 * Handler for the stop button.
 */
static void stopSound(GtkWidget *widget, gpointer data)
{
    // If the audio is running in some way, stop it.
    if (currentState != STOPPED)
    {
        stopAudio(stream, w);
        currentState = STOPPED;
    }
}

/**
 * Handler for the record button.
 */
static void recordSound(GtkWidget *widget, gpointer data)
{
    // If the audio is stopped.
    if (currentState == STOPPED)
    {
        // Reset the file that audio is recorded to.
        w->bulkData = calloc(recordBufferSize, 1);
        w->currentPointer = w->bulkData;
        w->dataSize = recordBufferSize;

        // Start the recording.
        startRecording(&stream, w);
        currentState = RECORDING;
    }
}

/**
 * Handler for the save button.
 */
static void saveSound(GtkWidget *widget, gpointer data)
{
    // If no audio is playing or recording then write the file to a new wav file.
    if (currentState == STOPPED)
    {
        writeWavFile("res/audio/OUT.wav", w);
    }
}

// Activate the window.
static void activate(GtkApplication *app, gpointer user_data)
{
    // Get the builder and window.
    GtkBuilder *build;
    GtkWidget *window;

    // Get the builder from the file.
    build = gtk_builder_new_from_file("src/frontend/ui/moredaw.ui");
    // Get the window object from the builder.
    window = GTK_WIDGET(gtk_builder_get_object(build, "window"));

    // Activate the header and body of the main page.
    activateHeader(app, user_data, build);
    activateBody(app, user_data, build);

    // Load the CSS for the main page.
    loadCSS(app, user_data);

    // Set the application to the window.
    gtk_window_set_application(GTK_WINDOW(window), GTK_APPLICATION(app));

    // Present the window. 
    gtk_window_present(GTK_WINDOW(window));

    // Unreference the builder.
    g_object_unref(build);
}

static void loadCSS(GtkApplication *app, gpointer user_data)
{
    // Get a new css provider then load css from the path.
    GtkCssProvider *css = gtk_css_provider_new();
    gtk_css_provider_load_from_path(css, "src/frontend/css/moredaw.css");

    // Add the styling to the app.
    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(css),
        GTK_STYLE_PROVIDER_PRIORITY_USER);
}

/**
 * Activate the header of the application.
 */
static void activateHeader(GtkApplication *app, gpointer user_data, GtkBuilder *build)
{
    // Initialise pointers to the elements.
    GtkWidget *playButton;
    GtkWidget *pauseButton;
    GtkWidget *stopButton;
    GtkWidget *recordButton;
    GtkWidget *saveButton;
    GtkWidget *header;

    // Get pointers to the elements.
    playButton = GTK_WIDGET(gtk_builder_get_object(build, "playButton"));
    pauseButton = GTK_WIDGET(gtk_builder_get_object(build, "pauseButton"));
    stopButton = GTK_WIDGET(gtk_builder_get_object(build, "stopButton"));
    recordButton = GTK_WIDGET(gtk_builder_get_object(build, "recordButton"));
    saveButton = GTK_WIDGET(gtk_builder_get_object(build, "saveButton"));
    header = GTK_WIDGET(gtk_builder_get_object(build, "header"));

    // Add the CSS classes.
    gtk_widget_add_css_class(playButton, "my-button");
    gtk_widget_add_css_class(pauseButton, "my-button");
    gtk_widget_add_css_class(stopButton, "my-button");
    gtk_widget_add_css_class(recordButton, "red-button");
    gtk_widget_add_css_class(saveButton, "my-button");
    gtk_widget_add_css_class(header, "header");

    // Connect the signals for the different buttons.
    g_signal_connect(playButton, "clicked", G_CALLBACK(playAudio), NULL);
    g_signal_connect(pauseButton, "clicked", G_CALLBACK(pauseSound), NULL);
    g_signal_connect(stopButton, "clicked", G_CALLBACK(stopSound), NULL);
    g_signal_connect(recordButton, "clicked", G_CALLBACK(recordSound), NULL);
    g_signal_connect(saveButton, "clicked", G_CALLBACK(saveSound), NULL);
}

/**
 * Activate the body of the window.
 */
static void activateBody(GtkApplication *app, gpointer user_data, GtkBuilder *build)
{
    // Get the two elements.
    GtkWidget *bodyBox;   

    bodyBox = GTK_WIDGET(gtk_builder_get_object(build, "body"));

    GtkWidget *track = g_object_new (track_widget_get_type(), NULL);
    GtkWidget *label = gtk_label_new("This Is");
    TrackWidget *track_widget = (TrackWidget *)track;

    // Add the waveform to the body.
    gtk_box_append((GtkBox*) bodyBox, track);

    int waveformHeight = 300;
    int waveformWidth = 600;
    GtkWidget *waveform = gtk_image_new_from_paintable(makeWaveform(waveformWidth,waveformHeight,w));
    gtk_widget_set_size_request(waveform, waveformWidth, waveformHeight);
    gtk_widget_set_hexpand(waveform, FALSE);
    gtk_widget_set_vexpand(waveform, FALSE);
    gtk_box_append((GtkBox*) track_widget_get_right(track_widget), waveform);
    gtk_box_append((GtkBox*) track_widget_get_left(track_widget), label);
}


/**
 * Main Method of program.
 */
int main(int argc, char **argv)
{
    // Allocate memory for the wave info.
    w = malloc(sizeof(WavInfo));

    // If there was a problem reading the file, print an error and exit.
    if (readWavFile("res/audio/test.wav", w) != 0)
    {
        printf("file not found!\n");
        exit(1);
    }

    // Initialise the stream.
    stream = initialise(w);

    // Get a pointer to the main app.
    GtkApplication *app;
    int status;

    // Initialise the app.
    app = gtk_application_new("joshuabeighton.moreDAW", G_APPLICATION_DEFAULT_FLAGS);

    // Connect the activation signal to the activate function above.
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    // Run the app.
    status = g_application_run(G_APPLICATION(app), argc, argv);

    // After execution, dereference the app.
    g_object_unref(app);

    // Return the status.
    return status;
}