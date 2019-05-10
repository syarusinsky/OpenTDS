/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioSettingsComponent.h"
#include "LatencyCalibrator.h"
#include "SilentSource.h"
#include "InfoBarComponent.h"
#include "TdsComponents.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent,
                        public Button::Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;
    
    void buttonClicked (Button* button) override;

private:
    //==============================================================================
    // Your private member variables go here...
    
    const int inputChannels = 1; // the only input to the system should be the ouput of the DUT, so currently only mono input supported
    const int outputChannels = 1; // since only mono input supported, currently only mono output is supported
    int bufferSize, latencyOffset = 0; // buffer size will be set by the user, while latency offset will be set by the calibration system (see LatencyCalibrator)
    
    String message; // this string is used to display messages on the screen regarding the current operation of the system
    
    std::unique_ptr<TextButton> calibrateButton;
    
    std::unique_ptr<AudioSourcePlayer> sourcePlayer;
    
    std::unique_ptr<AudioSettingsComponent> audioSettingsComponent;
    
    std::unique_ptr<LatencyCalibrator> latencyCalibrator;
    
    std::unique_ptr<SilentSource> silentSource;
    
    std::unique_ptr<InfoBarComponent> infoBarComponent;
    
    std::unique_ptr<TdsComponents> tdsComponents;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
