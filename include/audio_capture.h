#ifndef AUDIO_CAPTURE_H
#define AUDIO_CAPTURE_H

#include<portaudio.h>
#include<iostream>

#include "config.h"
int audioCallback(const void *inputBuffer,
                         void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo *timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *audioBuffer);


void checkError(PaError err);
void printAvailableDevices();
#endif