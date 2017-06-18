#ifndef SILENTSOURCE_H_INCLUDED
#define SILENTSOURCE_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class SilentSource     : public AudioSource
{
public:
    SilentSource ();
    ~SilentSource();
    
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock (const AudioSourceChannelInfo&) override;
    
private:
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SilentSource)
};

#endif
