#include <gtk/gtk.h>
#include <stdio.h>
#include<unistd.h>
#include "../backend/audioManager.h"

PaStream* stream = (PaStream*)0;
WavInfo* w = (WavInfo*) 0;


static void playAudio (GtkWidget *widget, gpointer data)
{
    g_print ("Playing Audio!\n");
    playFile(stream, w);
}

static void pauseSound (GtkWidget *widget, gpointer data){
  g_print ("Pausing Audio!\n");
  pauseAudio(stream,w);
}

static void stopSound (GtkWidget *widget, gpointer data){
  g_print ("Stopping Audio!\n");
  stopAudio(stream,w);
}



static void activate (GtkApplication *app, gpointer user_data)
{
  GtkBuilder *build;
  GtkWidget *window;
  GtkWidget *playButton;
  GtkWidget *pauseButton;
  GtkWidget *stopButton;
  GtkWidget *elapsedLabel;
  GtkWidget *totalLabel;


  build = gtk_builder_new_from_file ("src/frontend/moredaw.ui");
  window = GTK_WIDGET (gtk_builder_get_object (build, "window"));

  playButton = GTK_WIDGET (gtk_builder_get_object (build, "playButton"));
  pauseButton = GTK_WIDGET (gtk_builder_get_object (build, "pauseButton"));
  stopButton = GTK_WIDGET (gtk_builder_get_object (build, "stopButton"));
  elapsedLabel = GTK_WIDGET (gtk_builder_get_object (build, "elapsedTime"));
  totalLabel = GTK_WIDGET (gtk_builder_get_object (build, "totalTime"));

  g_signal_connect(playButton,"clicked", G_CALLBACK(playAudio),NULL);
  g_signal_connect(pauseButton,"clicked", G_CALLBACK(pauseSound),NULL);
  g_signal_connect(stopButton,"clicked", G_CALLBACK(stopSound),NULL);

  char* totalTimeString = malloc(5);
  char* elapsedTimeString = malloc(5);

  sprintf(elapsedTimeString,"%d:%d/",getElapsedDuration(w) / 60, getElapsedDuration(w) % 60);
  sprintf(totalTimeString, "%d:%d",getTotalDuration(w) / 60, getTotalDuration(w) % 60);

  gtk_label_set_label(GTK_LABEL(elapsedLabel),elapsedTimeString);
  gtk_label_set_label(GTK_LABEL(totalLabel),totalTimeString);

  gtk_window_set_application (GTK_WINDOW (window), GTK_APPLICATION (app));

  gtk_window_present (GTK_WINDOW (window));
  
  g_object_unref(build);
}


int main (int argc, char **argv)
{
  w = malloc(sizeof(WavInfo));
  if (readWavFile("res/audio/ThisIsInst.wav", w) != 0){
      printf("file not found!\n");
      exit(1);
  }
  stream = initialise(w);
  
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);
  return status;
}

