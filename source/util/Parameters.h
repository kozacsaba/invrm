#pragma once

#include "juce_audio_processors/juce_audio_processors.h"

using APVTS = juce::AudioProcessorValueTreeState;
using APF = juce::AudioParameterFloat;
using UniqueApfVector = std::vector<std::unique_ptr<APF>>;

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
        case PID::NumParams : ;
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

inline void createParam(UniqueApfVector& vec, 
                        PID pid,
                        juce::NormalisableRange<float> range,
                        float defaultValue)
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
        id, name, range,
        defaultValue,
        "db",
        juce::AudioProcessorParameter::Category::genericParameter,
        valToStr,
        strToVal
    ));
}

inline APVTS::ParameterLayout CreateParameterLayout()
{
    UniqueApfVector params;

    createParam(params, PID::PreGain, {-30.f, 30.f}, 0.f);
    createParam(params, PID::Threshold, {-80.f, 0.f}, 0.f);
    createParam(params, PID::WetMix, {0.f, 1.f}, 1.f);

    return {params.begin(), params.end()};
}

} // namspace invrm::param