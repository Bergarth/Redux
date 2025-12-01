#pragma once
#include <juce_core/juce_core.h>

// Simple SSDP (UPnP) discoverer for Denon/Marantz AVR devices.
// Sends an M-SEARCH and parses LOCATION/SERVER/USN headers.
struct SSDPDiscoverer
{
    struct Result
    {
        juce::String ip;          // sender IP or LOCATION host
        juce::URL     location;   // LOCATION url if present
        juce::String  server;     // SERVER header (often includes model)
        juce::String  st;         // search-target
        juce::String  usn;        // unique service name
        juce::String  raw;        // raw response (for debugging)
    };

    static juce::Array<Result> discover (int timeoutMs = 1200);
};
