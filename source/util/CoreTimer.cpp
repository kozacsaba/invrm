#include "CoreTimer.h"
#include "Logger.h"

using namespace invrm;

UpdatableBase::UpdatableBase()
{
    CoreTimer::getInstance()->registerUpdatable(this);
}

UpdatableBase::~UpdatableBase()
{
    CoreTimer::getInstance()->deregisterUpdatable(this);
}

void UpdatableBase::setUpdateFrequency(int f)
{
    CoreTimer::getInstance()->startTimerHz(f);
}


CoreTimer::CoreTimer() = default;

CoreTimer::~CoreTimer()
{
    // Not all components have been deregistered from the core timer, before destruction.
    // Possibility of dangling pointers.
    jassert(mComponents.empty());
}

bool CoreTimer::registerUpdatable(UpdatableBase* b)
{
    for(const auto& it : mComponents)
    {
        if(b == it)
        {
            return false;
        }
    }

    mComponents.emplace_back(b);
    return true;
}

bool CoreTimer::deregisterUpdatable(UpdatableBase* b)
{
    for(size_t i = 0; i < mComponents.size(); i++)
    {
        if(b == mComponents[i])
        {
            mComponents.erase(mComponents.begin() + (long long)i);
            return true;
        }
    }
    return false;
}

void CoreTimer::timerCallback()
{
    for(const auto& it : mComponents)
    {
        it->timedUpdate();
    }
}
