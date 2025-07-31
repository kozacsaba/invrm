#include "PluginProcessor.h"
#include "gui/PluginEditor.h"
#include "util/Logger.h"

PluginProcessor::PluginProcessor()
    : juce::AudioProcessor(
        BusesProperties()
            .withInput("Input",  juce::AudioChannelSet::stereo(), true)
            .withOutput("Output", juce::AudioChannelSet::stereo(), true)
            .withInput("Sidechain", juce::AudioChannelSet::stereo(), false)
      )
    , mModulator(std::make_unique<invrm::Modulator>())
{
#if LOG_LEVEL > 2
    auto* logger = patch::FileLogger::getInstance();
    juce::ignoreUnused(logger);
#endif
}
PluginProcessor::~PluginProcessor()
{
}


void PluginProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused(sampleRate, samplesPerBlock);
    //majd ha lesz prefilter

    mBusLevels = {0.f, 0.f};
    mWaveformBuffer.prepareToPlay(sampleRate);
    mModulator->prepareToPlay(samplesPerBlock);
}
void PluginProcessor::releaseResources()
{
    //idk lehet nem kell
}
bool PluginProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    const auto& mainIn  = layouts.getChannelSet(true, 0);
    const auto& mainOut = layouts.getChannelSet(false, 0);

    if (mainIn != mainOut)
        return false;

    if (!(mainIn == juce::AudioChannelSet::mono() || mainIn == juce::AudioChannelSet::stereo()))
        return false;

    if (layouts.inputBuses.size() < 2)
        return true;

    const auto& sideChain = layouts.getChannelSet(true, 1);

    if (mainIn == juce::AudioChannelSet::mono())
        return sideChain == juce::AudioChannelSet::mono();

    if (mainIn == juce::AudioChannelSet::stereo())
        return sideChain == juce::AudioChannelSet::mono() || sideChain == juce::AudioChannelSet::stereo();

    return false;
}

void PluginProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                    juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    juce::ignoreUnused (midiMessages);

    const auto& mainBus = getBusBuffer(buffer, true, 0);
    const float rmsLeft = mainBus.getRMSLevel(0, 0, mainBus.getNumSamples());
    const float rmsRight = mainBus.getRMSLevel(1, 0, mainBus.getNumSamples());
    const float mainBusLevel = (rmsLeft + rmsRight) * 0.5f;

    juce::AudioBuffer<float> sidechain = getBusBuffer(buffer, true, 1);
    const int numSidechainChannels = sidechain.getNumChannels();
    if (numSidechainChannels == 0) 
    {
        mBusLevels.store({mainBusLevel, 0.f});
        return;
    }
    const float rmsLefts = sidechain.getRMSLevel(0, 0, sidechain.getNumSamples());
    const float rmsRights = sidechain.getRMSLevel(1, 0, sidechain.getNumSamples());
    const float sideBusLevel = (rmsLefts + rmsRights) * 0.5f;
    mBusLevels.store({mainBusLevel, sideBusLevel});

    auto numChannels  = std::min(
        getTotalNumInputChannels(),
        getTotalNumOutputChannels());

    // filter sidechain if needed

    juce::AudioBuffer<float> mainInput  = getBusBuffer(buffer, true, 0);
    juce::AudioBuffer<float> mainOutput = getBusBuffer(buffer, false, 0);

    int len = buffer.getNumSamples();

    for(int ch = 0; ch < numChannels; ch++)
    {
        const float* pin = mainInput.getReadPointer(ch);
        float* pout = mainOutput.getWritePointer(ch);
        const float* pside = sidechain.getReadPointer(std::min(ch, numSidechainChannels-1));
        
        mModulator->processBlock(pin, pout, pside, len);
    }

    mWaveformBuffer.pushBuffer(mModulator->getFactorArray());
}


bool PluginProcessor::hasEditor() const { return true; }
juce::AudioProcessorEditor* PluginProcessor::createEditor()
{
    return new PluginEditor (*this);
}

void PluginProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::ignoreUnused (destData);
}
void PluginProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    juce::ignoreUnused (data, sizeInBytes);
}

float PluginProcessor::getBusLevel(int bus) const
{
    jassert(bus < 2);
    return mBusLevels.load()[(size_t)bus];
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}
