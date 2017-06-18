#ifndef TDSCOMPONENTS_H_INCLUDED
#define TDSCOMPONENTS_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "FrequencyGraphComponent.h"
#include "TdsSweep.h"

class TdsComponents     : public Component,
                          public Button::Listener
{
public:
    TdsComponents (int* lC, AudioDeviceManager* dm, AudioSourcePlayer* sourcePlayer, AudioSource* source, String* mTD);
    ~TdsComponents();
    
    void paint (Graphics& g) override;
    void resized() override;
    
    void buttonClicked (Button* button) override;
    
private:
    int startF, endF, bandW;
    float sweepT, delay;
    
    int* latencyComp;
    
    String* messageToDisplay;
    
    Value graphFlag;
    
    ScopedPointer<Array<float>> graphBins;
    
    ScopedPointer<TextEditor> startFreq;
    ScopedPointer<TextEditor> endFreq;
    ScopedPointer<TextEditor> sweepTime;
    ScopedPointer<TextEditor> bandwidth;
    ScopedPointer<TextEditor> timeOfFlight;
    ScopedPointer<TextEditor> temperature;
    ScopedPointer<TextEditor> distance;
    
    ScopedPointer<TextButton> sweepButton;
    
    ScopedPointer<FrequencyGraphComponent> frequencyGraphComponent;
    
    ScopedPointer<TdsSweep> tdsSweep;
    
    AudioDeviceManager* deviceManager;
    
    AudioSourcePlayer* player;
    AudioSource* audioSource;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TdsComponents)
};

#endif