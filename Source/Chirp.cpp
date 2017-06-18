#include "../JuceLibraryCode/JuceHeader.h"
#include "Chirp.h"

Chirp::Chirp (int sF, int eF, float dur, int sR)
:   currentFreq (sF),
    endFreq (eF),
    sampleRate (sR),
    currentAngle (0.0),
    angleDelta (0.0),
    freqIncr ( (eF - sF) / (dur * (sampleRate / 1000.0)) )
{
}

Chirp::~Chirp()
{
}

void Chirp::updateAngleAndFreq ()
{
    const double cyclesPerSample = currentFreq / sampleRate;
    angleDelta = cyclesPerSample * 2.0 * double_Pi;
    
    currentAngle += angleDelta;
    currentFreq += freqIncr;
}

const float Chirp::processSingleSample()
{
    if (currentFreq >= endFreq)
    {
        return (0.0);
    } else
    {
        return ((float) std::sin (currentAngle));
    }
}

const float Chirp::returnCurrentFreq()
{
    return currentFreq;
}

const float Chirp::returnFreqIncr()
{
    return freqIncr;
}