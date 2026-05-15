#include <iostream>
#include <portaudio.h>
#include <iomanip>
#include "config.h"
#include "ringbuffer.h"
using namespace std;




int audioCallback(const void *inputBuffer, void *outputBuffer,
                  unsigned long framesPerBuffer,
                  const PaStreamCallbackTimeInfo *,
                  PaStreamCallbackFlags,
                  void *userData)
{
    const int16_t *in = (const int16_t *)inputBuffer;
    ringBuffer    *rb = (ringBuffer *)userData;

    // outputBuffer is NULL on input-only streams — never touch it
    (void)outputBuffer;

    // write samples to ring buffer — this is all we need
    write_rb(rb, in, (int)framesPerBuffer);

    return paContinue;
}


void checkError(PaError err)
{
    if (err != paNoError)
    {
        cout << "PortAudio Error: " << Pa_GetErrorText(err) << endl;
        exit(EXIT_FAILURE);
    }
}




void printAvailableDevices()
{
    int numDevices = Pa_GetDeviceCount();
    if (numDevices < 0)
    {
        cout << "ERROR: Pa_GetDeviceCount returned " << numDevices << endl;
        return;
    }

    cout << left
         << setw(4)  << "Idx"
         << setw(40) << "Device Name"
         << setw(12) << "API"
         << setw(8)  << "In Ch"
         << setw(8)  << "Out Ch"
         << setw(12) << "Sample Rate"
         << endl;

    cout << string(84, '-') << endl;

    for (int i = 0; i < numDevices; i++)
    {
        const PaDeviceInfo  *deviceInfo = Pa_GetDeviceInfo(i);
        const PaHostApiInfo *apiInfo    = Pa_GetHostApiInfo(deviceInfo->hostApi);

        cout << left
             << setw(4)  << i
             << setw(40) << deviceInfo->name
             << setw(12) << apiInfo->name
             << setw(8)  << deviceInfo->maxInputChannels
             << setw(8)  << deviceInfo->maxOutputChannels
             << setw(12) << deviceInfo->defaultSampleRate
             << endl;
    }

    cout << string(84, '-') << endl;
}

