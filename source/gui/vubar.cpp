#include "vubar.h"

using namespace invrm;

VUBar::VUBar() = default;
VUBar::~VUBar() = default;

void VUBar::timedUpdate()
{
    barValue = query();
    repaint();
}

void VUBar::resized()
{
    auto area = getLocalBounds();
    barArea = area;
}

void VUBar::paint(juce::Graphics& g)
{
    g.setColour(juce::Colours::black);
    g.fillRect(barArea);
    const auto filledArea = barArea.withHeight(barArea.getHeight() * barValue);
    g.setColour(juce::Colours::white);
    g.fillRect(filledArea);
}
