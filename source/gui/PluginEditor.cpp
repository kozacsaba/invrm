#include "proc/PluginProcessor.h"
#include "PluginEditor.h"
#include "gui/vubar.h"

PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p)
    , processorRef (p)
{
    invrm::VUBar::setUpdateFrequency(25.f);
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
}
