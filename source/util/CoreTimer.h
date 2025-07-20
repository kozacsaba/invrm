#pragma once

#include <vector>
#include <juce_events/juce_events.h>

#include "Singleton.h"

namespace invrm
{

class UpdatableBase
{
public:
    UpdatableBase();
    ~UpdatableBase();

    static void setUpdateFrequency(int frequencyInHz);

    virtual void timedUpdate() = 0;
}; // UpdatableBase

class CoreTimer
    : public Singleton<CoreTimer>
    , public juce::Timer
{
public:
    CoreTimer();
    ~CoreTimer() override;

    void timerCallback() override;

    bool registerUpdatable(UpdatableBase* component);
    bool deregisterUpdatable(UpdatableBase* component);

private:
    std::vector<UpdatableBase*> mComponents;
}; // CoreTimer

} // invrm
