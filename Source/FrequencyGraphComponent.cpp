#include "../JuceLibraryCode/JuceHeader.h"
#include "FrequencyGraphComponent.h"

FrequencyGraphComponent::FrequencyGraphComponent (Array<float>* gBins, Value* gF)
:   graphBins (gBins),
    graphFlag (gF)
{
    graphFlag->addListener (this);
}

FrequencyGraphComponent::~FrequencyGraphComponent()
{
}

void FrequencyGraphComponent::paint (Graphics& g)
{
    g.fillAll (Colours::white);
    
    Path response;
    
    const Rectangle<int> freqGraph = getLocalBounds();
    
    const Range<int> graphHorizontalRange = freqGraph.getHorizontalRange();
    const Range<int> graphVerticalRange = freqGraph.getVerticalRange();
    
    const float floatRef = 1.0;

    double xPointsToPlot[11] = { 20.0, 50.0, 100.0, 200.0, 300.0, 500.0, 1000.0, 5000.0, 10000.0, 15000.0, 20000.0 };
    double yPointsToPlot[8] = { pow (10.0, -50.0/20.0) * floatRef, pow (10.0, -40.0/20.0) * floatRef, pow (10.0, -30.0/20.0) * floatRef, pow (10.0, -20.0/20.0) * floatRef, pow (10.0, -10.0/20.0) * floatRef, pow (10.0, -6.0/20.0) * floatRef, pow (10.0, -3.0/20.0) * floatRef, floatRef };

    const float dL[2] = {4.0, 2.0};

    for (int i = 0; i < ( sizeof(xPointsToPlot) / sizeof(*xPointsToPlot) ); i++)
    {
        const double distanceAlongXAxis = ( log (xPointsToPlot[i]) - log (20.0) ) / ( log (20000.0) - log (20.0) );
        const int pixelX = (int) ( distanceAlongXAxis * (graphHorizontalRange.getEnd() - graphHorizontalRange.getStart()) + graphHorizontalRange.getStart() );
        g.setColour (Colours::darkslategrey);
        const Line<float> line ( pixelX, graphVerticalRange.getStart(), pixelX, graphVerticalRange.getEnd() );
        g.drawDashedLine ( line, dL, 2, 1.0, 0 );
        g.setColour (Colours::black);
        g.drawSingleLineText ( String (std::round (xPointsToPlot[i])), pixelX, graphVerticalRange.getEnd() - 5, Justification::left );
    }

    for (int i = 0; i < ( sizeof(yPointsToPlot) / sizeof(*yPointsToPlot) ); i++)
    {
        const double distanceAlongYAxis = ( log10 (yPointsToPlot[i]) - log10 (floatRef) ) / (log10 ( pow (10.0, -50.0/20.0) * floatRef) - log10 (floatRef) );
        const int pixelY = (int) ( distanceAlongYAxis * (graphVerticalRange.getEnd() - graphVerticalRange.getStart()) + graphVerticalRange.getStart() );
        g.setColour (Colours::darkslategrey);
        const Line<float> line (graphHorizontalRange.getStart(), pixelY, graphHorizontalRange.getEnd(), pixelY);
        g.drawDashedLine (line, dL, 2, 1.0, 0);
        g.setColour (Colours::black);
        g.drawSingleLineText ( String (20 * log10 (yPointsToPlot[i]/floatRef)), graphHorizontalRange.getStart() + 10, pixelY, Justification::left );
    }
    
    response.startNewSubPath (graphHorizontalRange.getStart(), graphVerticalRange.getEnd());

    for (int i = 0; i < ( graphBins->size() / 2 ); i++)
    {
        const double freq = (*graphBins) [(i * 2)];
        const double magnitude = (*graphBins) [(i * 2) + 1];

        if (freq > 0.0 && freq < 20000.0)
        {
            const double distanceAlongXAxis = (log (freq) - log (20.0f)) / (log (20000.0f) - log (20.0f));
            int pixelX = (int) (distanceAlongXAxis * (graphHorizontalRange.getEnd() - graphHorizontalRange.getStart()) + graphHorizontalRange.getStart());
            const double distanceAlongYAxis = (log10 (magnitude) - log10 (floatRef)) / (log10 (pow (10.0, -50.0/20.0) * floatRef) - log10 (floatRef));
            int pixelY = (int) (distanceAlongYAxis * (graphVerticalRange.getEnd() - graphVerticalRange.getStart()) + graphVerticalRange.getStart());

            if (pixelY < graphVerticalRange.getStart())
            {
                pixelY = graphVerticalRange.getStart();
            } else if (pixelY > graphVerticalRange.getEnd())
            {
                pixelY = graphVerticalRange.getEnd();
            }

            response.lineTo (pixelX, pixelY);
        }
    }

    g.setColour (Colours::black);
    g.strokePath (response, PathStrokeType (1.0f));
}

void FrequencyGraphComponent::resized()
{
}

void FrequencyGraphComponent::valueChanged (Value& gF)
{
    graphFlag->setValue (false);
    
    repaint ( getLocalBounds() );
}