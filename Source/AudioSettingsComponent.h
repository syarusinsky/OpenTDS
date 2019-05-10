#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

// The AudioSettingsComponent class is used to launch a window that displays audio settings when a button is pressed.
class AudioSettingsComponent   : public Component,
                                 public Button::Listener
{
public:
   AudioSettingsComponent (AudioDeviceManager& deviceManager, unsigned int inputs, unsigned int outputs);
   ~AudioSettingsComponent();
   
   void showAudioDeviceSelectorWindow();
   
   // component virtual functions
   void paint (Graphics& g) override;
   void resized() override;
   
   // button::listener virtual functions
   void buttonClicked (Button* button) override;

private:
   std::unique_ptr<TextButton> audioSettingsButton;
   
   std::unique_ptr<AudioDeviceSelectorComponent> audioDeviceSelector;
   
   SafePointer<DialogWindow> audioDeviceSelectorWindow;
   
   JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioSettingsComponent)
};