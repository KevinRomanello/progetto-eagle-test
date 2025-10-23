#include "csv_parser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

// Funzione helper per estrarre il nome del file
std::string GetFileNameFromPath(const std::string& filePath) {
    size_t lastSlash = filePath.find_last_of("/\\");
    if (lastSlash == std::string::npos) return filePath;
    return filePath.substr(lastSlash + 1);
}

// Funzione helper per dividere l'intestazione
std::vector<std::string> ParseHeader(const std::string& line) {
    std::vector<std::string> headers;
    std::stringstream ss(line);
    std::string header;
    while (std::getline(ss, header, ',')) {
        // Rimuovi eventuali spazi bianchi (opzionale ma robusto)
        header.erase(header.find_last_not_of(" \n\r\t")+1);
        header.erase(0, header.find_first_not_of(" \n\r\t"));
        headers.push_back(header);
    }
    return headers;
}

TelemetryData CSVParser::Parse(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Impossibile aprire il file: " + filePath);
    }

    TelemetryData telemetryData;
    telemetryData.fileName = GetFileNameFromPath(filePath);

    std::string line;
    int lineCount = 0;

    // 1. Leggi l'intestazione
    if (!std::getline(file, line)) {
        throw std::runtime_error("File vuoto.");
    }
    lineCount++;

    telemetryData.columnNames = ParseHeader(line);
    if (telemetryData.columnNames.empty()) {
        throw std::runtime_error("Intestazione CSV non valida o mancante.");
    }

    // 2. Inizializza le colonne nella mappa
    for (const auto& colName : telemetryData.columnNames) {
        telemetryData.columns[colName] = std::vector<double>();
    }

    // 3. Leggi i dati riga per riga
    while (std::getline(file, line)) {
        lineCount++;
        std::stringstream ss(line);
        std::string cell;

        // Itera sulle colonne nell'ordine in cui le abbiamo lette
        for (int i = 0; i < telemetryData.columnNames.size(); ++i) {
            if (!std::getline(ss, cell, ',')) {
                std::cerr << "Errore parsing riga " << lineCount << ": riga incompleta." << std::endl;
                continue; // Salta al prossimo ciclo di riga
            }

            try {
                const std::string& colName = telemetryData.columnNames[i];
                telemetryData.columns[colName].push_back(std::stod(cell));
            } catch (const std::exception& e) {
                std::cerr << "Errore parsing riga " << lineCount << ": dato non valido '" << cell << "'" << std::endl;
            }
        }
    }

    if (telemetryData.GetTimestamps().empty()) {
        throw std::runtime_error("Nessun dato valido trovato nel file.");
    }

    std::cout << "Parsing completato: " << telemetryData.fileName << std::endl;
    return telemetryData;
}