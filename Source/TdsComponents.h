#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "FrequencyGraphComponent.h"
#include "TdsSweep.h"

// The TdsComponents class contains the graphical elements for changing TDS settings and displaying frequency response, as well as the
// TdsSweep AudioSource to retrieve the TDS Bins to graph.
class TdsComponents     : public Component,
                          public Button::Listener
{
public:
   TdsComponents (int* lC, AudioDeviceManager* deviceManager, AudioSourcePlayer* sourcePlayer, AudioSource* source, String* messageToDisplay);
   ~TdsComponents();
   
   // Component virtual functions
   void paint (Graphics& g) override;
   void resized() override;
   
   // Button::Listener virtual functions
   void buttonClicked (Button* button) override;
   
private:
   int startF, endF, bandW;
   float sweepT, delay;
   
   int* latencyComp; // latency compensation achieved through latency calibration module
   
   String* messageToDisplay;
   
   Value graphFlag; // this Value is used as a flag to signal the FrequencyGraphComponent to begin graphing
   
   std::unique_ptr<Array<float>> graphBins; // these bins contain the frequency response of the system

   // various text boxes for user settings
   std::unique_ptr<TextEditor> startFreq;
   std::unique_ptr<TextEditor> endFreq;
   std::unique_ptr<TextEditor> sweepTime;
   std::unique_ptr<TextEditor> bandwidth;
   std::unique_ptr<TextEditor> timeOfFlight;
   std::unique_ptr<TextEditor> temperature;
   std::unique_ptr<TextEditor> distance;
    
   std::unique_ptr<TextButton> sweepButton;
    
   std::unique_ptr<FrequencyGraphComponent> frequencyGraphComponent; // the graph used for graphing the graphBins (displays the frequency response)
    
   std::unique_ptr<TdsSweep> tdsSweep; // the AudioSource that obtains the graphBins
    
   AudioDeviceManager* deviceManager;
    
   AudioSourcePlayer* player;
   AudioSource* audioSource;
    
   JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TdsComponents)
};