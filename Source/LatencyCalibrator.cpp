#include "../JuceLibraryCode/JuceHeader.h"
#include "LatencyCalibrator.h"

LatencyCalibrator::LatencyCalibrator (AudioDeviceManager& dm, AudioSourcePlayer* sourcePlayer, AudioSource* source, int* lO, String* mTD)
:   spikeTrainPlaying (true),
    running (false),
    spikeTrainSampleNum (0),
    recordingSampleNum (0),
    spikeIncr (0),
    outputLatency (0),
    player (sourcePlayer),
    newSource (source),
    latencyOffset (lO),
    messageToDisplay (mTD)
{
    AudioIODevice* device = dm.getCurrentAudioDevice();
    outputLatency = device->getOutputLatencyInSamples();
    
    startTimer (100);
    
    String m;
    m << "Calibrating . . . ";
    
    *(messageToDisplay) = m;
}

LatencyCalibrator::~LatencyCalibrator()
{
}

void LatencyCalibrator::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    const ScopedLock sl (lock);
    
    spikeTrain = new AudioSampleBuffer (1, sampleRate);
    recordingBuffer = new AudioSampleBuffer (1, sampleRate * 5);
    recordingBuffer->clear();
    
    generateSpikeTrain();
    
    running = true;
}

void LatencyCalibrator::releaseResources()
{
}

void LatencyCalibrator::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    const ScopedLock sl (lock);
    
    if (running)
    {
        float* recordingPointer = recordingBuffer->getWritePointer (0);
        const int recordingSize = recordingBuffer->getNumSamples();
    
        float* const bufferWritePointer = bufferToFill.buffer->getWritePointer (0);
        const float* const bufferReadPointer = bufferToFill.buffer->getReadPointer (0);
        
        if (spikeTrainPlaying)
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
        } else
        {
            for (int i = 0; i < bufferToFill.numSamples; i++)
            {
                if (recordingSampleNum < recordingSize)
                {
                    recordingPointer[recordingSampleNum] = bufferReadPointer[i];
                    bufferWritePointer[i] = 0;
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

void LatencyCalibrator::timerCallback()
{
    if (running == true && recordingSampleNum >= recordingBuffer->getNumSamples())
    {
        running = false;
        stopTimer();
        
        parseRecording();
    }
}

void LatencyCalibrator::generateSpikeTrain()
{
    const ScopedLock sl (lock);
    
    float* const writePointer = spikeTrain->getWritePointer (0);
    const float* const readPointer = spikeTrain->getReadPointer (0);
    const int size = spikeTrain->getNumSamples();
    
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

void LatencyCalibrator::parseRecording()
{   
    const float* readPointer = recordingBuffer->getReadPointer (0);
    
    const int size = recordingBuffer->getNumSamples();
    const int windowSize = 20;
    
    ScopedPointer<Array<float>> rmsValues = new Array<float>();
    rmsValues->resize (size / windowSize);
    
    for (int i = 0; i < size; i += windowSize)
    {
        float rms = 0;
        
        for (int z = 0; z < windowSize; z++)
        {
            rms += (pow ( readPointer[(i + z)], 2 ));
        }
        
        rms = sqrt ( rms / windowSize );
        
        rmsValues->set ((i / windowSize), rms);
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
    
    /* =============== USED FOR LOGGING THE TRANSIENTS FOR GRAPHING PURPOSES ====================================================
    Logger::setCurrentLogger ( new FileLogger ( File ("C:/Users/Stevey/Pictures/ATII Progress/rmsValues.txt"), "RMS VALUES", 0 ) );
    
    for (int p = 0; p < (size / windowSize); p++)
    {
        String message;
        message << p << newLine;
        message << String (rmsValues->getReference (p)) << newLine;
        Logger::getCurrentLogger()->writeToLog ( message );
    }
    */
    
    /* =============== USED FOR WRITING AUDIO FILE OF RECORDING ====================================================
    File file ("C:/Users/Stevey/Pictures/ATII Progress/SpikeTrain.wav");
    file.deleteFile();
    FileOutputStream fileStream (file);
    if (fileStream.openedOk())
    {
        WavAudioFormat wavFormat;
        ScopedPointer<AudioFormatWriter> writer = wavFormat.createWriterFor (&fileStream, 44100, 1, 16, StringPairArray(), 0);
        writer->writeFromAudioSampleBuffer (*recordingBuffer.get(), 0, size);
        
        delete writer;
    } else
    {
        Logger::getCurrentLogger()->writeToLog ("Filestream not opened ok");
    } =================================================================================
    */
}