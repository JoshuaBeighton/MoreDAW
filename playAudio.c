#include "playAudio.h"
#include "wavHandler.h"
#include <stdio.h>

/* This routine will be called by the PortAudio engine when audio is needed.
 * It may called at interrupt level on some machines so don't do anything
 * that could mess up the system like calling malloc() or free().
 */
int patestCallback(const void *inputBuffer, void *outputBuffer,
                   unsigned long framesPerBuffer,
                   const PaStreamCallbackTimeInfo *timeInfo,
                   PaStreamCallbackFlags statusFlags, void *userData) {
  /* Cast data passed through stream to our structure. */
  WavInfo *data = (WavInfo *)userData;
  short *out = (short *)outputBuffer;
  unsigned int i;
  (void)inputBuffer; /* Prevent unused variable warning. */

  for (i = 0; i < framesPerBuffer * data->channels; i++) {
    if ((void *)data->currentPointer - data->bulkData >= data->dataSize) {
      *out = 0;
    } else {
      //*(data->currentPointer) = swapEndian(*(data->currentPointer));
      *out = *data->currentPointer;
      data->currentPointer++;
    }
    out++;
  }
  printf("%li\n", (void *)data->currentPointer - data->bulkData);
  return 0;
}

short swapEndian(short n) {
  short leftByte = (n & 0x000000FF);
  short rightByte = (n & 0x0000FF00);
  leftByte = leftByte << 8;
  rightByte = rightByte >> 8;
  return leftByte | rightByte;
}