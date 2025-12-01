#pragma once
#include "../services/DeviceSnapshot.h"
#include <juce_audio_basics/juce_audio_basics.h>

struct MeasurementData {
    juce::String channelID; // e.g. "FL"
    int positionIndex;      // 1..N
    juce::AudioBuffer<float> impulseResponse;
};

class ProjectSession
{
public:
    ProjectSession() {}
    
    // The "Source of Truth" for the system layout
    DeviceSnapshot activeConfig;
    
    // The "Database" of captured sounds
    std::vector<MeasurementData> measurements;
    
    void setConfig(const DeviceSnapshot& s) { activeConfig = s; }
    void clearMeasurements() { measurements.clear(); }
    
    bool hasConfig() const { return !activeConfig.speakers.isEmpty(); }
};
