#include "MainComponent.h"

MainComponent::MainComponent() 
    : measurePage(deviceManager, audioEngine) // Pass references to MeasurePage
{
    setSize(1024, 768);

    // 1. Initialize Audio Device
    deviceManager.initialiseWithDefaultDevices(1, 2); // 1 Input (Mic), 2 Outputs (Stereo)

    // 2. Connect AudioEngine -> Player -> Device
    audioSourcePlayer.setSource(&audioEngine);
    deviceManager.addAudioCallback(&audioSourcePlayer);

    // 3. Setup UI Tabs
    addAndMakeVisible(tabs);
    tabs.addTab("Home", juce::Colours::darkgrey, &homePage, false);
    tabs.addTab("Measure", juce::Colours::black, &measurePage, false);
}

MainComponent::~MainComponent()
{
    // Clean shutdown is critical
    deviceManager.removeAudioCallback(&audioSourcePlayer);
    audioSourcePlayer.setSource(nullptr);
}

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void MainComponent::resized()
{
    tabs.setBounds(getLocalBounds());
}
