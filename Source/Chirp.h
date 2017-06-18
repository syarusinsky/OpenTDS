#ifndef CHIRP_H_INCLUDED
#define CHIRP_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class Chirp
{
    public:
    
        Chirp(int sF, int eF, float dur, int sR);
        ~Chirp();
    
        void updateAngleAndFreq ();
        const float processSingleSample();
        const float returnCurrentFreq();
        const float returnFreqIncr();
        
    private:
    
        int endFreq, sampleRate;
        double currentFreq, currentAngle, angleDelta, freqIncr, delay;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Chirp)
};

#endif
