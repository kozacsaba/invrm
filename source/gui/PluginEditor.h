#pragma once

#include <array>

#include "proc/PluginProcessor.h"
#include "gui/vubar.h"
#include "gui/WaveformDisplay.h"
#include "util/Parameters.h"

struct AttachedSlider
{
    AttachedSlider(PluginProcessor& p, invrm::param::PID pid)
        : slider()
        , attachment(
            *(p.getParameters()[(int)pid]),
            slider,
            nullptr
        )
    {};

    juce::Slider slider;
    juce::SliderParameterAttachment attachment;
};

using AttachedSliderArray = std::array<AttachedSlider, invrm::param::numParams>;

class PluginEditor final : public juce::AudioProcessorEditor
{
public:
    explicit PluginEditor (PluginProcessor&);
    ~PluginEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    PluginProcessor& processorRef;
    invrm::VUBar mBarIn;
    invrm::VUBar mBarOut;
    invrm::WaveformDisplay mWaveformDisplay;

    AttachedSliderArray mSliders;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
