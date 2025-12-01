#include "MeasurementImporter.h"
#include "json.hpp" // <--- FIXED: Includes the file we just downloaded
#include <fstream>
#include <sstream>
#include <iostream>

using json = nlohmann::json;
using namespace Genesis;

std::optional<MeasurementData> MeasurementImporter::importREW(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) return std::nullopt;

    MeasurementData data;
    data.name = "REW Import";
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '*' || line.find("Freq") != std::string::npos) continue;
        std::stringstream ss(line);
        double freq = 0.0, mag = 0.0, phase = 0.0;
        if (ss >> freq >> mag >> phase) {
            data.frequencies.push_back(freq);
            data.magnitudeDB.push_back(mag);
            data.phase.push_back(phase);
        }
    }
    if (data.frequencies.empty()) return std::nullopt;
    return data;
}

std::optional<MeasurementData> MeasurementImporter::importAudyssey(const std::string& filePath, const std::string& targetChannelCode) {
    std::ifstream file(filePath);
    if (!file.is_open()) return std::nullopt;

    try {
        json j = json::parse(file);
        MeasurementData data;
        
        if (!j.contains("detectedChannels")) return std::nullopt;

        for (const auto& channel : j["detectedChannels"]) {
            std::string cmdId = channel.value("commandId", "");
            if (cmdId == targetChannelCode) {
                data.channelCode = cmdId;
                if (channel.contains("responseData")) {
                    auto& resp = channel["responseData"];
                    for (auto& [key, value] : resp.items()) {
                         try {
                             double f = std::stod(key);
                             double m = value.get<double>();
                             data.frequencies.push_back(f);
                             data.magnitudeDB.push_back(m);
                         } catch (...) { continue; }
                    }
                }
                return data;
            }
        }
    } catch (...) {}
    return std::nullopt;
}
