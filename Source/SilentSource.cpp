#include "../JuceLibraryCode/JuceHeader.h"
#include "SilentSource.h"

SilentSource::SilentSource ()
{
}

SilentSource::~SilentSource()
{
}

void SilentSource::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
}

void SilentSource::releaseResources()
{
}

void SilentSource::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();
}