#pragma once

#include "proc/PluginProcessor.h"
#include "gui/vubar.h"
#include "gui/WaveformDisplay.h"

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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
