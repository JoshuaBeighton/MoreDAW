#include "portaudio.h"
#include "wavHandler.h"
PaStream* initialise(WavInfo* w);
void playFile(PaStream*, WavInfo * w);

void pauseAudio(PaStream*, WavInfo * w);

void stopAudio(PaStream* stream, WavInfo* w);

void startRecording(PaStream* stream, WavInfo* w);

void stopRecording(PaStream* stream, WavInfo* w);