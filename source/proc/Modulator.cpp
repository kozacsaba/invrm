#include "Modulator.h"

using namespace invrm;

Modulator::Modulator() = default;
Modulator::~Modulator() = default;

void Modulator::processBlock (const float* mainIn,
                              float* mainOut,
                              const float* side,
                              int length)
{
    for(int s = 0; s < length; s++)
    {
        float factor = std::abs(side[s]);
        factor *= mPregain;
        factor = std::max(0.f, factor - mThreshold);
        factor = std::max(0.f, 1.f - factor);

        mainOut[s] = factor * mainIn[s];
    }
}

void Modulator::setPregain(float pregain)
{
    mPregain = pregain;
}
void Modulator::setThreshold(float threshold)
{
    mThreshold = threshold;
}
