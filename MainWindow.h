#pragma once
#include <juce_gui_extra/juce_gui_extra.h>
class MainComponent;

class MainWindow : public juce::DocumentWindow
{
public:
    explicit MainWindow(const juce::String& name);
    void closeButtonPressed() override;
};
