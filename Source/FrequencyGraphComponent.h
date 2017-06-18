#ifndef FREQUENCYGRAPHCOMPONENT_H_INCLUDED
#define FREQUENCYGRAPHCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class FrequencyGraphComponent   : public Component,
                                  public Value::Listener
{
    public:
    
        FrequencyGraphComponent (Array<float>* gBins, Value* gF);
        ~FrequencyGraphComponent();
    
        void paint (Graphics& g) override;
        void resized() override;
        
        void valueChanged (Value& gF) override;
    
    private:
        Value* graphFlag;
    
        Array<float>* graphBins;
    
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FrequencyGraphComponent)
};

#endif