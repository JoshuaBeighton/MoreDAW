#include "portaudio.h"
#include "wavHandler.h"
PaStream* initialise(WavInfo* w);
void playFile(PaStream*, WavInfo * w);

void pauseAudio(PaStream*, WavInfo * w);
