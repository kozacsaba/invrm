#include "Modulator.h"

#define UNPACK_PARAM(paramName)                                                 \
    const int paramName##PID = static_cast<int>(param::PID::paramName);         \
    const float paramName##Norm = mParams[paramName##PID]->getValue();          \
    const float paramName##Val = mParams[paramName##PID]                        \
        ->getNormalisableRange()                                                \
        .convertFrom0to1(paramName##Norm);                                      \
    do {} while(false)

#define UNPACK_DECIBEL(paramName)                                               \
    const int paramName##PID = static_cast<int>(param::PID::paramName);         \
    const float paramName##Norm = mParams[paramName##PID]->getValue();          \
    const float paramName##DB = mParams[paramName##PID]                         \
        ->getNormalisableRange()                                                \
        .convertFrom0to1(paramName##Norm);                                      \
    const float paramName##Val = juce::Decibels::decibelsToGain(paramName##DB); \
    do {} while(false)

using namespace invrm;

Modulator::Modulator(PluginProcessor* p)
    : mProcessor(p)
    , mParams(mProcessor->getParameters())
{}
Modulator::~Modulator() = default;

void Modulator::processBlock (const float* mainIn,
                              float* mainOut,
                              const float* side,
                              int length)
{

    UNPACK_DECIBEL(PreGain);
    UNPACK_DECIBEL(Threshold);
    UNPACK_PARAM(WetMix);

    for(int s = 0; s < length; s++)
    {
        float factor = std::abs(side[s]);
        factor *= PreGainVal;
        factor = std::max(0.f, factor - ThresholdVal);
        factor = std::max(0.f, 1.f - factor);
        mFactorArray.setSample(0, s, factor);

        mainOut[s] = (WetMixVal * factor + (1-WetMixVal)) * mainIn[s];
    }
}

void Modulator::prepareToPlay(int maxBlockSize)
{
    mFactorArray.setSize(1, maxBlockSize);
}

#undef UNPACK_PARAM
#undef UNPACK_DECIBEL