#pragma once
#include <juce_dsp/juce_dsp.h>

class Analyzer
{
public:
    Analyzer();
    void calculateFrequencyResponse(const juce::AudioBuffer<float>& buffer, 
                                    double sampleRate, 
                                    std::vector<float>& magnitudes);
private:
    static constexpr int fftOrder = 15; 
    static constexpr int fftSize  = 1 << fftOrder;
    juce::dsp::FFT fft { fftOrder };
    juce::dsp::WindowingFunction<float> window { fftSize, juce::dsp::WindowingFunction<float>::hann };
};
