#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_graphics/juce_graphics.h>
#include <vector>
#include "proc/WaveformBuffer.h"
#include "util/CoreTimer.h"

namespace invrm
{

class WaveformDisplay 
    : public juce::Component
    , public UpdatableBase
{
public:
    WaveformDisplay(const WaveformBuffer& sourceBuffer)
        : buffer(sourceBuffer)
        , thresholdParameter(nullptr)
    {
        min.resize((size_t)buffer.getNumPoints(), 0.0f);
        max.resize((size_t)buffer.getNumPoints(), 0.0f);
    }

    void timedUpdate() override
    {
        buffer.getData(min, max);
        if(thresholdParameter)
        {
            const float thDB = thresholdParameter->get();
            thresholdLevel = juce::Decibels::decibelsToGain(thDB);
        }
        repaint();
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::black);
        g.setColour(juce::Colours::limegreen);

        const auto bounds = getLocalBounds();
        const int w = bounds.getWidth();
        const int h = bounds.getHeight();
        const int numPoints = static_cast<int>(min.size());
        const float columnWidth = static_cast<float>(w) / static_cast<float>(numPoints);

        for (int i = 0; i < numPoints; ++i)
        {
            const float minY = juce::jmap(min[(size_t)i], -1.0f, 1.0f, (float)h, 0.0f);
            const float maxY = juce::jmap(max[(size_t)i], -1.0f, 1.0f, (float)h, 0.0f);

            const float initX = float(i) * columnWidth;
            const float initY = maxY;
            const float rectW = columnWidth;
            const float rectH = minY - maxY;

            g.fillRect(juce::Rectangle<float>(initX, initY, rectW, rectH));
        }

        if(thresholdParameter)
        {
            juce::Point<float> start = bounds.getBottomLeft().toFloat();
            juce::Point<float> end = bounds.getBottomRight().toFloat();
            
            start.addXY(0.f, -thresholdLevel*(float)h);
            end.addXY(0.f, -thresholdLevel*(float)h);

            juce::Line<float> levelLine(start, end);
            g.setColour(juce::Colours::red);
            g.drawLine(levelLine, 3.f);
        }
    }

    void setThresholdParameter(juce::AudioParameterFloat* param)
    {
        thresholdParameter = param;
    }

    juce::AudioParameterFloat* getThresholdParameter()
    {
        return thresholdParameter;
    }

private:
    const WaveformBuffer& buffer;
    std::vector<float> min, max;
    juce::AudioParameterFloat* thresholdParameter;
    float thresholdLevel = 0.f;
};

}