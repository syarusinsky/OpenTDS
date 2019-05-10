#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

// The Chirp class is not an AudioSource, but instead intended to be used to process single samples
// of a sine wave sweep between two frequencies within a given duration of time with a given sample rate
class Chirp
{
public:
   Chirp (int startingFreq, int endingFreq, float duration, int sampleRate); // duration should be given in seconds
   ~Chirp();
   
   void updateAngleAndFreq(); // since the sine sweep is changing frequencies on every sample, the current angle must be updated per sample for use with sin()
   float processSingleSample(); // returns a single sample of the sweep, but does not update the angle since that should be done manually
   float getCurrentFreq();
   float getFreqIncr();
   
private:
   int endFreq, sampleRate;
   double currentFreq, currentAngle, angleDelta, freqIncr;
   
   JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Chirp)
};