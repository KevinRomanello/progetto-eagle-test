#pragma once
#include <string>
#include <vector>
#include <map>

// Struttura dati flessibile per la telemetria
struct TelemetryData {
    std::string fileName;

    std::vector<std::string> columnNames;
    std::map<std::string, std::vector<double>> columns;

    // Helper per ottenere i dati della prima colonna (X-Axis)
    std::vector<double>& GetTimestamps() {
        if (columnNames.empty()) {
            static std::vector<double> empty;
            return empty;
        }
        return columns[columnNames[0]];
    }

    // Helper per ottenere i dati per nome
    std::vector<double>& GetColumnData(const std::string& colName) {
        return columns[colName];
    }
};