#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Chirp.h"

// The TdsSweep class is an AudioSource that both sends a sine sweep (chirp) to a device under test and records and processes the output audio from the
// device under test. This process of Time Delay Spectrometry results in "TDS Bins" which can be graphed to determine the frequency response of the
// device under test. These bins are passed back through the argument "graphingBins".
class TdsSweep   : public AudioSource
{
public:
   TdsSweep (int startFreq, int endFreq, float sweepTime, int bandWidth, float delay, int blockSize, int sampleRate, AudioSourcePlayer* sourcePlayer, AudioSource* source, 
             String* messageToDisplay, Array<float>* graphingBins, Value* graphingFlag);
   ~TdsSweep();
   
   // AudioSource virtual functions
   void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
   void releaseResources() override;
   void getNextAudioBlock (const AudioSourceChannelInfo&) override;
   
private:
   int startFreq, endFreq, bandWidth, blockSize, sampleRate, tdsBinsSize;
   float sweepTime, delay, currentFilterFreq;
   
   String* const messageToDisplay;
   
   Value* const graphFlag; // a pointer to the Value that acts as a flag signalling to graph the TDS bins
   
   IIRFilter bPFilter; // the tracking band-pass filter
   
   std::unique_ptr<juce::dsp::FFT> fft; // FFT object used to perform Fast Fourier Transforms, with the output sent to fftBins
   
   Array<float> fftBins; // the fftBins store the output of the FFT on each audio block, after first filling it with windowed samples
   Array<float> tdsBins; // the tdsBins store the result of the TDS sweep
   
   Array<float>* graphBins; // a pointer to Array<float> is passed into this object, so that this object can give it assign it to this object's tdsBins for graphing
   
   std::unique_ptr<Chirp> sineSweep; // a Chirp object for generating the necessary sine wave sweep to be used for TDS
   
   AudioSourcePlayer* player;
   AudioSource* audioSource;
   
   CriticalSection lock;
   
   JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TdsSweep)
};