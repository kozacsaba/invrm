#pragma once

#include "juce_audio_basics/juce_audio_basics.h"

namespace invrm
{

class Modulator
{
public:
    Modulator();
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
    inline static float mPregain = 1.f;
    inline static float mThreshold = 0.f;
    inline static float mWetMix = 1.f;
    juce::AudioBuffer<float> mFactorArray;

};

} // namespace invrm
