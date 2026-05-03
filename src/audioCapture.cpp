//To capture audio form the microphone and store in a ring buffer using callback 

#include <iostream>
#include <thread>
#include<portaudio.h>
#include <iomanip>
using namespace std;


bool isdisplayactive = true;
static void checkError(PaError err)
{
    if (err != paNoError)
    {
        cout << "Error:- " << Pa_GetErrorText(err) << endl;
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

    cout << "\n================ Available Audio Devices ================\n";

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
        const PaDeviceInfo* deviceInfo = Pa_GetDeviceInfo(i);
        const PaHostApiInfo* apiInfo =
            Pa_GetHostApiInfo(deviceInfo->hostApi);

        cout << left
             << setw(4)  << i
             << setw(40) << deviceInfo->name
             << setw(12) << apiInfo->name
             << setw(8)  << deviceInfo->maxInputChannels
             << setw(8)  << deviceInfo->maxOutputChannels
             << setw(12) << deviceInfo->defaultSampleRate
             << endl;
    }

    cout << "=========================================================\n";
}

int main(){
    
    cout << "=== PortAudio Test ===" << endl;

    PaError err = Pa_Initialize();
    checkError(err);

    if(isdisplayactive){
        printAvailableDevices();
        return 0;
    }  

    int inputMic = 0;
    int outputSpeaker = 0;

    PaStreamParameters inputParameter;
    PaStreamParameters outputParameter;

    inputParameter.device = inputMic;
    inputParameter.channelCount = 1;
    inputParameter.sampleFormat = paInt16;
    inputParameter.hostApiSpecificStreamInfo = NULL;
    inputParameter.suggestedLatency = Pa_GetDeviceInfo(inputMic)->defaultLowInputLatency;


    outputParameter.device = outputSpeaker;
    outputParameter.channelCount = 1;
    outputParameter.sampleFormat = paInt16;
    outputParameter.hostApiSpecificStreamInfo = NULL;
    outputParameter.suggestedLatency = Pa_GetDeviceInfo(outputSpeaker)->defaultLowOutputLatency;

    

}
