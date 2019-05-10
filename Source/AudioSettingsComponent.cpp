#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioSettingsComponent.h"

AudioSettingsComponent::AudioSettingsComponent ( AudioDeviceManager& deviceManager, unsigned int inputs, unsigned int outputs )
{
    audioSettingsButton = std::make_unique<TextButton> ("Audio Settings");
    addAndMakeVisible ( audioSettingsButton.get() );
    
    audioDeviceSelector = std::make_unique<AudioDeviceSelectorComponent> ( deviceManager, inputs, inputs, outputs, outputs, false, false, false, false );
    
    audioSettingsButton->addListener (this);
}

AudioSettingsComponent::~AudioSettingsComponent()
{
    audioSettingsButton->removeListener (this);
}

void AudioSettingsComponent::showAudioDeviceSelectorWindow()
{
    DialogWindow::LaunchOptions options;
    options.content.setNonOwned (audioDeviceSelector.get());
    
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

void AudioSettingsComponent::paint (Graphics& g) {}

void AudioSettingsComponent::resized()
{
    int buttonWidth = 100;
    Rectangle<int> area = getLocalBounds();
    area.removeFromLeft ( getWidth() - buttonWidth );
    audioSettingsButton->setBounds (area);
}

void AudioSettingsComponent::buttonClicked (Button* button)
{
    if (button == audioSettingsButton.get())
    {
        showAudioDeviceSelectorWindow();
    }
}