#pragma once

#include "util/Parameters.h"

#include "juce_audio_basics/juce_audio_basics.h"

class PluginProcessor;

namespace invrm
{

class Modulator
{
public:
    Modulator(PluginProcessor* p);
    ~Modulator();

    void processBlock (const float* mainIn,
                       float* mainOut,
                       const float* side,
                       int length);

    void prepareToPlay(int maxBlockSize);

    static void setPregain(float);
    static void setThreshold(float);
    const juce::AudioBuffer<float>& getFactorArray() const { return mFactorArray; }

private:
    PluginProcessor* mProcessor;

    juce::AudioBuffer<float> mFactorArray;

};

} // namespace invrm
