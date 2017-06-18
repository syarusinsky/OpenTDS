#include "../JuceLibraryCode/JuceHeader.h"
#include "TdsSweep.h"

TdsSweep::TdsSweep (int startF, int endF, float sweepT, int bandW, float del, int blockS, int sampleR, AudioSourcePlayer* sourcePlayer, AudioSource* source, String* mTD, Array<float>* gBins, Value* gF)
:   startFreq (startF),
    endFreq (endF),
    bandWidth (bandW),
    blockSize (blockS),
    sampleRate (sampleR),
    tdsBinsSize (0),
    sweepTime (sweepT),
    delay (del),
    currentFilterFreq (startF),
    messageToDisplay (mTD),
    graphFlag (gF),
    graphBins (gBins),
    sineSweep ( new Chirp (startFreq, endFreq, sweepTime, sampleRate) ),
    player (sourcePlayer),
    audioSource (source)
{
}

TdsSweep::~TdsSweep()
{
    fft = nullptr;
}

void TdsSweep::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    *(messageToDisplay) = String ("Sweeping...");
    const int order = ( log (blockSize) / log (2) );
        
    fft = new FFT ( order, false );
    fftBins.resize ( (fft->getSize() * 2) );
        
    tdsBins.resize (200000);
}

void TdsSweep::releaseResources()
{
}

void TdsSweep::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    const ScopedLock sl (lock);
    
    float* const bufferWritePointer = bufferToFill.buffer->getWritePointer (0);
    const float* const bufferReadPointer = bufferToFill.buffer->getReadPointer (0);
    
                        
    const float currentFreq = sineSweep->returnCurrentFreq();
    const float freqIncr = sineSweep->returnFreqIncr();
    
    currentFilterFreq = currentFreq - ( freqIncr * (delay * sampleRate) );
                        
    for (int sample = 0; sample < bufferToFill.numSamples; sample++) 
    {
        float processedAudio;
                            
        const float filterFreq = currentFilterFreq + (freqIncr * sample);
        const double Q = filterFreq / bandWidth;
                            
        if (Q > 0)
        {
            const IIRCoefficients iC = IIRCoefficients::makeBandPass (sampleRate, filterFreq, Q);
                            
            bPFilter.setCoefficients (iC);
                            
            processedAudio = bPFilter.processSingleSampleRaw ( bufferReadPointer[sample] );
        } else {
            processedAudio = bufferReadPointer[sample];
        }
                            
        const float hanningWindowMultiplier = 0.5 * ( 1 - cos ((2 * double_Pi * sample)/(bufferToFill.numSamples - 1)) );
        fftBins.set ( sample, (hanningWindowMultiplier * processedAudio) );
    }
                        
    if (currentFilterFreq > 0.0 && currentFilterFreq < endFreq)
    {
        fft->performRealOnlyForwardTransform ( fftBins.getRawDataPointer() );
                        
        float avgMagnitude = 0.0;
                        
        for (int i = 1; i < fft->getSize(); i++)
        {
            avgMagnitude += sqrt (square (fftBins[(i * 2)]) + square (fftBins[(i * 2) + 1]));
        }
                        
        avgMagnitude = (avgMagnitude / fft->getSize());
                        
        tdsBins.set ( tdsBinsSize, currentFilterFreq );
        tdsBinsSize++;
        tdsBins.set ( tdsBinsSize, avgMagnitude );
        tdsBinsSize++;
        
    } else if (currentFilterFreq >= endFreq)
    {
        tdsBins.resize (tdsBinsSize);
        *(graphBins) = tdsBins;
        
        *(messageToDisplay) = String ("Sweep Completed");
        
        currentFilterFreq = 0.0;
        sineSweep = nullptr;
        player->setSource (audioSource);
        graphFlag->setValue (true);
    }
    
    
    const float level = 0.125f;
                        
    if (sineSweep != nullptr)
    {
        for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
        {
            sineSweep->updateAngleAndFreq ();
            const float currentSample = sineSweep->processSingleSample();
            bufferWritePointer[sample] = currentSample * level;
        }
    } else
    {
        bufferToFill.clearActiveBufferRegion();
    }
}