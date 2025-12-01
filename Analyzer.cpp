#include "Analyzer.h"

using namespace juce;

Analyzer::Analyzer() {}

void Analyzer::calculateFrequencyResponse(const AudioBuffer<float>& buffer, 
                                          double sampleRate, 
                                          std::vector<float>& magnitudes)
{
    if (buffer.getNumSamples() < fftSize) return;

    std::vector<float> fftData(fftSize * 2, 0.0f);
    const float* raw = buffer.getReadPointer(0);
    
    // Find Impulse Peak to center the window
    int peakIdx = 0;
    float peakMag = 0.0f;
    for (int i = 0; i < buffer.getNumSamples(); ++i) {
        float m = std::abs(raw[i]);
        if (m > peakMag) { peakMag = m; peakIdx = i; }
    }
    
    // Safety: If silent, just return silence
    if (peakMag < 0.0001f) {
        magnitudes.assign(fftSize / 2, -100.0f);
        return;
    }
    
    int start = peakIdx - (fftSize / 2);
    if (start < 0) start = 0;
    
    for (int i = 0; i < fftSize; ++i) {
        if (start + i < buffer.getNumSamples())
            fftData[i] = raw[start + i];
    }

    window.multiplyWithWindowingTable(fftData.data(), fftSize);
    fft.performFrequencyOnlyForwardTransform(fftData.data());

    magnitudes.clear();
    magnitudes.resize(fftSize / 2);
    
    for (int i = 0; i < fftSize / 2; ++i) {
        // Boost raw FFT values to look like standard SPL (Arbitrary +20dB offset for now)
        magnitudes[i] = Decibels::gainToDecibels(fftData[i]) + 20.0f; 
    }
}
