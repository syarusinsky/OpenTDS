#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

// The LatencyCalibrator class is an AudioSource that is intended to be used when calibrating the latency of your
// audio interface for use with the TDS Sweep. Since the delay is crucial to an accurate TDS measurement, latency is
// calibrated by sending a train of spikes from the output of your interface to the input of your interface and
// this module calculates the latency.
class LatencyCalibrator   : public AudioSource,
                            private Timer
{
public:
   LatencyCalibrator (AudioDeviceManager& deviceManager, AudioSourcePlayer* sourcePlayer, AudioSource* source, int* latencyOffset, String* messageToDisplay);
   ~LatencyCalibrator();
   
   void generateSpikeTrain(); // the spike train is sent from the output of your audio interface, to the input of your audio interface
   void parseRecording(); // this module records the input and parses it to calculate the roundtrip time
   
   // AudioSource virtual functions
   void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
   void releaseResources() override;
   void getNextAudioBlock (const AudioSourceChannelInfo&) override;
   
   // Timer virtual functions
   void timerCallback() override;
   
private:
   bool spikeTrainPlaying, running;
   int spikeTrainSampleNum, recordingSampleNum, spikeIncr, outputLatency;
   
   int* latencyOffset;
    
   String* messageToDisplay;
   
   AudioSourcePlayer* player;
   AudioSource* newSource;
   
   // Buffers to store the recordings of both the generated spike train and the recorded spike train from the input of the audio interface
   std::unique_ptr<AudioSampleBuffer> spikeTrain;
   std::unique_ptr<AudioSampleBuffer> recordingBuffer;
   
   CriticalSection lock;
   
   JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LatencyCalibrator)
};
