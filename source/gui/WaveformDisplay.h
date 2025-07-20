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
    {
        min.resize(buffer.getNumPoints(), 0.0f);
        max.resize(buffer.getNumPoints(), 0.0f);
    }

    void timedUpdate() override
    {
        buffer.getData(min, max);
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
            const float minY = juce::jmap(min[i], -1.0f, 1.0f, (float)h, 0.0f);
            const float maxY = juce::jmap(max[i], -1.0f, 1.0f, (float)h, 0.0f);

            const float initX = i * columnWidth;
            const float initY = maxY;
            const float rectW = columnWidth;
            const float rectH = minY - maxY;

            g.fillRect(juce::Rectangle<float>(initX, initY, rectW, rectH));
        }
    }

private:
    const WaveformBuffer& buffer;
    std::vector<float> min, max;
};

}