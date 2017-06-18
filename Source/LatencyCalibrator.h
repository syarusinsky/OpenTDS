#ifndef LATENCYCALIBRATOR_H_INCLUDED
#define LATENCYCALIBRATOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class LatencyCalibrator     : public AudioSource,
                              private Timer
{
public:
    LatencyCalibrator (AudioDeviceManager& dm, AudioSourcePlayer* sourcePlayer, AudioSource* source, int* lO, String* mTD);
    ~LatencyCalibrator();
    
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock (const AudioSourceChannelInfo&) override;
    
    void timerCallback() override;
    
    void generateSpikeTrain();
    void parseRecording();
    
private:
    bool spikeTrainPlaying, running;
    int spikeTrainSampleNum, recordingSampleNum, spikeIncr, outputLatency;
    
    int* latencyOffset;
    
    String* messageToDisplay;
    
    AudioSourcePlayer* player;
    AudioSource* newSource;
    
    ScopedPointer<AudioSampleBuffer> spikeTrain;
    ScopedPointer<AudioSampleBuffer> recordingBuffer;
    
    CriticalSection lock;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LatencyCalibrator)
};


#endif