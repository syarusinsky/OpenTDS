#include "../JuceLibraryCode/JuceHeader.h"
#include "tdsComponents.h"

TdsComponents::TdsComponents (int* lC, AudioDeviceManager* dm, AudioSourcePlayer* sourcePlayer, AudioSource* source, String* mTD)
:   startF (0),
    endF (0),
    bandW (0),
    latencyComp (lC),
    messageToDisplay (mTD),
    sweepT (0.0),
    delay (0.0),
    deviceManager (dm),
    player (sourcePlayer),
    audioSource (source)
{   
    addAndMakeVisible ( startFreq = new TextEditor ("startFreq") );
    startFreq->setMultiLine (false);
    startFreq->setReturnKeyStartsNewLine (false);
    startFreq->setReadOnly (false);
    startFreq->setScrollbarsShown (false);
    startFreq->setCaretVisible (true);
    startFreq->setPopupMenuEnabled (true);
    startFreq->setText ( String ("start freq") );

    addAndMakeVisible ( endFreq = new TextEditor ("endFreq") );
    endFreq->setMultiLine (false);
    endFreq->setReturnKeyStartsNewLine (false);
    endFreq->setReadOnly (false);
    endFreq->setScrollbarsShown (false);
    endFreq->setCaretVisible (true);
    endFreq->setPopupMenuEnabled (true);
    endFreq->setText ( String ("end Freq") );

    addAndMakeVisible ( sweepTime = new TextEditor ("sweepTime") );
    sweepTime->setMultiLine (false);
    sweepTime->setReturnKeyStartsNewLine (false);
    sweepTime->setReadOnly (false);
    sweepTime->setScrollbarsShown (false);
    sweepTime->setCaretVisible (true);
    sweepTime->setPopupMenuEnabled (true);
    sweepTime->setText ( String ("time (seconds)") );

    addAndMakeVisible ( bandwidth = new TextEditor ("bandwidth") );
    bandwidth->setMultiLine (false);
    bandwidth->setReturnKeyStartsNewLine (false);
    bandwidth->setReadOnly (false);
    bandwidth->setScrollbarsShown (false);
    bandwidth->setCaretVisible (true);
    bandwidth->setPopupMenuEnabled (true);
    bandwidth->setText ( String ("bandwidth") );

    addAndMakeVisible ( timeOfFlight = new TextEditor ("timeOfFlight") );
    timeOfFlight->setMultiLine (false);
    timeOfFlight->setReturnKeyStartsNewLine (false);
    timeOfFlight->setReadOnly (false);
    timeOfFlight->setScrollbarsShown (false);
    timeOfFlight->setCaretVisible (true);
    timeOfFlight->setPopupMenuEnabled (true);
    timeOfFlight->setText ( String ("delay (ms)") );

    addAndMakeVisible ( temperature = new TextEditor ("temperature") );
    temperature->setMultiLine (false);
    temperature->setReturnKeyStartsNewLine (false);
    temperature->setReadOnly (false);
    temperature->setScrollbarsShown (false);
    temperature->setCaretVisible (true);
    temperature->setPopupMenuEnabled (true);
    temperature->setText ( String ("temp (F)") );

    addAndMakeVisible ( distance = new TextEditor ("distance") );
    distance->setMultiLine (false);
    distance->setReturnKeyStartsNewLine (false);
    distance->setReadOnly (false);
    distance->setScrollbarsShown (false);
    distance->setCaretVisible (true);
    distance->setPopupMenuEnabled (true);
    distance->setText ( String ("distance (ft)") );
    
    addAndMakeVisible ( sweepButton = new TextButton ("sweepButton") );
    sweepButton->setButtonText ( String ("sweep") );
    sweepButton->addListener (this);
    
    graphBins = new Array<float> ();
    graphBins->insertMultiple ( 0, 0, 100 );
    
    addAndMakeVisible ( frequencyGraphComponent = new FrequencyGraphComponent (graphBins.get(), &graphFlag) );
    
    graphFlag.setValue (false);
}

TdsComponents::~TdsComponents()
{
    startFreq = nullptr;
    endFreq = nullptr;
    sweepTime = nullptr;
    bandwidth = nullptr;
    timeOfFlight = nullptr;
    temperature = nullptr;
    distance = nullptr;
    sweepButton = nullptr;
    
    frequencyGraphComponent = nullptr;
}

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
    if (button == sweepButton)
    {
        const int sF = startFreq->getText().getIntValue();
        const int eF = endFreq->getText().getIntValue();
        const float sT = sweepTime->getText().getFloatValue();
        const int bW = bandwidth->getText().getIntValue();
        const int tmp = ( (temperature->getText().getIntValue() - 32) * (5/9) );
        const float dst = distance->getText().getFloatValue();
        const float del = timeOfFlight->getText().getFloatValue();

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
                    tdsSweep = new TdsSweep (startF, endF, sweepT, bandW, delay, blockSize, sampleRate, player, audioSource, messageToDisplay, graphBins.get(), &graphFlag);
                    player->setSource (tdsSweep);
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