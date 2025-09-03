#include "Modulator.h"
#include "proc/PluginProcessor.h"

#define UNPACK_PARAM(paramName)                                                 \
    const int paramName##PID = static_cast<int>(param::PID::paramName);         \
    const auto* paramName##Param = mProcessor->getParameters()[paramName##PID]; \
    const float paramName##Norm = paramName##Param->get();                      \
    const float paramName##Val = paramName##Param                               \
        ->getNormalisableRange()                                                \
        .convertFrom0to1(paramName##Norm);                                      \
    do {} while(false)

#define UNPACK_DECIBEL(paramName)                                               \
    const int paramName##PID = static_cast<int>(param::PID::paramName);         \
    const auto* paramName##Param = mProcessor->getParameters()[paramName##PID]; \
    const float paramName##Norm = paramName##Param->get();                      \
    const float paramName##DB = paramName##Param                                \
        ->getNormalisableRange()                                                \
        .convertFrom0to1(paramName##Norm);                                      \
    const float paramName##Val = juce::Decibels::decibelsToGain(paramName##DB); \
    do {} while(false)

using namespace invrm;

Modulator::Modulator(PluginProcessor* p)
    : mProcessor(p)
{}
Modulator::~Modulator() = default;

void Modulator::processBlock (const float* mainIn,
                              float* mainOut,
                              const float* side,
                              int length)
{
    //UNPACK_DECIBEL(PreGain);
    const int PreGainPID = static_cast<int>(param::PID::PreGain);
    const float PreGainDB = mProcessor->getParameters()[PreGainPID]->get();
    const float PreGainVal = juce::Decibels::decibelsToGain(PreGainDB);

    // UNPACK_DECIBEL(Threshold);
    const int ThresholdPID = static_cast<int>(param::PID::Threshold);
    const float ThresholdDB = mProcessor->getParameters()[ThresholdPID]->get();
    const float ThresholdVal = juce::Decibels::decibelsToGain(ThresholdDB);
    
    //UNPACK_PARAM(WetMix);
    const int WetMixPID = static_cast<int>(param::PID::WetMix);
    const auto* WetMixParam = mProcessor->getParameters()[WetMixPID];
    const float WetMixVal = WetMixParam->get();

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