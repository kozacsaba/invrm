#pragma once

#include "juce_audio_basics/juce_audio_basics.h"
#include "proc/PluginProcessor.h"

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
    std::vector<RAP*>& mParams;

    juce::AudioBuffer<float> mFactorArray;

};

} // namespace invrm
