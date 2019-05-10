#include "../JuceLibraryCode/JuceHeader.h"
#include "TdsSweep.h"

TdsSweep::TdsSweep (int startFreq, int endFreq, float sweepTime, int bandWidth, float delay, int blockSize, int sampleRate, AudioSourcePlayer* sourcePlayer, AudioSource* source, String* messageToDisplay, Array<float>* graphingBins, Value* graphingFlag)
:   startFreq (startFreq),
    endFreq (endFreq),
    bandWidth (bandWidth),
    blockSize (blockSize),
    sampleRate (sampleRate),
    tdsBinsSize (0),
    sweepTime (sweepTime),
    delay (delay),
    currentFilterFreq (startFreq),
    messageToDisplay (messageToDisplay),
    graphFlag (graphingFlag),
    graphBins (graphingBins),
    sineSweep ( std::make_unique<Chirp> (startFreq, endFreq, sweepTime, sampleRate) ),
    player (sourcePlayer),
    audioSource (source)
{
}
TdsSweep::~TdsSweep() {}

void TdsSweep::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    *(messageToDisplay) = String ("Sweeping...");
    
    const int order = ( log(blockSize) / log(2) ); // the number of points the fft will operate on 
    
    // preparing the FFT object, fftBins, and tdsBins for use in getNextAudioBlock
    fft = std::make_unique<juce::dsp::FFT> (order);
    fftBins.resize ( (fft->getSize() * 2) );
    tdsBins.resize (200000);
}

void TdsSweep::releaseResources() {}

void TdsSweep::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    const ScopedLock sl (lock); // gaining access to the critical section, the remaining function is thread-safe
    
    float* const bufferWritePointer = bufferToFill.buffer->getWritePointer (0);
    const float* const bufferReadPointer = bufferToFill.buffer->getReadPointer (0);
    
    const float currentFreq = sineSweep->getCurrentFreq();
    const float freqIncr = sineSweep->getFreqIncr();
    
    currentFilterFreq = currentFreq - ( freqIncr * (delay * sampleRate) ); // since the filter must lag behind the direct sound
    
    // reading, filtering, and windowing the buffer to be processed by FFT and placed in TDS bins later
    for (int sample = 0; sample < bufferToFill.numSamples; sample++)
    {
        float processedSample;
        
        const float filterFreq = currentFilterFreq + (freqIncr * sample); // the filter must sweep as it passes through the buffer
        const double Q = filterFreq / bandWidth;
        
        if (Q > 0)
        {
            const IIRCoefficients iirC = IIRCoefficients::makeBandPass (sampleRate, filterFreq, Q);
            
            bPFilter.setCoefficients (iirC);
            
            processedSample = bPFilter.processSingleSampleRaw ( bufferReadPointer[sample] );
        } else 
        {
            processedSample = bufferReadPointer[sample];
        }
        
        const float hanningWindowMultiplier = 0.5 * ( 1 - cos ((2 * double_Pi * sample)/(bufferToFill.numSamples - 1)) ); // sample should be windowed for processing by FFT
        fftBins.set ( sample, (hanningWindowMultiplier * processedSample) );
    }
    
    // placing the processed block magnitudes and frequencies into the TDS bins for graphing
    if (currentFilterFreq > 0.0 && currentFilterFreq < endFreq)
    {
        fft->performRealOnlyForwardTransform ( fftBins.getRawDataPointer() );
        
        // calculating the average magnitude for this audio block filtered (in the future this process should only select the bins the filter passed through)
        float avgMagnitude = 0.0;
        for (int i = 1; i < fft->getSize(); i++)
        {
            avgMagnitude += sqrt (square (fftBins[(i * 2)]) + square (fftBins[(i * 2) + 1]));
        }
        avgMagnitude = (avgMagnitude / fft->getSize());
        
        // adding the currentFilterFreq and avgMagnitude to their associated TDS bins
        tdsBins.set ( tdsBinsSize, currentFilterFreq );
        tdsBinsSize++;
        tdsBins.set ( tdsBinsSize, avgMagnitude );
        tdsBinsSize++;
    } else if (currentFilterFreq >= endFreq)
    {
        // we now have the size of the tds bins, so we can resize the array and assign the graphBins pointer to tdsBins for graphing by graphing module
        tdsBins.resize (tdsBinsSize);
        *(graphBins) = tdsBins;
        
        *(messageToDisplay) = String ("Sweep Completed");
        
        currentFilterFreq = 0.0;
        player->setSource (audioSource); // since our sweep is completed we need a new audio source
        graphFlag->setValue (true); // inform graphing module that we're ready to begin graphing
    }
    
    const float level = 0.125f; // sine sweep (chirp) level
    
    // outputting the chirp
    if (sineSweep != nullptr)
    {
        for (int sample = 0; sample < bufferToFill.numSamples; sample++)
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