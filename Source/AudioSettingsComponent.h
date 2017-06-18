#ifndef AUDIOSETTINGSCOMPONENT_H_INCLUDED
#define AUDIOSETTINGSCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class AudioSettingsComponent    : public Component,
                                  public Button::Listener
{
public:
    AudioSettingsComponent (AudioDeviceManager& dm, unsigned int inputs, unsigned int outputs);
    ~AudioSettingsComponent();

    void paint (Graphics& g) override;
    void resized() override;
    
    void showAudioDeviceSelectorWindow();
    
    void buttonClicked (Button* button) override;

private:
    ScopedPointer<TextButton> audioSettingsButton;
    
    ScopedPointer<AudioDeviceSelectorComponent> audioDeviceSelector;
    
    SafePointer<DialogWindow> audioDeviceSelectorWindow;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioSettingsComponent)
};


#endif
