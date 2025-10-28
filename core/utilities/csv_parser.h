#pragma once

#include "../core/data/telemetry_data.h"
#include <string>

class CSVParser {
public:
    static TelemetryData Parse(const std::string& filePath);
};

std::string GetFileNameFromPath(const std::string& filePath);
