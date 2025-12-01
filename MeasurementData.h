#pragma once
#include <vector>
#include <string>

namespace Genesis {
    struct MeasurementData {
        std::string name;
        std::string channelCode; 
        int sampleRate = 48000;
        
        std::vector<double> frequencies;
        std::vector<double> magnitudeDB;
        std::vector<double> phase; 

        bool isValid() const { return !frequencies.empty() && frequencies.size() == magnitudeDB.size(); }
    };
}
