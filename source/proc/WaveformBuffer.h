#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

namespace invrm
{

class WaveformBuffer
{
public:
    WaveformBuffer()
        : minValues((size_t)numPoints, 0.0f)
        , maxValues((size_t)numPoints, 0.0f)
        , writeIndex(0)
    {
    }

    void prepareToPlay(double sRate)
    {
        sampleRate = sRate;
        samplesPerPoint = static_cast<int>((sampleRate * displayDurationMs * 0.001) / numPoints);
        jassert(samplesPerPoint > 0);
        reset();
    }
    
    void reset()
    {
        std::fill(minValues.begin(), minValues.end(), 0.0f);
        std::fill(maxValues.begin(), maxValues.end(), 0.0f);
        writeIndex = 0;
        sampleCounter = 0;
        currentMin = std::numeric_limits<float>::max();
        currentMax = std::numeric_limits<float>::lowest();
    }

    void pushBuffer(const juce::AudioBuffer<float>& buffer, int usedLength)
    {
        // usedLength is needed only because some DAWs do not work with constant sized buffers.
        // Each process block could potentionally be called on different sized buffers, and thus
        // we cannot rely on the maximum size (which the modulator class gives us)

        const int numSamples = usedLength;
        const float* channelData = buffer.getReadPointer(0);

        for (int i = 0; i < numSamples; ++i)
        {
            float sample = channelData[i];
            currentMin = std::min(currentMin, sample);
            currentMax = std::max(currentMax, sample);

            if (++sampleCounter >= samplesPerPoint)
            {
                minValues[(size_t)writeIndex] = currentMin;
                maxValues[(size_t)writeIndex] = currentMax;

                writeIndex = (writeIndex + 1) % numPoints;
                sampleCounter = 0;
                currentMin = std::numeric_limits<float>::max();
                currentMax = std::numeric_limits<float>::lowest();
            }
        }
    }

    void getData(std::vector<float>& outMin, std::vector<float>& outMax) const
    {
        for (size_t i = 0; i < (size_t)numPoints; ++i)
        {
            const size_t index = ((size_t)writeIndex + i) % (size_t)numPoints;
            outMin[i] = minValues[index];
            outMax[i] = maxValues[index];
        }
    }

    int getNumPoints() const { return numPoints; }

    void setDisplayDuration(double inMs)
    {
        displayDurationMs = (float)inMs;
        samplesPerPoint = static_cast<int>((sampleRate * displayDurationMs * 0.001) / numPoints);
    }

private:
    const int numPoints = 100;
    double sampleRate;
    float displayDurationMs = 500.f;
    int samplesPerPoint;

    std::vector<float> minValues;
    std::vector<float> maxValues;

    int writeIndex;
    int sampleCounter = 0;
    float currentMin = std::numeric_limits<float>::max();
    float currentMax = std::numeric_limits<float>::lowest();
};

} // namespace invrm