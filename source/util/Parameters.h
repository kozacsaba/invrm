#pragma once

#include "juce_audio_processors/juce_audio_processors.h"

using APVTS = juce::AudioProcessorValueTreeState;
using RAP = juce::RangedAudioParameter;
using UniqueRapVector = std::vector<std::unique_ptr<RAP>>;

namespace invrm::param
{

enum class PID
{
    PreGain,
    Threshold,
    WetMix,

    NumParams
};
static constexpr int numParams = static_cast<int>(PID::NumParams);

inline juce::String toName(PID pid)
{
    switch (pid)
    {
        case PID::PreGain : return "PreGain";
        case PID::Threshold : return "Threshold";
        case PID::WetMix : return "Dry/Wet Mix";
        default : return "error";
    }
}

inline juce::String toId(juce::String name)
{
    return name.toLowerCase().removeCharacters(" /");
}

inline juce::String toId(PID pid)
{
    return toId(toName(pid));
}

inline void createParam(UniqueRapVector& vec, PID pid)
{
    const auto valToStr = [](float val, int maxLen)
    {
        return juce::String(val, maxLen);
    };

    const auto strToVal = [](juce::String str)
    {
        return str.getFloatValue();
    };

    const auto name = toName(pid);
    const auto id = toId(pid);

    vec.push_back(std::make_unique<juce::AudioParameterFloat>
    (
        id, name, 
        juce::Range<float>(-100.f, 100.f),
        0.f,
        "db",
        juce::AudioProcessorParameter::Category::genericParameter,
        valToStr,
        strToVal
    ));
}

inline APVTS::ParameterLayout CreateParameterLayout()
{
    UniqueRapVector params;

    createParam(params, PID::PreGain);
    createParam(params, PID::Threshold);
    createParam(params, PID::WetMix);

    return {params.begin(), params.end()};
}

} // namspace invrm::param