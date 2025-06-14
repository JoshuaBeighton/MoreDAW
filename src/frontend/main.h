#include <gtk/gtk.h>
#include <stdio.h>
#include<unistd.h>
#include "../backend/audioManager.h"

static void playAudio (GtkWidget *widget, gpointer data);
static void pauseSound (GtkWidget *widget, gpointer data);
static void stopSound (GtkWidget *widget, gpointer data);
static void recordSound (GtkWidget *widget, gpointer data);
static void activateHeader(GtkApplication *app, gpointer user_data, GtkBuilder *build);
static void activateBody(GtkApplication *app, gpointer user_data, GtkBuilder *build);
static void activate (GtkApplication *app, gpointer user_data);
static void loadCSS(GtkApplication *app, gpointer user_data);
static void saveSound(GtkWidget *widget, gpointer data);
int main (int argc, char **argv);