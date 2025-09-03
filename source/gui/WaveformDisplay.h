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
    WaveformDisplay(const WaveformBuffer& sourceBuffer, 
                    const WaveformBuffer& inputBuffer)
        : buffer(sourceBuffer)
        , input(inputBuffer)
        , thresholdParameter(nullptr)
    {
        min.resize((size_t)buffer.getNumPoints(), 0.0f);
        max.resize((size_t)buffer.getNumPoints(), 0.0f);
        inmin.resize((size_t)input.getNumPoints(), 0.f);
        inmax.resize((size_t)input.getNumPoints(), 0.f);
    }

    void timedUpdate() override
    {
        buffer.getData(min, max);
        input.getData(inmin, inmax);
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

        const auto bounds = getLocalBounds();
        const int w = bounds.getWidth();
        const int h = bounds.getHeight();
        const int numPoints = static_cast<int>(min.size());
        const float columnWidth = static_cast<float>(w) / static_cast<float>(numPoints);
        
        // (Sidechain) Input Waveform
        juce::Path inputWavePath;
        inputWavePath.startNewSubPath(bounds.getBottomLeft().toFloat());
        for (int i = 0; i < numPoints; ++i)
        {
            const float current_min_y = std::abs(inmin[(size_t)i]);
            const float current_max_y = std::abs(inmax[(size_t)i]);
            const float Y = std::max(current_min_y, current_max_y);
            const float val = juce::jmap(Y, 0.f, 1.f, (float)h, 0.f);
            
            const float step = float(i) * columnWidth;

            inputWavePath.lineTo(step, val);
        }
        inputWavePath.lineTo(bounds.getBottomRight().toFloat());
        inputWavePath.closeSubPath();
        g.setColour(juce::Colours::grey);
        g.fillPath(inputWavePath);

        // Gain factor
        g.setColour(juce::Colours::limegreen);
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

        // Treshold Level
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
    const WaveformBuffer& input;
    std::vector<float> min, max, inmin, inmax;
    juce::AudioParameterFloat* thresholdParameter;
    float thresholdLevel = 0.f;
};

}