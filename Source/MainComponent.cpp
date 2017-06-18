#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioSettingsComponent.h"
#include "LatencyCalibrator.h"
#include "SilentSource.h"
#include "InfoBarComponent.h"
#include "TdsComponents.h"

class MainContentComponent   : public AudioAppComponent,
                               public Button::Listener
{
public:
    MainContentComponent()
    :   inputChannels (1),
        outputChannels (2),
        bufferSize (0),
        latencyOffset (0),
        message ( String ("Please Calibrate Latency (plug output into input)") ),
        audioSettingsComponent ( new AudioSettingsComponent (deviceManager, inputChannels, outputChannels) ),
        silentSource ( new SilentSource() ),
        sourcePlayer ( new AudioSourcePlayer() ),
        infoBarComponent ( new InfoBarComponent (&message) ),
        tdsComponents ( new TdsComponents ( &latencyOffset, &deviceManager, sourcePlayer.get(), silentSource.get(), &message ) )
    {
        setSize (800, 600);
        
        sourcePlayer->setSource (silentSource);
        deviceManager.addAudioCallback (sourcePlayer);

        setAudioChannels (inputChannels, outputChannels);
        
        addAndMakeVisible (audioSettingsComponent);
        addAndMakeVisible (calibrateButton = new TextButton ("Calibrate Latency"));
        addAndMakeVisible (infoBarComponent);
        addAndMakeVisible (tdsComponents);
        
        calibrateButton->addListener (this);
    }

    ~MainContentComponent()
    {
        calibrateButton->removeListener (this);
        
        deviceManager.removeAudioCallback (sourcePlayer);
        
        sourcePlayer = nullptr;
        
        shutdownAudio();
    }

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        bufferSize = samplesPerBlockExpected;
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        bufferToFill.clearActiveBufferRegion();
    }

    void releaseResources() override
    {
    }

    void paint (Graphics& g) override
    {
        g.fillAll (Colours::lightgrey);
        
        Rectangle<int> area = getLocalBounds();
    
        const int headerFooterHeight = 20;
        
        audioSettingsComponent->setBounds ( area.removeFromTop (headerFooterHeight) );
        
        int buttonWidth = 100;
        
        area = getLocalBounds();
        area.removeFromLeft ( getWidth() - buttonWidth );
        
        calibrateButton->setBounds ( area.removeFromBottom (headerFooterHeight) );
        
        area = getLocalBounds();
        area.removeFromRight ( buttonWidth );
        
        infoBarComponent->setBounds ( area.removeFromBottom (headerFooterHeight) );
        
        area = getLocalBounds();
        area.removeFromTop (headerFooterHeight);
        area.removeFromBottom (headerFooterHeight);
        
        tdsComponents->setBounds (area);
    }

    void resized() override
    {
    }
    
    void buttonClicked (Button* button) override
    {
        if (button == calibrateButton && sourcePlayer->getCurrentSource() == silentSource)
        {
            latencyCalibrator = new LatencyCalibrator ( deviceManager, sourcePlayer.get(), silentSource.get(), &latencyOffset, &message );
            sourcePlayer->setSource (latencyCalibrator);
        } else
        {
            message = String ("Please Wait for Sweep or Calibration to Complete...");
        }
    }

private:
    int inputChannels, outputChannels, bufferSize, latencyOffset;
    
    String message;
    
    ScopedPointer<TextButton> calibrateButton;
    
    ScopedPointer<AudioSourcePlayer> sourcePlayer;
    
    ScopedPointer<AudioSettingsComponent> audioSettingsComponent;
    
    ScopedPointer<LatencyCalibrator> latencyCalibrator;
    
    ScopedPointer<SilentSource> silentSource;
    
    ScopedPointer<InfoBarComponent> infoBarComponent;
    
    ScopedPointer<TdsComponents> tdsComponents;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif
