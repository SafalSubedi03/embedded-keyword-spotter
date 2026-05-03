#include "audio_capture.h"
#include "config.h"
#include "ringbuffer.h"


#include<iostream>
#include<portaudio.h>

ringBuffer audio_buffer;



using namespace std;


int main()
{
    initilize_rb(&audio_buffer);
    PaError err = Pa_Initialize();
    checkError(err);

    

    if (isDisplayActive)
        printAvailableDevices();

    PaStreamParameters inputParams;
    inputParams.device                    = Pa_GetDefaultInputDevice();
    inputParams.channelCount              = CHANNELS;
    inputParams.sampleFormat              = paInt16;
    inputParams.suggestedLatency          = Pa_GetDeviceInfo(Pa_GetDefaultInputDevice())->defaultLowInputLatency;
    inputParams.hostApiSpecificStreamInfo = NULL;

    PaStreamParameters outputParams;
    outputParams.device                    = Pa_GetDefaultOutputDevice();
    outputParams.channelCount              = CHANNELS;
    outputParams.sampleFormat              = paInt16;
    outputParams.suggestedLatency          = Pa_GetDeviceInfo(Pa_GetDefaultOutputDevice())->defaultLowOutputLatency;
    outputParams.hostApiSpecificStreamInfo = NULL;

    cout << "Input:  [" << inputParams.device << "] " 
     << Pa_GetDeviceInfo(inputParams.device)->name << endl;
    cout << "Output: [" << outputParams.device << "] " 
     << Pa_GetDeviceInfo(outputParams.device)->name << endl;

    PaStream *stream;
    err = Pa_OpenStream(
        &stream,
        &inputParams,
        &outputParams,
        SAMPLE_RATE,
        FRAMES_PER_BUFFER,
        paClipOff,
        audioCallback,
        &audio_buffer
    );
    checkError(err);

    cout << "\nMic passthrough active — speak into mic to hear yourself..." << endl;
    cout << "Press Ctrl+C to stop." << endl;

    err = Pa_StartStream(stream);
    checkError(err);

    while (Pa_IsStreamActive(stream))
        Pa_Sleep(100);

    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();

    cout << "Done." << endl;
    return 0;
}