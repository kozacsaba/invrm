/*  VUBar
    Simple component that periodically reads a value and updates its bar accordingly.
*/

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <functional>

#include "util/CoreTimer.h"

namespace invrm
{

class VUBar 
    : public juce::Component
    , public UpdatableBase
{
public:
    VUBar();
    ~VUBar();

    void timedUpdate() override;

    void resized() override;
    void paint(juce::Graphics& g) override;

    std::function<float()> query;

private:
    juce::Rectangle<int> barArea;
    float barValue = 0.f;
}; // VUBar

} // namespace invrm
