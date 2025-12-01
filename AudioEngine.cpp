#include "AudioEngine.h"
#include <cmath>

AudioEngine::AudioEngine() {}
AudioEngine::~AudioEngine() {}

void AudioEngine::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    currentSampleRate = sampleRate;
    // Calculate 4 seconds of samples
    sweepLengthInSamples = static_cast<int>(4.0 * sampleRate);
}

void AudioEngine::releaseResources() {}

void AudioEngine::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();

    if (!isPlaying) return;

    auto* leftChannel = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    auto* rightChannel = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);

    for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
    {
        if (sweepPosition >= sweepLengthInSamples)
        {
            isPlaying = false;
            break;
        }

        // Generate a simple test tone (Sine Wave) for now to prove audio works
        // Later we swap this for the Logarithmic Chirp math
        float currentSample = (float)std::sin(2.0 * juce::MathConstants<double>::pi * 440.0 * (sweepPosition / currentSampleRate));
        
        // Write to both channels
        if (leftChannel) leftChannel[sample] = currentSample * 0.5f; 
        if (rightChannel) rightChannel[sample] = currentSample * 0.5f;

        sweepPosition++;
    }
}

void AudioEngine::startSweep()
{
    sweepPosition = 0;
    isPlaying = true;
}

bool AudioEngine::isSweepComplete() const
{
    return !isPlaying;
}
