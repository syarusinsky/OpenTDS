#ifndef INFOBARCOMPONENT_H_INCLUDED
#define INFOBARCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class InfoBarComponent     : public Component,
                             private Timer
{
public:
    InfoBarComponent (String* mTD);
    ~InfoBarComponent();
    
    void paint (Graphics& g) override;
    void resized() override;
    
    void timerCallback() override;
    
private:
    String* messageToDisplay;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InfoBarComponent)
};

#endif