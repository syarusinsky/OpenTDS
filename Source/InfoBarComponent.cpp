#include "../JuceLibraryCode/JuceHeader.h"
#include "InfoBarComponent.h"

InfoBarComponent::InfoBarComponent (String* messageToDisplay)
:   messageToDisplay (messageToDisplay)
{
    startTimer (500);
}

InfoBarComponent::~InfoBarComponent() {}

void InfoBarComponent::paint (Graphics& g)
{
    const int padding = 3;
    
    Rectangle<int> area = getLocalBounds();
    area.removeFromTop (padding);
    area.removeFromBottom (padding);
    area.removeFromLeft (padding);
    area.removeFromRight (padding);
    
    g.setColour (Colours::white);
    g.fillRect (area);
    
    g.setColour (Colours::black);
    g.drawFittedText ( *(messageToDisplay), area, Justification::centred, 1 );
}

void InfoBarComponent::resized()
{
}

void InfoBarComponent::timerCallback()
{
    repaint ( getLocalBounds() );
}