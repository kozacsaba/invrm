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
            *(p.getParameters()[(size_t)pid]),
            slider,
            nullptr
        )
    {
        label.setText(invrm::param::toName(pid), juce::dontSendNotification);
    }

    juce::Slider slider;
    juce::SliderParameterAttachment attachment;
    juce::Label label;
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
    juce::Slider mTimeSlider;

    AttachedSliderArray mSliders;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
