#pragma once
// Explicit includes (Bulletproof method)
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_audio_utils/juce_audio_utils.h> 
#include <juce_audio_devices/juce_audio_devices.h>

#include "ui/HomePage.h"
#include "ui/MeasurePage.h"
#include "audio/AudioEngine.h"

class MainComponent : public juce::Component
{
public:
    MainComponent();
    ~MainComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    juce::TabbedComponent tabs { juce::TabbedButtonBar::TabsAtTop };
    
    // Audio Infrastructure
    juce::AudioDeviceManager deviceManager;
    AudioEngine audioEngine;
    juce::AudioSourcePlayer audioSourcePlayer;

    // Pages
    HomePage homePage;
    MeasurePage measurePage;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
