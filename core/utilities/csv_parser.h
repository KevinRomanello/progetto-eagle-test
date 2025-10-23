#pragma once

#include "../core/data/telemetry_data.h"
#include <string>

class CSVParser {
public:
    // Tenta di parsare un file CSV con intestazione
    static TelemetryData Parse(const std::string& filePath);
};