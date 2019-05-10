#include "../JuceLibraryCode/JuceHeader.h"
#include "LatencyCalibrator.h"

LatencyCalibrator::LatencyCalibrator (AudioDeviceManager& deviceManager, AudioSourcePlayer* sourcePlayer, AudioSource* source, int* latencyOffset, String* messageToDisplay)
:   spikeTrainPlaying (true),
    running (false),
    spikeTrainSampleNum (0),
    recordingSampleNum (0),
    spikeIncr (0),
    outputLatency (0),
    player (sourcePlayer),
    newSource (source),
    latencyOffset (latencyOffset),
    messageToDisplay (messageToDisplay)
{
    AudioIODevice* device = deviceManager.getCurrentAudioDevice();
    outputLatency = device->getOutputLatencyInSamples();
    
    startTimer (100);
    
    String m;
    m << "Calibrating . . . ";
    
    *(messageToDisplay) = m;
}

LatencyCalibrator::~LatencyCalibrator() {}

void LatencyCalibrator::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    const ScopedLock sl (lock);
    
    spikeTrain = std::make_unique<AudioSampleBuffer> (1, sampleRate);
    recordingBuffer = std::make_unique<AudioSampleBuffer> (1, sampleRate * 5); // 5 is chosen arbitrarily, but rountrip latency should never exceed this
    recordingBuffer->clear();
    
    generateSpikeTrain(); // this method fills the spikeTrain
    
    running = true;
}

void LatencyCalibrator::releaseResources() {}

void LatencyCalibrator::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    const ScopedLock sl (lock);
    
    if (running) // running flag set by timerCallback
    {
        float* recordingPointer = recordingBuffer->getWritePointer (0);
        const int recordingSize = recordingBuffer->getNumSamples();
    
        float* const bufferWritePointer = bufferToFill.buffer->getWritePointer (0);
        const float* const bufferReadPointer = bufferToFill.buffer->getReadPointer (0);
        
        if (spikeTrainPlaying) // while there are still samples to output from spikeTrain, write those samples to the buffer and record input
        {
            const int size = spikeTrain->getNumSamples();
            const float* const spikeTrainReadPointer = spikeTrain->getReadPointer (0);
        
            for (int i = 0; i < bufferToFill.numSamples; i++)
            {
                if (spikeTrainSampleNum <= size)
                {
                    recordingPointer[recordingSampleNum] = bufferReadPointer[i];
                    bufferWritePointer[i] = spikeTrainReadPointer[spikeTrainSampleNum];
                    spikeTrainSampleNum++;
                    recordingSampleNum++;
                } else 
                {
                    spikeTrainPlaying = false;
                    recordingPointer[recordingSampleNum] = bufferReadPointer[i];
                    bufferWritePointer[i] = 0;
                    recordingSampleNum++;
                }
            }
        } else // if there are no more samples to output from spikeTrain, record input until recording buffer is full
        {
            for (int i = 0; i < bufferToFill.numSamples; i++)
            {
                if (recordingSampleNum < recordingSize)
                {
                    recordingPointer[recordingSampleNum] = bufferReadPointer[i];
                    bufferWritePointer[i] = -7;
                    recordingSampleNum++;
                } else
                {
                    bufferWritePointer[i] = 0;
                }
            }
        }
    } else
    {
        bufferToFill.clearActiveBufferRegion();
    }
}

// sets running flag to false if recording is complete
void LatencyCalibrator::timerCallback()
{
    if (running == true && recordingSampleNum >= recordingBuffer->getNumSamples())
    {
        running = false;
        stopTimer();
        
        parseRecording();
    }
}

// fills the spikeTrain AudioBuffer
void LatencyCalibrator::generateSpikeTrain()
{
    const ScopedLock sl (lock);
    
    float* const writePointer = spikeTrain->getWritePointer (0);
    const float* const readPointer = spikeTrain->getReadPointer (0);
    const int size = spikeTrain->getNumSamples();
    
    // fill spikeTrain with 10 spikes
    spikeIncr = (size / 10);
    int spikePoints[10] = { spikeIncr, spikeIncr * 2, spikeIncr * 3, spikeIncr * 4, spikeIncr * 5, spikeIncr * 6, spikeIncr * 7, spikeIncr * 8, spikeIncr * 9, spikeIncr * 10 };
    
    for (int i = 0; i < size; i++)
    {
        bool exists = std::find( std::begin (spikePoints), std::end (spikePoints), i ) != std::end(spikePoints);
        
        if (exists)
        {
            writePointer[i] = 0.99f;
        } else
        {
            writePointer[i] = 0;
        }
    }
}

// this method parses the recording looking for spikes, compares them with the original recording, and sets the latency offset
void LatencyCalibrator::parseRecording()
{   
    const float* readPointer = recordingBuffer->getReadPointer (0);
    
    const int size = recordingBuffer->getNumSamples();
    const int windowSize = 20; // we pass through the buffer by "windows" that allow us to get the RMS values
    
    ScopedPointer<Array<float>> rmsValues = new Array<float>();
    rmsValues->resize (size / windowSize);
    
    for (int i = 0; i < size; i += windowSize) // incrementing by windowSize
    {
        float rms = 0;
        
        for (int z = 0; z < windowSize; z++)
        {
            rms += (pow ( readPointer[(i + z)], 2 ));
        }
        
        rms = sqrt ( rms / windowSize );
        
        rmsValues->set ((i / windowSize), rms); // store the RMS values
    }
    
    const float threshold = 10.0;
    const int wait = 5;
    int iter = wait;
    int numSpikes = 0;
    float latency = 0.0;
    
    const float noiseFloor = ( rmsValues->getReference (0) + rmsValues->getReference (1) ) / 2;
    
    for (int p = 2; p < (size / windowSize); p++)
    {
        float rmsValue = rmsValues->getReference (p);
        
        if ( rmsValue > (noiseFloor * threshold) && iter >= wait )
        {
            int sample = ( (p * windowSize) - spikeIncr );
            
            if (numSpikes == 0)
            {
                latency = (sample);
            } else
            {
                latency += ( sample - (spikeIncr * numSpikes) );
            }
            
            numSpikes++;
            iter = 0;
        }
        
        iter++;
    }
    
    if (numSpikes == 9)
    {
        int totalLatency = ( round (latency) / numSpikes );
        
        *(latencyOffset) = totalLatency;
        
        String m;
        
        m << "Calibration Successful... ";
        m << "Total Latency: " << *(latencyOffset);
        
        *(messageToDisplay) = m;
        
    } else
    {
        String m;
        m << "Calibration Unsuccessful, please retry...";
        
        *(messageToDisplay) = m;
    }
    
    player->setSource (newSource);
}