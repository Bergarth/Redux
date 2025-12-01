#pragma once
#include <juce_gui_extra/juce_gui_extra.h>

class TargetBlendPage : public juce::Component
{
public:
    void paint(juce::Graphics& g) override
    {
        g.drawText("Target/Blend page (stub)", getLocalBounds(), juce::Justification::centred);
    }
};
