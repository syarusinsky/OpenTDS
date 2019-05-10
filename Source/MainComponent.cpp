/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
:   inputChannels (1),
    outputChannels (1),
    bufferSize (0),
    latencyOffset (0),
    message ( String ("Please Calibrate Latency (plug output into input)") ),
    audioSettingsComponent ( std::make_unique<AudioSettingsComponent> (deviceManager, inputChannels, outputChannels) ),
    silentSource ( std::make_unique<SilentSource>() ),
    sourcePlayer ( std::make_unique<AudioSourcePlayer>() ),
    infoBarComponent ( std::make_unique<InfoBarComponent> (&message) ),
    tdsComponents ( std::make_unique<TdsComponents> ( &latencyOffset, &deviceManager, sourcePlayer.get(), silentSource.get(), &message ) )
{
    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (inputChannels, outputChannels); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (inputChannels, outputChannels);
    }
    
    sourcePlayer->setSource (silentSource.get());
    deviceManager.addAudioCallback (sourcePlayer.get());
    
    calibrateButton = std::make_unique<TextButton> ("Calibrate Latency");
        
    addAndMakeVisible (audioSettingsComponent.get());
    addAndMakeVisible (calibrateButton.get());
    addAndMakeVisible (infoBarComponent.get());
    addAndMakeVisible (tdsComponents.get());
    
    calibrateButton->addListener (this);
    
    setSize (800, 600);
}

MainComponent::~MainComponent()
{
    calibrateButton->removeListener (this);
    
    deviceManager.removeAudioCallback (sourcePlayer.get());
    
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    bufferSize = samplesPerBlockExpected;
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();
}

void MainComponent::releaseResources() {}

//==============================================================================
void MainComponent::paint (Graphics& g)
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

void MainComponent::resized() {}

// this method initializes a LatencyCalibrator if the calibrate button is pressed
void MainComponent::buttonClicked (Button* button)
{
    if (button == calibrateButton.get() && sourcePlayer->getCurrentSource() == silentSource.get())
    {
        latencyCalibrator = std::make_unique<LatencyCalibrator> ( deviceManager, sourcePlayer.get(), silentSource.get(), &latencyOffset, &message );
        sourcePlayer->setSource (latencyCalibrator.get());
    } else
    {
        message = String ("Please Wait for Sweep or Calibration to Complete...");
    }
}