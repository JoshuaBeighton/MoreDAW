#include "main.h"
#define recordBufferSize 1000000
#define PLAY_BUFFER_SIZE 44100

// Globals
PaStream *stream = (PaStream *)0;
WavInfo *rendered = (WavInfo *)0;
TrackList *masterList = (TrackList *)0;
State currentState = STOPPED;
GtkBuilder *build;

/**
 * Main Method of program.
 */
int main(int argc, char **argv)
{
    signal(2, (__sighandler_t)tidy);
    masterList = malloc(sizeof(TrackList));
    masterList->trackCount = 0;

    // Allocate memory for the wave info.
    rendered = malloc(sizeof(WavInfo));

    // Add two tracks to the list.
    addTrack_File(masterList, "res/audio/clap32.wav");

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

// Activate the window.
static void activate(GtkApplication *app, gpointer user_data)
{
    // Get the builder and window.
    GtkWidget *window;

    // Get the builder from the file.
    build = gtk_builder_new_from_file("src/frontend/ui/moredaw.ui");
    // Get the window object from the builder.
    window = GTK_WIDGET(gtk_builder_get_object(build, "window"));

    // Activate the toolbar and body of the main page.
    activateToolbar();
    activateBody(masterList);
    activateHeader(app);

    GtkBuilder *toolbarBuilder = gtk_builder_new_from_file("src/frontend/ui/templates/Menu.ui");
    GMenuModel *menu_model = G_MENU_MODEL(gtk_builder_get_object(toolbarBuilder, "menubar"));

    GtkWidget *menu_bar = gtk_popover_menu_bar_new_from_model(menu_model);
    GtkWidget *top_level_box = GTK_WIDGET(gtk_builder_get_object(build, "topBox"));
    gtk_box_prepend(GTK_BOX(top_level_box), menu_bar);

    // Load the CSS for the main page.
    loadCSS(app, user_data);

    // Set the application to the window.
    gtk_window_set_application(GTK_WINDOW(window), GTK_APPLICATION(app));

    // Present the window.
    gtk_window_present(GTK_WINDOW(window));

    // Unreference the builder.
    // g_object_unref(build);
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

static void activateHeader(GtkApplication *app)
{
    const GActionEntry app_actions[] = {
        {"open", openFile, NULL, NULL, NULL},
        {"quit", quit, NULL, NULL, NULL}};

    g_action_map_add_action_entries(G_ACTION_MAP(app),
                                    app_actions,
                                    G_N_ELEMENTS(app_actions),
                                    build);
}

/**
 * Activate the toolbar at the top of the application.
 */
static void activateToolbar()
{
    // Initialise pointers to the elements.
    GtkWidget *playButton;
    GtkWidget *pauseButton;
    GtkWidget *stopButton;
    GtkWidget *recordButton;
    GtkWidget *saveButton;
    GtkWidget *rewindButton;
    GtkWidget *toolbar;

    // Get pointers to the elements.
    playButton = GTK_WIDGET(gtk_builder_get_object(build, "playButton"));
    pauseButton = GTK_WIDGET(gtk_builder_get_object(build, "pauseButton"));
    stopButton = GTK_WIDGET(gtk_builder_get_object(build, "stopButton"));
    recordButton = GTK_WIDGET(gtk_builder_get_object(build, "recordButton"));
    saveButton = GTK_WIDGET(gtk_builder_get_object(build, "saveButton"));
    rewindButton = GTK_WIDGET(gtk_builder_get_object(build, "rewindButton"));
    toolbar = GTK_WIDGET(gtk_builder_get_object(build, "header"));

    // Add the CSS classes.
    gtk_widget_add_css_class(playButton, "my-button");
    gtk_widget_add_css_class(pauseButton, "my-button");
    gtk_widget_add_css_class(stopButton, "my-button");
    gtk_widget_add_css_class(recordButton, "red-button");
    gtk_widget_add_css_class(saveButton, "my-button");
    gtk_widget_add_css_class(rewindButton, "my-button");
    gtk_widget_add_css_class(toolbar, "header");

    // Connect the signals for the different buttons.
    g_signal_connect(playButton, "clicked", G_CALLBACK(playAudio), NULL);
    g_signal_connect(pauseButton, "clicked", G_CALLBACK(pauseSound), NULL);
    g_signal_connect(stopButton, "clicked", G_CALLBACK(stopSound), NULL);
    g_signal_connect(recordButton, "clicked", G_CALLBACK(recordSound), NULL);
    g_signal_connect(saveButton, "clicked", G_CALLBACK(saveSound), NULL);
    g_signal_connect(rewindButton, "clicked", G_CALLBACK(rewindSound), NULL);
}

char *formatTrackName(char *input)
{
    int len = strlen(input);
    if (len < 15)
    {
        return input;
    }
    char *output = malloc(15);
    output[0] = '.';
    output[1] = '.';
    output[2] = '.';
    strncpy(output + 3, input + len - 12, 12);
    output[15] = '\0';
    return output;
}

/**
 * Activate the body of the window.
 */
static void activateBody(TrackList *toShow)
{
    // Get the two elements.
    GtkWidget *bodyBox;

    bodyBox = GTK_WIDGET(gtk_builder_get_object(build, "body"));
    int waveformHeight = 100;
    int waveformWidth = 1000;
    printf("%p\n", toShow);
    printf("%i\n", toShow->trackCount);
    for (int i = 0; i < toShow->trackCount; i++)
    {

        GtkWidget *track = g_object_new(track_widget_get_type(), NULL);
        GtkWidget *label = gtk_label_new(formatTrackName(toShow->tracks[i]->name));
        TrackWidget *track_widget = (TrackWidget *)track;

        gtk_box_append((GtkBox *)bodyBox, track);
        gtk_box_append((GtkBox *)track_widget_get_header(track_widget), label);
        printf("i = %i. \n", i);
        g_signal_connect(track_widget_get_delete_button(track_widget), "clicked", G_CALLBACK(deleteTrack), (gpointer)toShow->tracks[i]);
        // printf("Track Count: %i\n", toShow->trackCount);
        GtkWidget *waveform = (GtkWidget *)makeWaveform(waveformWidth, waveformHeight, toShow->tracks[i]);
        gtk_box_append((GtkBox *)track_widget_get_right(track_widget), waveform);
    }
}

static void *render(void *_args)
{
    // Fill the buffer to start with.
    fillBuffer(masterList, rendered, PLAY_BUFFER_SIZE);
    // Wait for the stream to start
    while (Pa_IsStreamStopped(stream) == 1)
    {
        g_usleep(10);
    }

    // Loop until end of stream.
    while (Pa_IsStreamActive(stream) == 1)
    {
        fillBuffer(masterList, rendered, PLAY_BUFFER_SIZE);
        g_usleep(10);
    }
    printf("DONE\n");
    return NULL;
}

// Handler for the play button.
static void playAudio(GtkWidget *widget, gpointer data)
{
    rendered = createRenderTarget(masterList);

    // Initialise the stream.
    stream = initialise(rendered);

    Pa_SetStreamFinishedCallback(stream,onStreamFinished);
    // If the audio is currently not playing, play audio and set the current state.
    if (currentState == STOPPED)
    {
        pthread_t renderThread;
        pthread_create(&renderThread, NULL, render, NULL);
        playFile(stream, rendered);
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
        pauseAudio(stream, rendered);
        currentState = STOPPED;
    }
}

/**
 * Handler for the pause button.
 */
static void rewindSound(GtkWidget *widget, gpointer data)
{
    rendered->currentPointer = rendered->bulkData;
    rendered->renderPointer = rendered->bulkData;
}

/**
 * Handler for the stop button.
 */
static void stopSound(GtkWidget *widget, gpointer data)
{
    // If the audio is running in some way, stop it.
    if (currentState != STOPPED)
    {
        stopAudio(stream, rendered);
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
        rendered->bulkData = calloc(recordBufferSize, 1);
        rendered->currentPointer = rendered->bulkData;
        rendered->dataSize = recordBufferSize;

        // Start the recording.
        startRecording(&stream, rendered);
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
        writeWavFile("res/audio/OUT.wav", rendered);
    }
}

/**
 * Handler for the save button.
 */
static void deleteTrack(GtkWidget *widget, gpointer data)
{
    WavInfo* track = (WavInfo*) data;
    int index = 0;
    for (int i = 0; i < masterList->trackCount; i++){
        if (track == masterList->tracks[i]){
            index = i;
        }
    }
    removeTrackByIndex(masterList, index);
    // remove waveform from screen: TODO
}

static void openFile(GSimpleAction *action,
                     GVariant *parameter,
                     gpointer user_data)
{
    GtkWindow *parent = (GtkWindow *)gtk_window_new();

    GtkFileDialog *dialog = gtk_file_dialog_new();

    gtk_file_dialog_open(dialog,
                         parent,
                         NULL, // optional GCancellable
                         (GAsyncReadyCallback)on_file_chosen,
                         user_data); // user_data for callback
}

// callback after file is chosen
static void on_file_chosen(GObject *source,
                           GAsyncResult *result,
                           gpointer user_data)
{
    GtkFileDialog *dialog = GTK_FILE_DIALOG(source);
    GError *err;
    GFile *file = gtk_file_dialog_open_finish(dialog, result, &err);
    TrackList *newTracks = malloc(sizeof(TrackList));
    newTracks->trackCount = 0;
    if (file)
    {
        char *filename = g_file_get_path(file);
        addTrack_File(masterList, filename);
        addTrack_File(newTracks, filename);
        activateBody(newTracks);
        g_object_unref(file);
        free(newTracks);
    }
    else
    {
        fprintf(stderr, "Error: %s", err->message);
    }
}

static void onStreamFinished(void* _userData){
    currentState = STOPPED;
}

static void quit(GSimpleAction *action,
                 GVariant *parameter,
                 gpointer user_data)
{
    tidy();
}

void tidy()
{
    closeAudioManager();
    freeWavInfo(rendered);
    exit(0);
}