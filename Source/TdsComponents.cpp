#include "../JuceLibraryCode/JuceHeader.h"
#include "TdsComponents.h"

TdsComponents::TdsComponents (int* lC, AudioDeviceManager* deviceManager, AudioSourcePlayer* sourcePlayer, AudioSource* source, String* messageToDisplay)
:   startF (0),
    endF (0),
    bandW (0),
    latencyComp (lC),
    messageToDisplay (messageToDisplay),
    sweepT (0.0),
    delay (0.0),
    deviceManager (deviceManager),
    player (sourcePlayer),
    audioSource (source)
{   
    startFreq = std::make_unique<TextEditor> ("startFreq");
    addAndMakeVisible (startFreq.get());
    startFreq->setMultiLine (false);
    startFreq->setReturnKeyStartsNewLine (false);
    startFreq->setReadOnly (false);
    startFreq->setScrollbarsShown (false);
    startFreq->setCaretVisible (true);
    startFreq->setPopupMenuEnabled (true);
    startFreq->setText ( String ("start freq") );

    endFreq = std::make_unique<TextEditor> ("endFreq");
    addAndMakeVisible (endFreq.get());
    endFreq->setMultiLine (false);
    endFreq->setReturnKeyStartsNewLine (false);
    endFreq->setReadOnly (false);
    endFreq->setScrollbarsShown (false);
    endFreq->setCaretVisible (true);
    endFreq->setPopupMenuEnabled (true);
    endFreq->setText ( String ("end Freq") );

    sweepTime = std::make_unique<TextEditor> ("sweepTime");
    addAndMakeVisible (sweepTime.get());
    sweepTime->setMultiLine (false);
    sweepTime->setReturnKeyStartsNewLine (false);
    sweepTime->setReadOnly (false);
    sweepTime->setScrollbarsShown (false);
    sweepTime->setCaretVisible (true);
    sweepTime->setPopupMenuEnabled (true);
    sweepTime->setText ( String ("time (seconds)") );

    bandwidth = std::make_unique<TextEditor> ("bandwidth");
    addAndMakeVisible (bandwidth.get());
    bandwidth->setMultiLine (false);
    bandwidth->setReturnKeyStartsNewLine (false);
    bandwidth->setReadOnly (false);
    bandwidth->setScrollbarsShown (false);
    bandwidth->setCaretVisible (true);
    bandwidth->setPopupMenuEnabled (true);
    bandwidth->setText ( String ("bandwidth") );

    timeOfFlight = std::make_unique<TextEditor> ("timeOfFlight");
    addAndMakeVisible (timeOfFlight.get());
    timeOfFlight->setMultiLine (false);
    timeOfFlight->setReturnKeyStartsNewLine (false);
    timeOfFlight->setReadOnly (false);
    timeOfFlight->setScrollbarsShown (false);
    timeOfFlight->setCaretVisible (true);
    timeOfFlight->setPopupMenuEnabled (true);
    timeOfFlight->setText ( String ("delay (ms)") );

    temperature = std::make_unique<TextEditor> ("temperature");
    addAndMakeVisible (temperature.get());
    temperature->setMultiLine (false);
    temperature->setReturnKeyStartsNewLine (false);
    temperature->setReadOnly (false);
    temperature->setScrollbarsShown (false);
    temperature->setCaretVisible (true);
    temperature->setPopupMenuEnabled (true);
    temperature->setText ( String ("temp (F)") );

    distance = std::make_unique<TextEditor> ("distance");
    addAndMakeVisible (distance.get());
    distance->setMultiLine (false);
    distance->setReturnKeyStartsNewLine (false);
    distance->setReadOnly (false);
    distance->setScrollbarsShown (false);
    distance->setCaretVisible (true);
    distance->setPopupMenuEnabled (true);
    distance->setText ( String ("distance (ft)") );
    
    sweepButton = std::make_unique<TextButton> ("sweepButton");
    addAndMakeVisible (sweepButton.get());
    sweepButton->setButtonText ( String ("sweep") );
    sweepButton->addListener (this);
    
    graphBins = std::make_unique<Array<float>> ();
    graphBins->insertMultiple ( 0, 0, 100 );
    
    frequencyGraphComponent = std::make_unique<FrequencyGraphComponent> (graphBins.get(), &graphFlag);
    addAndMakeVisible (frequencyGraphComponent.get());
    
    graphFlag.setValue (false);
}

