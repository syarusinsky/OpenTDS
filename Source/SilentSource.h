#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

// SilentSource is an audio source that produces silence.
class SilentSource   : public AudioSource
{
public:
   SilentSource();
   ~SilentSource();
   
   // AudioSource virtual functions
   void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
   void releaseResources() override;
   void getNextAudioBlock (const AudioSourceChannelInfo&) override;
   
private:
   JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SilentSource)
};