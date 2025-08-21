#include <gtk/gtk.h>
#include <stdio.h>
#include <unistd.h>
#include "../backend/audioManager.h"
#include "waveformManager.h"
#include "track.h"

static void playAudio(GtkWidget *widget, gpointer data);
static void pauseSound(GtkWidget *widget, gpointer data);
static void stopSound(GtkWidget *widget, gpointer data);
static void recordSound(GtkWidget *widget, gpointer data);
static void activateHeader(GtkApplication *app);
static void activateToolbar();
static void activateBody(TrackList* toShow);
static void activate(GtkApplication *app, gpointer user_data);
static void loadCSS(GtkApplication *app, gpointer user_data);
static void saveSound(GtkWidget *widget, gpointer data);
static void openFile(GSimpleAction *action,
                     GVariant *parameter,
                     gpointer user_data);
static void on_file_chosen(GObject *source,
                           GAsyncResult *result,
                           gpointer user_data);
static void tidy();

typedef enum
{
    STOPPED,
    RECORDING,
    PLAYING
} State;

int main(int argc, char **argv);