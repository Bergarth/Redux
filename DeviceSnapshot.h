#pragma once
#include <juce_core/juce_core.h>

struct DeviceSnapshot {
    juce::String ip;
    juce::StringArray speakers; // FL, C, FR...
    juce::StringArray subs;     // SW1...
    juce::StringArray tactile;  // TAC...
    bool connected = false;
    
    void clear() { speakers.clear(); subs.clear(); tactile.clear(); connected = false; }
};
