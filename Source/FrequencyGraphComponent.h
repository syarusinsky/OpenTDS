#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

// The FrequencyGraphComponent class is a graphical element that is used to 
// graph the TDS bins and display them to the user. It using a Value::Listener
// to determine when graphing should be performed.
class FrequencyGraphComponent   : public Component,
                                  public Value::Listener
{
public:
   FrequencyGraphComponent (Array<float>* graphingBins, Value* graphingFlag);
   ~FrequencyGraphComponent();
   
   // Component virtual functions
   void paint (Graphics& g) override;
   void resized() override;
   
   // Value::Listener virtual functions
   void valueChanged (Value& graphingFlag) override;
   
private:
   Value* graphFlag;
   
   Array<float>* graphBins;
   
   JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FrequencyGraphComponent)
};