TdsComponents::~TdsComponents() {}

void TdsComponents::paint (Graphics& g)
{
    Rectangle<int> area = getLocalBounds();
    
    const int controlsBarHeight = 50;
    
    Rectangle<int> controlsArea = area.removeFromBottom (controlsBarHeight);
    
    controlsArea.removeFromTop ( round (controlsArea.getHeight() / 2) );
    
    Rectangle<int> control = controlsArea;
    const int controlWidth = control.proportionOfWidth (0.12);
    const int padding = control.proportionOfWidth (0.005);
    
    startFreq->setBounds ( control.removeFromLeft (controlWidth) );
    control.removeFromLeft (padding);
    endFreq->setBounds ( control.removeFromLeft (controlWidth) );
    control.removeFromLeft (padding);
    sweepTime->setBounds ( control.removeFromLeft (controlWidth) );
    control.removeFromLeft (padding);
    bandwidth->setBounds ( control.removeFromLeft (controlWidth) );
    control.removeFromLeft (padding);
    timeOfFlight->setBounds ( control.removeFromLeft (controlWidth) );
    control.removeFromLeft (padding);
    temperature->setBounds ( control.removeFromLeft (controlWidth) );
    control.removeFromLeft (padding);
    distance->setBounds ( control.removeFromLeft (controlWidth) );
    control.removeFromLeft (padding);
    sweepButton->setBounds ( control.removeFromLeft (controlWidth) );
    
    const int graphPadding = 30;
    
    area.removeFromLeft (graphPadding);
    area.removeFromRight (graphPadding);
    
    frequencyGraphComponent->setBounds (area);
}

void TdsComponents::resized()
{
}

void TdsComponents::buttonClicked (Button* button)
{
    if (button == sweepButton.get())
    {
        const int sF = startFreq->getText().getIntValue();
        const int eF = endFreq->getText().getIntValue();
        const float sT = sweepTime->getText().getFloatValue();
        const int bW = bandwidth->getText().getIntValue();
        const int tmp = ( (temperature->getText().getIntValue() - 32) * (5/9) );
        const float dst = distance->getText().getFloatValue();
        const float del = timeOfFlight->getText().getFloatValue();

        // if settings are valid, start a TDS sweep
        if (sF >= 20 && sF <= 20000 && eF >= 20 && eF <= 20000 && sT >= 0.01 && sT <= 10.0 && bW >= 20.0 && bW <= 1500.0)
        {
            startF = sF;
            endF = eF;
            sweepT = sT * 1000.0;
            bandW = bW;
            
            AudioIODevice* device = deviceManager->getCurrentAudioDevice();
            const int sampleRate = device->getCurrentSampleRate();
            const int blockSize = device->getCurrentBufferSizeSamples();
            
            const float latencyInSamples = *(latencyComp);
            
            const float latencyOffset = ( latencyInSamples / sampleRate );

            if (!del && !dst && !tmp)
            {
                delay = 0.0 + latencyOffset;
            } else if (!del && dst >= 0.0 && tmp <= 100)
            {
                const float c = 1085.96 + (0.6 * tmp);
                delay = (dst / c) + latencyOffset;
            } else if (del >= 0.0 && !dst && !tmp)
            {
                delay = (del * 0.001) + latencyOffset;
            } else {
                delay = 0.0 + latencyOffset;
            }
            
            if (player->getCurrentSource() == audioSource)
            {   
                if (blockSize == 128 || blockSize == 256 || blockSize == 512 || blockSize == 1024 || blockSize == 2048)
                {
                    tdsSweep = std::make_unique<TdsSweep> (startF, endF, sweepT, bandW, delay, blockSize, sampleRate, player, audioSource, messageToDisplay, graphBins.get(), &graphFlag);
                    player->setSource (tdsSweep.get());
                } else
                {
                    *(messageToDisplay) = String ("This is Not a Valid Buffer Size, Use 2^n");
                }
            } else
            {
                *(messageToDisplay) = String ("Please Wait for Sweep or Calibration to Complete...");
            }
        }
    }
}