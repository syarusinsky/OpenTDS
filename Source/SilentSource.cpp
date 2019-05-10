#include "../JuceLibraryCode/JuceHeader.h"
#include "SilentSource.h"

SilentSource::SilentSource() {}
SilentSource::~SilentSource() {}

void SilentSource::prepareToPlay (int samplesPerBlockExpected, double sampleRate) {}
void SilentSource::releaseResources() {}
void SilentSource::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    // clearing the buffer to ensure silent output
    bufferToFill.clearActiveBufferRegion();
}