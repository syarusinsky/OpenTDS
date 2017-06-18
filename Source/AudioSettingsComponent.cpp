#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioSettingsComponent.h"


AudioSettingsComponent::AudioSettingsComponent ( AudioDeviceManager& dm, unsigned int inputs, unsigned int outputs )
{
    addAndMakeVisible ( audioSettingsButton = new TextButton ("Audio Settings") );
    
    audioDeviceSelector = new AudioDeviceSelectorComponent ( dm, inputs, inputs, outputs, outputs, false, false, false, false );
    
    audioSettingsButton->addListener (this);
}

AudioSettingsComponent::~AudioSettingsComponent()
{
    audioSettingsButton->removeListener (this);
}

void AudioSettingsComponent::paint (Graphics& g)
{
}

void AudioSettingsComponent::resized()
{
    int buttonWidth = 100;
    Rectangle<int> area = getLocalBounds();
    area.removeFromLeft ( getWidth() - buttonWidth );
    audioSettingsButton->setBounds (area);
}

void AudioSettingsComponent::showAudioDeviceSelectorWindow()
{
    DialogWindow::LaunchOptions options;
    options.content.setNonOwned (audioDeviceSelector);
    
    const int width = 300;
    const int height = 500;

    Rectangle<int> area (0, 0, width, height);

    options.content->setSize ( area.getWidth(), area.getHeight() );

    options.dialogTitle                   = "Audio Settings";
    options.dialogBackgroundColour        = Colours::lightgrey;
    options.escapeKeyTriggersCloseButton  = true;
    options.useNativeTitleBar             = true;
    options.resizable                     = false;

    audioDeviceSelectorWindow = options.launchAsync();

    if (audioDeviceSelectorWindow != nullptr)
    {
        audioDeviceSelectorWindow->centreWithSize ( width, height );
    }
}

void AudioSettingsComponent::buttonClicked (Button* button)
{
    if (button == audioSettingsButton)
    {
        showAudioDeviceSelectorWindow();
    }
}
