#pragma once
#include <string>
#include <vector>
#include <map>
#include <limits> // Per inizializzare min/max

// Creiamo una struct per contenere i dati E i metadati di una colonna
struct DataColumn {
    std::vector<double> values;
    double min = std::numeric_limits<double>::max(); // Inizia al max possibile
    double max = std::numeric_limits<double>::lowest(); // Inizia al min possibile
};

struct TelemetryData {
    std::string fileName;
    std::vector<std::string> columnNames;

    // La mappa ora usa il nome della colonna per mappare a un oggetto DataColumn
    std::map<std::string, DataColumn> columns;

    // --- HELPER AGGIORNATI ---
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

    // Manteniamo questo helper per compatibilità con le altre view
    std::vector<double>& GetColumnData(const std::string& colName) {
        return columns[colName].values;
    }
};