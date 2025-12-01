#pragma once
#include <juce_gui_extra/juce_gui_extra.h>

class App : public juce::JUCEApplication
{
public:
    const juce::String getApplicationName()  override { return "Genesis Redux"; }
    const juce::String getApplicationVersion() override { return "0.1.0"; }
    void initialise (const juce::String&) override;
    void shutdown() override;
private:
    std::unique_ptr<class MainWindow> mainWindow;
};
