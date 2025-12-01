#include "MeasurePage.h"

using namespace Genesis;

MeasurePage::MeasurePage(juce::AudioDeviceManager& dm, AudioEngine& eng)
    : manager(dm), audioEngine(eng)
{
    addAndMakeVisible(selector);
    selector.addItem("Front Left", 1);
    selector.addItem("Front Right", 2);
    selector.addItem("Center", 3);
    selector.setSelectedId(1);

    addAndMakeVisible(playSweepBtn);
    playSweepBtn.onClick = [this] {
        progressValue = 0.0; // Reset bar
        audioEngine.startSweep();
        startTimer(30); // Faster timer for smoother animation
    };

    addAndMakeVisible(loadBtn);
    loadBtn.onClick = [this] { loadFile(); };

    addAndMakeVisible(progressBar);
}

MeasurePage::~MeasurePage() {}

void MeasurePage::loadFile()
{
    auto chooser = std::make_shared<juce::FileChooser>("Select Measurement",
        juce::File::getSpecialLocation(juce::File::userHomeDirectory),
        "*.txt;*.ady;*.mqx");

    chooser->launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
        [this, chooser](const juce::FileChooser& fc)
        {
            auto file = fc.getResult();
            if (file.existsAsFile())
            {
                std::optional<MeasurementData> data;
                if (file.hasFileExtension(".txt")) {
                    data = MeasurementImporter::importREW(file.getFullPathName().toStdString());
                }
                else if (file.hasFileExtension(".ady") || file.hasFileExtension(".mqx")) {
                    data = MeasurementImporter::importAudyssey(file.getFullPathName().toStdString(), "FL");
                }

                if (data.has_value()) {
                    responsePoints = data->magnitudeDB;
                    graphTitle = data->name.empty() ? file.getFileName() : juce::String(data->name);
                    repaint();
                }
            }
        });
}

void MeasurePage::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    auto area = getLocalBounds().reduced(20);
    auto graphArea = area.removeFromBottom(area.getHeight() - 50);
    
    g.setColour(juce::Colours::darkgrey);
    g.drawRect(graphArea);

    g.setColour(juce::Colours::white);
    g.setFont(20.0f);
    g.drawText(graphTitle, area.removeFromTop(30), juce::Justification::centred);

    if (!responsePoints.empty())
    {
        g.setColour(juce::Colours::lime);
        juce::Path path;
        
        float xStep = (float)graphArea.getWidth() / (float)responsePoints.size();
        float yMin = -60.0f;
        float yMax = 10.0f;
        float yRange = yMax - yMin;

        for (size_t i = 0; i < responsePoints.size(); ++i)
        {
            float db = (float)responsePoints[i];
            float x = graphArea.getX() + (i * xStep);
            float normalizedY = 1.0f - ((db - yMin) / yRange);
            float y = graphArea.getY() + (normalizedY * graphArea.getHeight());

            if (i == 0) path.startNewSubPath(x, y);
            else path.lineTo(x, y);
        }
        g.strokePath(path, juce::PathStrokeType(2.0f));
    }
}

void MeasurePage::resized()
{
    auto area = getLocalBounds().reduced(20);
    auto topRow = area.removeFromTop(40);
    selector.setBounds(topRow.removeFromLeft(200));
    playSweepBtn.setBounds(topRow.removeFromLeft(100).reduced(5));
    loadBtn.setBounds(topRow.removeFromLeft(100).reduced(5));
    area.removeFromTop(10);
    progressBar.setBounds(area.removeFromTop(20));
}

void MeasurePage::timerCallback()
{
    if (audioEngine.isSweepComplete()) {
        stopTimer();
        progressValue = 1.0; // Ensure bar is full
    } else {
        // Fake progress for now (increment by 1% per tick)
        // Later we will ask AudioEngine for exact progress
        if (progressValue < 1.0) progressValue += 0.01;
    }
}
