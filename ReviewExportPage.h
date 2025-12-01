#pragma once
#include <juce_gui_extra/juce_gui_extra.h>

class ReviewExportPage : public juce::Component
{
public:
    void paint(juce::Graphics& g) override
    {
        g.drawText("Review/Export page (stub)", getLocalBounds(), juce::Justification::centred);
    }
};
