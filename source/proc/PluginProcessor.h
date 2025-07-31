#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "Modulator.h"
#include "proc/WaveformBuffer.h"
#include "util/Parameters.h"

class PluginProcessor final : public juce::AudioProcessor
{
public:
    PluginProcessor();
    ~PluginProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int index) override { juce::ignoreUnused (index); }
    const juce::String getProgramName (int index) override 
    {
        juce::ignoreUnused (index);
        return {};
    }
    void changeProgramName (int index, const juce::String& newName) override 
    {
        juce::ignoreUnused (index, newName);
    }
    
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    float getBusLevel(int bus) const;
    const invrm::WaveformBuffer& getWaveformBuffer() const { return mWaveformBuffer; }

private:
    APVTS mApvts;
    std::vector<RAP*> mParams;

    std::unique_ptr<invrm::Modulator> mModulator;
    std::atomic<std::array<float, 2>> mBusLevels;
    invrm::WaveformBuffer mWaveformBuffer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};
