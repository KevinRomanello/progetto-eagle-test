#pragma once
#include <string>
#include <vector>
#include <map>
#include <limits>

// Creiamo una struct per contenere i dati di una colonna
struct DataColumn {
    std::vector<double> values;
    double min = std::numeric_limits<double>::max(); // Inizia al max possibile
    double max = std::numeric_limits<double>::lowest(); // Inizia al min possibile per il master plot
};

struct TelemetryData {
    std::string fileName;
    std::vector<std::string> columnNames;

    // La mappa ora usa il nome della colonna per mappare a un oggetto DataColumn
    std::map<std::string, DataColumn> columns;


    std::vector<double>& GetTimestamps() {
        if (columnNames.empty()) {
            static std::vector<double> empty;
            return empty;
        }
        return columns[columnNames[0]].values;
    }

    DataColumn& GetColumn(const std::string& colName) {
        return columns[colName];
    }
    
    std::vector<double>& GetColumnData(const std::string& colName) {
        return columns[colName].values;
    }
};