#pragma once
#include <juce_core/juce_core.h>
#include "../net/TelnetClient.h"
#include "../net/SSDPDiscoverer.h"
#include "DeviceSnapshot.h"

class DeviceService
{
public:
    juce::Array<SSDPDiscoverer::Result> scan() { return SSDPDiscoverer::discover(2000); }
    bool connectAndScan(const juce::String& ip);
    bool loadFromFile(const juce::File& f);
    DeviceSnapshot getSnapshot() const { return snapshot; }

private:
    TelnetClient telnet;
    DeviceSnapshot snapshot;
};
