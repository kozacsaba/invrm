#include "proc/PluginProcessor.h"
#include "PluginEditor.h"
#include "gui/vubar.h"

PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p)
    , processorRef (p)
    , mWaveformDisplay (processorRef.getWaveformBuffer())
    , mSliders
    {
        AttachedSlider(p, invrm::param::PID::PreGain),
        AttachedSlider(p, invrm::param::PID::Threshold),
        AttachedSlider(p, invrm::param::PID::WetMix)
    }
{
    for(auto& slider : mSliders)
    {
        addAndMakeVisible(slider.slider);
        slider.slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 25);
    }

    invrm::VUBar::setUpdateFrequency(25);
    addAndMakeVisible(mBarIn);
    mBarIn.query = [this]() -> float
    {
        return processorRef.getBusLevel(0);
    };
    addAndMakeVisible(mBarOut);
    mBarOut.query = [this]() -> float
    {
        return processorRef.getBusLevel(1);
    };

    addAndMakeVisible(mWaveformDisplay);

    setSize (400, 300);
}

PluginEditor::~PluginEditor()
{
}


void PluginEditor::paint (juce::Graphics& g)
{
    auto area = getLocalBounds();
    area.removeFromLeft(60);

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour(juce::Colours::black);
    g.setFont(20.0f);
    g.drawText("Hello, world", area, juce::Justification::centred, true);
}

void PluginEditor::resized()
{
    auto area = getLocalBounds();
    mBarIn.setBounds(area.removeFromLeft(30));
    mBarOut.setBounds(area.removeFromRight(30));

    auto knobArea = area.removeFromBottom(area.getHeight() / 3);
    const int knobWidth = knobArea.getWidth() / invrm::param::numParams;
    for(int i = 0; i < invrm::param::numParams; i++)
    {
        mSliders[(size_t)i].slider.setBounds(knobArea.removeFromLeft(knobWidth));
    }

    mWaveformDisplay.setBounds(area.reduced(20));
}
