#include "../JuceLibraryCode/JuceHeader.h"
#include "Chirp.h"

Chirp::Chirp (int startingFreq, int endingFreq, float duration, int sampleRate)
:   currentFreq (startingFreq),
    endFreq (endingFreq),
    sampleRate (sampleRate),
    currentAngle (0.0),
    angleDelta (0.0),
    freqIncr ( (endingFreq - startingFreq) / (duration * (sampleRate / 1000.0)) )
{
}
Chirp::~Chirp() {}

void Chirp::updateAngleAndFreq ()
{
    // since the frequency is changing per sample, we need to recalculate the angle delta
    const double cyclesPerSample = currentFreq / sampleRate;
    angleDelta = cyclesPerSample * 2.0 * double_Pi;
    
    // then we update the current angle and frequency
    currentAngle += angleDelta; 
    currentFreq += freqIncr;
}

float Chirp::processSingleSample()
{
    if (currentFreq >= endFreq)
    {
        return 0.0;
    } else
    {
        return ( (float) std::sin(currentAngle) );
    }
}

float Chirp::getCurrentFreq() { return currentFreq; }
float Chirp::getFreqIncr() { return freqIncr; }