#include "portaudio.h"
#include "wavHandler.h"
#include "playAudio.h"
#include "recordAudio.h"
#include <stdio.h>
#include <stdlib.h>
PaStream* initialise(WavInfo* w);
void playFile(PaStream*, WavInfo * w);

void pauseAudio(PaStream*, WavInfo * w);

void stopAudio(PaStream* stream, WavInfo* w);

void startRecording(PaStream** stream, WavInfo* w);

void stopRecording(PaStream* stream, WavInfo* w);

void closeAudioManager();