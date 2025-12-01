#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "../services/DeviceService.h"
#include "../session/ProjectSession.h"

class HomePage : public juce::Component, public juce::Timer, public juce::FileDragAndDropTarget
{
public:
    HomePage(DeviceService& svc, ProjectSession& session);
    void paint(juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;
    
    bool isInterestedInFileDrag (const juce::StringArray& files) override;
    void filesDropped (const juce::StringArray& files, int x, int y) override;

private:
    void runScan();
    void runManualConnect();
    void drawItem(juce::Graphics& g, juce::String label, int x, int y, bool present, juce::Colour activeCol);
    void syncToSession(); // SAVES config to ProjectSession

    DeviceService& svc;
    ProjectSession& session; // Reference to Brain
    
    juce::TextButton scanBtn, connectBtn;
    juce::TextEditor ipField;
    
    juce::String statusMsg = "Ready (Drop .avr file)";
    bool isScanning = false;
};
