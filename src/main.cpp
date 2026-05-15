//  ┌─────────────────────────────────────────────────────┐
//  │  Required includes for main.cpp                     │
//  └─────────────────────────────────────────────────────┘

#include <iostream>      // cout, endl
#include <cstdio>        // printf
#include <cstdlib>       // exit, EXIT_FAILURE
#include <cstring>       // memset
#include <cstdint>       // int16_t
#include <iomanip>       // setw (for printAvailableDevices)

#include <portaudio.h>   // Pa_Initialize, Pa_OpenStream, etc.  (-lportaudio)

#include "config.h"      // SAMPLE_RATE, FRAMES_PER_BUFFER, CHANNELS, ...
#include "ringbuffer.h"  // ringBuffer, initilize_rb, write_rb, read_rb, available_rb
#include "audio_capture.h" // audioCallback, checkError, printAvailableDevices

using namespace std;

// ─── Global ring buffer ───────────────────────────────────────────────────────
ringBuffer *audio_buffer = nullptr;
// ─── Main ─────────────────────────────────────────────────────────────────────
int main()
{
    audio_buffer = new ringBuffer();
    initilize_rb(audio_buffer);

    PaError err = Pa_Initialize();
    checkError(err);

    if (isDisplayActive)
        printAvailableDevices();

    // ── Input device ──────────────────────────────────────────────────────────
    PaStreamParameters inputParams;
    inputParams.device                    = Pa_GetDefaultInputDevice();
    inputParams.channelCount              = CHANNELS;
    inputParams.sampleFormat              = paInt16;
    inputParams.suggestedLatency          = Pa_GetDeviceInfo(inputParams.device)
                                                ->defaultLowInputLatency;
    inputParams.hostApiSpecificStreamInfo = NULL;

    // ── Output device (kept NULL — input-only stream avoids feedback) ─────────
    // If you ever need passthrough again swap NULL back to &outputParams,
    // but make sure you use headphones, not laptop speakers.

    cout << "Input:  [" << inputParams.device << "] "
         << Pa_GetDeviceInfo(inputParams.device)->name << endl;
    cout << "Sample rate: " << SAMPLE_RATE << endl;

    // ── Open input-only stream ────────────────────────────────────────────────
    PaStream *stream;
    err = Pa_OpenStream(
        &stream,
        &inputParams,
        NULL,               // no output device — eliminates feedback entirely
        SAMPLE_RATE,
        FRAMES_PER_BUFFER,
        paClipOff,
        audioCallback,
        audio_buffer
    );
    checkError(err);

    cout << "\nCapturing audio — speak into mic..." << endl;
    cout << "Press Ctrl+C to stop." << endl;

    err = Pa_StartStream(stream);
    checkError(err);

    // ── Main thread: read from ring buffer and verify samples ─────────────────
    int16_t samples[FRAMES_PER_BUFFER];

    for (;;) {
    Pa_Sleep(100);

    if (available_rb(audio_buffer) >= FRAMES_PER_BUFFER) {
        int got = read_rb(audio_buffer, samples, FRAMES_PER_BUFFER);

        int16_t peak = 0;
        for (int i = 0; i < got; i++) {
            int16_t v = samples[i] < 0 ? -samples[i] : samples[i];
            if (v > peak) peak = v;
        }

        int bars = peak / 2000;
        printf("Peak: %6d  [", peak);
        for (int i = 0; i < 16; i++)
            printf("%s", i < bars ? "#" : " ");
        printf("]\n");
    }
}
    // ── Cleanup ───────────────────────────────────────────────────────────────
    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();

    cout << "Done." << endl;
    return 0;
}