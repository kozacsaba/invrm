#include "proc/PluginProcessor.h"
#include "PluginEditor.h"
#include "gui/vubar.h"

PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p)
    , processorRef (p)
    , mWaveformDisplay (
        processorRef.getWaveformBuffer(),
        processorRef.getInputBuffer()
    )
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
        addAndMakeVisible(slider.label);
    }

    addAndMakeVisible(mTimeSlider);
    mTimeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    mTimeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 20, 20);
    mTimeSlider.setMinAndMaxValues(50.0, 1000.0, juce::dontSendNotification);
    mTimeSlider.setValue(500.0, juce::dontSendNotification);
    mTimeSlider.onValueChange = [this]() 
    {
        processorRef.getInputBuffer().setDisplayDuration(mTimeSlider.getValue());
        processorRef.getWaveformBuffer().setDisplayDuration(mTimeSlider.getValue());
    };

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
    mWaveformDisplay.setThresholdParameter(
        processorRef.getParameters()[(size_t)invrm::param::PID::Threshold]);

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
        auto panel = knobArea.removeFromLeft(knobWidth);
        mSliders[(size_t)i].label.setBounds(panel.removeFromTop(30));
        mSliders[(size_t)i].slider.setBounds(panel);
    }

    mTimeSlider.setBounds(area.removeFromLeft(20));
    mWaveformDisplay.setBounds(area.reduced(20));
}
