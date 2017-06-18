#ifndef TDSSWEEP_H_INCLUDED
#define TDSSWEEP_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Chirp.h"

class TdsSweep  : public AudioSource
{
    public:
    
        TdsSweep (int startF, int endF, float sweepT, int bandW, float del, int blockS, int sampleR, AudioSourcePlayer* sourcePlayer, AudioSource* source, String* mTD, Array<float>* gBins, Value* gF);
        ~TdsSweep();
    
        void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
        void releaseResources() override;
        void getNextAudioBlock (const AudioSourceChannelInfo&) override;
    
    private:
    
        int startFreq, endFreq, bandWidth, blockSize, sampleRate, tdsBinsSize; 
        float sweepTime, delay, currentFilterFreq;
    
        String* messageToDisplay;
        
        Value* graphFlag;
    
        IIRFilter bPFilter;
    
        ScopedPointer<FFT> fft;
    
        Array<float> fftBins;
        Array<float> tdsBins;
    
        Array<float>* graphBins;
    
        ScopedPointer<Chirp> sineSweep;
    
        AudioSourcePlayer* player;
        AudioSource* audioSource;
    
        CriticalSection lock;
    
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TdsSweep)
};

#endif