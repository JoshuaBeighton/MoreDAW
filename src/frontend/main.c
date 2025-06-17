#include "main.h"
#define recordBufferSize 1000000

PaStream *stream = (PaStream *)0;
WavInfo *w = (WavInfo *)0;
State currentState = STOPPED;

static void playAudio(GtkWidget *widget, gpointer data)
{
    if (currentState == STOPPED)
    {
        playFile(stream, w);
        currentState = PLAYING;
    }
}

static void pauseSound(GtkWidget *widget, gpointer data)
{
    if (currentState != STOPPED)
    {
        pauseAudio(stream, w);
        currentState = STOPPED;
    }
}

static void stopSound(GtkWidget *widget, gpointer data)
{
    if (currentState != STOPPED)
    {
        stopAudio(stream, w);
        currentState = STOPPED;
    }
}

static void recordSound(GtkWidget *widget, gpointer data)
{
    if (currentState == STOPPED)
    {
        w->bulkData = calloc(recordBufferSize, 1);
        w->currentPointer = w->bulkData;
        w->dataSize = recordBufferSize;
        startRecording(&stream, w);
        currentState = RECORDING;
    }
}

static void saveSound(GtkWidget *widget, gpointer data)
{
    if (currentState == STOPPED)
    {
        writeWavFile("res/audio/OUT.wav", w);
    }
}

static void activate(GtkApplication *app, gpointer user_data)
{
    GtkBuilder *build;
    GtkWidget *window;

    build = gtk_builder_new_from_file("src/frontend/moredaw.ui");
    window = GTK_WIDGET(gtk_builder_get_object(build, "window"));

    activateHeader(app, user_data, build);
    activateBody(app, user_data, build);
    loadCSS(app, user_data);
    gtk_window_set_application(GTK_WINDOW(window), GTK_APPLICATION(app));

    gtk_window_present(GTK_WINDOW(window));

    g_object_unref(build);
}

static void loadCSS(GtkApplication *app, gpointer user_data)
{
    GtkCssProvider *css = gtk_css_provider_new();
    gtk_css_provider_load_from_path(css, "src/frontend/moredaw.css");
    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(css),
        GTK_STYLE_PROVIDER_PRIORITY_USER);
}

static void activateHeader(GtkApplication *app, gpointer user_data, GtkBuilder *build)
{
    GtkWidget *playButton;
    GtkWidget *pauseButton;
    GtkWidget *stopButton;
    GtkWidget *recordButton;
    GtkWidget *saveButton;
    GtkWidget *header;
    playButton = GTK_WIDGET(gtk_builder_get_object(build, "playButton"));
    pauseButton = GTK_WIDGET(gtk_builder_get_object(build, "pauseButton"));
    stopButton = GTK_WIDGET(gtk_builder_get_object(build, "stopButton"));
    recordButton = GTK_WIDGET(gtk_builder_get_object(build, "recordButton"));
    saveButton = GTK_WIDGET(gtk_builder_get_object(build, "saveButton"));
    header = GTK_WIDGET(gtk_builder_get_object(build, "header"));

    gtk_widget_add_css_class(playButton, "my-button");
    gtk_widget_add_css_class(pauseButton, "my-button");
    gtk_widget_add_css_class(stopButton, "my-button");
    gtk_widget_add_css_class(recordButton, "red-button");
    gtk_widget_add_css_class(saveButton, "my-button");
    gtk_widget_add_css_class(header, "header");

    g_signal_connect(playButton, "clicked", G_CALLBACK(playAudio), NULL);
    g_signal_connect(pauseButton, "clicked", G_CALLBACK(pauseSound), NULL);
    g_signal_connect(stopButton, "clicked", G_CALLBACK(stopSound), NULL);
    g_signal_connect(recordButton, "clicked", G_CALLBACK(recordSound), NULL);
    g_signal_connect(saveButton, "clicked", G_CALLBACK(saveSound), NULL);
}

static void activateBody(GtkApplication *app, gpointer user_data, GtkBuilder *build)
{
    GtkWidget *elapsedLabel;
    GtkWidget *totalLabel;

    elapsedLabel = GTK_WIDGET(gtk_builder_get_object(build, "elapsedTime"));
    totalLabel = GTK_WIDGET(gtk_builder_get_object(build, "totalTime"));

    char *totalTimeString = malloc(5);
    char *elapsedTimeString = malloc(5);

    sprintf(elapsedTimeString, "%d:%d/", getElapsedDuration(w) / 60, getElapsedDuration(w) % 60);
    sprintf(totalTimeString, "%d:%d", getTotalDuration(w) / 60, getTotalDuration(w) % 60);

    gtk_label_set_label(GTK_LABEL(elapsedLabel), elapsedTimeString);
    gtk_label_set_label(GTK_LABEL(totalLabel), totalTimeString);
}

int main(int argc, char **argv)
{
    w = malloc(sizeof(WavInfo));
    if (readWavFile("res/audio/ThisIsInst.wav", w) != 0)
    {
        printf("file not found!\n");
        exit(1);
    }
    stream = initialise(w);

    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}