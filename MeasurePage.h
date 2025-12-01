#pragma once
// Explicit includes to satisfy the compiler
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_events/juce_events.h>
#include <juce_audio_devices/juce_audio_devices.h>

#include "../audio/AudioEngine.h"
#include "../audio/Analyzer.h"
#include "../services/MeasurementImporter.h"

class MeasurePage : public juce::Component, public juce::Timer
{
public:
    MeasurePage(juce::AudioDeviceManager& deviceManager, AudioEngine& engine);
    ~MeasurePage() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;

private:
    juce::AudioDeviceManager& manager;
    AudioEngine& audioEngine;

    juce::ComboBox selector;
    juce::TextButton playSweepBtn{ "MEASURE" };
    juce::TextButton loadBtn{ "LOAD FILE" };
    
    // FIXED: ProgressBar needs a double variable, not a Label
    double progressValue = 0.0; 
    juce::ProgressBar progressBar{ progressValue };
    
    std::vector<double> responsePoints;
    juce::String graphTitle = "No Measurement Loaded";

    void loadFile();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MeasurePage)
};
