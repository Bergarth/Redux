#pragma once
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <atomic>

class AudioEngine : public juce::AudioSource
{
public:
    AudioEngine();
    ~AudioEngine() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

    void startSweep();
    bool isSweepComplete() const;

private:
    double currentSampleRate = 48000.0;
    std::atomic<bool> isPlaying { false };
    std::atomic<int> sweepPosition { 0 };
    int sweepLengthInSamples = 0;
};
