#pragma once
#include <string>
#include <vector>
#include <optional>
#include "../audio/MeasurementData.h"

namespace Genesis {
    class MeasurementImporter {
    public:
        static std::optional<MeasurementData> importREW(const std::string& filePath);
        static std::optional<MeasurementData> importAudyssey(const std::string& filePath, const std::string& targetChannelCode);
    };
}
