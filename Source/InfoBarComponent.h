#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

// The InfoBarComponent class is intended to be a graphical element that contains a message.
// The message is manipulated through other modules and communicates status and errors to the user.
class InfoBarComponent   : public Component,
                           private Timer
{
public:
   InfoBarComponent (String* messageToDisplay);
   ~InfoBarComponent();
   
   // Component virtual functions
   void paint (Graphics& g) override;
   void resized() override;
   
   // Timer virtual functions
   void timerCallback() override;

private:
   String* messageToDisplay;
   
   JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InfoBarComponent)
};