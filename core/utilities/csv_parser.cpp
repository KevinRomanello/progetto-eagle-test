#include "csv_parser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

/*
 * FUNZIONE DI UTILITÀ (HELPER) #1
 * Questa funzione "pulisce" il percorso del file.
 * Prende un percorso completo come "C:/Utenti/Nome/Documenti/skidpad.csv"
 * e restituisce solo "skidpad.csv".
 * È una funzione di "qualità della vita" per non mostrare percorsi lunghi nella UI.
 */
std::string GetFileNameFromPath(const std::string& filePath) {
    // Cerca l'ultima occorrenza di uno slash (sia / per Unix/Mac che \ per Windows)
    size_t lastSlash = filePath.find_last_of("/\\");

    // Se non trova nessuno slash (std::string::npos), significa che
    // il percorso è già solo il nome del file, quindi lo restituisce.
    if (lastSlash == std::string::npos)
        return filePath;

    // Altrimenti, restituisce la "sotto-stringa" (substr) che inizia
    // dal carattere *dopo* (lastSlash + 1) l'ultimo slash.
    return filePath.substr(lastSlash + 1);
}

/*
 * FUNZIONE DI UTILITÀ (HELPER) #2
 * Questa è una delle parti più importanti della logica flessibile.
 * Prende una singola riga (che ci aspettiamo sia l'intestazione, es. "Timestamp,Position,Velocity")
 * e la divide (splitta) ad ogni virgola, restituendo un vettore di stringhe.
 */
std::vector<std::string> ParseHeader(const std::string& line) {
    std::vector<std::string> headers; // Il vettore che conterrà i nomi delle colonne
    std::stringstream ss(line);       // Mette la riga in uno "stream di stringa"
    std::string header;

    // Esegue un ciclo: "leggi da 'ss' fino alla prossima virgola ','
    // e metti il risultato in 'header'".
    while (std::getline(ss, header, ',')) {

        // Pulizia: rimuove eventuali spazi bianchi prima o dopo il nome
        // (es. " Timestamp " diventa "Timestamp"). Questo rende il parser robusto.
        header.erase(header.find_last_not_of(" \n\r\t")+1);
        header.erase(0, header.find_first_not_of(" \n\r\t"));

        // Aggiunge il nome pulito al nostro vettore di intestazioni
        headers.push_back(header);
    }
    return headers; // Restituisce il vettore (es. ["Timestamp", "Position", "Velocity"])
}


/*
 * FUNZIONE PRINCIPALE
 * Questa è la funzione pubblica chiamata dal 'upload_file_controller'.
 * Orchestra l'intero processo di parsing.
 */
TelemetryData CSVParser::Parse(const std::string& filePath) {
    // 1. APERTURA FILE
    // Tenta di aprire il file in modalità lettura.
    std::ifstream file(filePath);
    // Se il file non si apre (non esiste, è bloccato), lancia un'eccezione
    // che il controller (LoadFile) dovrà gestire in un blocco try-catch.
    if (!file.is_open()) {
        throw std::runtime_error("Impossibile aprire il file: " + filePath);
    }

    // 2. CREAZIONE OGGETTO
    // Crea l'oggetto TelemetryData vuoto che andremo a riempire.
    TelemetryData telemetryData;
    // Imposta il nome del file usando la nostra funzione di utilità.
    telemetryData.fileName = GetFileNameFromPath(filePath);

    std::string line;    // Variabile per contenere ogni riga letta
    int lineCount = 0; // Contatore per messaggi di errore utili

    // 3. PARSING INTESTAZIONE (HEADER)
    // Tenta di leggere la *prima riga* dal file.
    if (!std::getline(file, line)) {
        throw std::runtime_error("File vuoto."); // Se fallisce, il file è vuoto
    }
    lineCount++; // Siamo alla riga 1

    // Chiama la nostra funzione helper per ottenere il vettore di nomi delle colonne
    telemetryData.columnNames = ParseHeader(line);

    // Se non ha trovato intestazioni, il file non è valido.
    if (telemetryData.columnNames.empty()) {
        throw std::runtime_error("Intestazione CSV non valida o mancante.");
    }

    // 4. INIZIALIZZAZIONE MAPPA
    // Questo è fondamentale. Ora che abbiamo i nomi delle colonne
    // (es. "Timestamp", "Velocity"), dobbiamo inizializzare la mappa
    // nel nostro oggetto TelemetryData.
    for (const auto& colName : telemetryData.columnNames) {
        // Per ogni nome, crea una "chiave" nella mappa
        // e le assegna un vettore di double *vuoto*, pronto per essere riempito.
        telemetryData.columns[colName] = std::vector<double>();
    }

    // 5. PARSING DATI (IL LOOP PRINCIPALE)
    // Ora, esegue un ciclo su *tutte le righe rimanenti* del file.
    while (std::getline(file, line)) {
        lineCount++;
        std::stringstream ss(line); // Mette la riga di dati (es. "0.1,15.2,4.5") nello stream
        std::string cell;           // Variabile per contenere il singolo dato (es. "15.2")

        // LOOP INTERNO: Itera sulle colonne per questa riga
        // Usiamo un indice 'i' perché l'ORDINE è importante.
        // Dobbiamo associare il primo dato ("0.1") alla prima intestazione ("Timestamp"),
        // il secondo dato ("15.2") alla seconda intestazione ("Position"), ecc.
        for (int i = 0; i < telemetryData.columnNames.size(); ++i) {

            // Leggi il prossimo dato dalla riga (fino alla virgola)
            if (!std::getline(ss, cell, ',')) {
                // Se fallisce, la riga è "sporca" (es. ha meno colonne dell'intestazione)
                std::cerr << "Errore parsing riga " << lineCount << ": riga incompleta." << std::endl;
                continue; // Ignora questa riga e passa alla prossima
            }

            // Blocco di sicurezza: 'stod' (string to double) fallirà
            // se la cella contiene testo (es. "N/A").
            try {
                // 1. Prendi il nome della colonna per l'indice corrente (es. i=0 -> "Timestamp")
                const std::string& colName = telemetryData.columnNames[i];

                // 2. Converti la cella (es. "15.2") in un double (15.2)
                double value = std::stod(cell);

                // 3. Usa il nome della colonna per trovare il vettore giusto nella mappa
                //    e aggiungi il valore.
                telemetryData.columns[colName].push_back(value);

            } catch (const std::exception& e) {
                // Il dato non era un numero. Lo segnaliamo e andiamo avanti.
                std::cerr << "Errore parsing riga " << lineCount << ": dato non valido '" << cell << "'" << std::endl;
            }
        }
    }

    // 6. CONTROLLO FINALE E RESTITUZIONE
    // Dopo il loop, controlliamo se abbiamo effettivamente caricato qualcosa.
    // 'GetTimestamps()' è l'helper nella struct che prende i dati della prima colonna.
    if (telemetryData.GetTimestamps().empty()) {
        throw std::runtime_error("Nessun dato valido trovato nel file (solo intestazione?).");
    }

    std::cout << "Parsing completato: " << telemetryData.fileName << std::endl;

    // Restituisce l'oggetto TelemetryData completo al controller.
    return telemetryData;
}