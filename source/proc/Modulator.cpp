#include "Modulator.h"
#include "proc/PluginProcessor.h"

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
    const int PreGainPID = static_cast<int>(param::PID::PreGain);
    const float PreGainDB = mProcessor->getParameters()[PreGainPID]->get();
    const float PreGainVal = juce::Decibels::decibelsToGain(PreGainDB);

    const int ThresholdPID = static_cast<int>(param::PID::Threshold);
    const float ThresholdDB = mProcessor->getParameters()[ThresholdPID]->get();
    const float ThresholdVal = juce::Decibels::decibelsToGain(ThresholdDB);
    
    const int WetMixPID = static_cast<int>(param::PID::WetMix);
    const float WetMixVal = mProcessor->getParameters()[WetMixPID]->get();

    for(int s = 0; s < length; s++)
    {
        float factor = std::abs(side[s]);
        factor *= PreGainVal;
        mInputArray.setSample(0, s, factor); // processed input

        factor = std::max(0.f, factor - ThresholdVal);
        factor = std::max(0.f, 1.f - factor);
        mFactorArray.setSample(0, s, factor);

        mainOut[s] = (WetMixVal * factor + (1-WetMixVal)) * mainIn[s];
    }
}

void Modulator::prepareToPlay(int maxBlockSize)
{
    mFactorArray.setSize(1, maxBlockSize);
    mFactorArray.clear();

    mInputArray.setSize(1, maxBlockSize);
    mInputArray.clear();
}
