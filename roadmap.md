# **Progetto: Analizzatore di Dati Telemetrici per Veicoli**

## **1\. Descrizione del Progetto**

### **Obiettivo Finale**

Creare un'applicazione desktop C++ che permetta di caricare, visualizzare e analizzare dati telemetrici provenienti da file CSV. L'applicazione sarà uno strumento tecnico interattivo, rivolto a ingegneri, team di corse o appassionati che necessitano di interpretare dati di performance di un veicolo.

### **Architettura e Filosofia**

Il software sarà costruito seguendo un'architettura **modulare e scalabile**. Ogni funzionalità principale (Login, Analisi Dati) sarà implementata in file sorgente separati per mantenere il codice pulito e facilitare future espansioni.

Il cuore dell'applicazione sarà un **Singleton** per lo **Stato Globale**, che garantirà che tutte le parti del software (i diversi moduli) condividano e reagiscano in modo coerente agli stessi dati, come lo stato di autenticazione dell'utente o i dati telemetrici caricati.

L'interfaccia utente, costruita con **ImGui** e **ImPlot**, sarà reattiva e orientata ai dati, offrendo visualizzazioni chiare e strumenti di analisi interattivi.

### **Funzionalità Chiave**

1. **Sistema di Autenticazione a Ruoli:**  
   * **Basic User:** Accesso limitato alle visualizzazioni di base (es. solo velocità).  
   * **Maintainer:** Accesso a dati più dettagliati (es. input del pilota come acceleratore e freno).  
   * **Admin:** Accesso completo a tutti i grafici, statistiche avanzate e tabelle di dati grezzi.  
2. **Caricamento e Parsing di File CSV:**  
   * Interfaccia per selezionare e caricare file di telemetria.  
   * Parser robusto per leggere i dati e popolarli nello stato globale dell'applicazione.  
3. **Dashboard di Analisi Multi-Grafico:**  
   * **Grafico Performance:** Visualizzazione di velocità, acceleratore e freno nel tempo.  
   * **Grafico Tracciato:** Visualizzazione del percorso del veicolo (coordinate X vs Y) per analizzare le traiettorie.  
   * **Statistiche Rilevanti:** Calcolo e visualizzazione di metriche chiave come velocità massima, tempo sul giro, etc.  
   * **Tabella Dati:** Vista tabellare dei dati grezzi per un'analisi puntuale.

## **2\. Roadmap di Sviluppo**

Questa roadmap è divisa in fasi logiche, dal setup iniziale alle funzionalità più avanzate.

### **Fase 0: Fondamenta e Setup (Prerequisiti)**

* **Obiettivo:** Avere un ambiente di sviluppo funzionante e la struttura base del progetto.  
* **Task:**  
  1. **Configurare l'Ambiente:** Assicurarsi che CLion (o un altro IDE), CMake e le dipendenze (GLFW, GLEW) siano installati e funzionanti su macOS.  
  2. **Compilare il Progetto Iniziale:** Eseguire il codice attuale per confermare che la finestra e le demo di ImGui/ImPlot si aprano correttamente.  
  3. **Creare la Struttura dei File:** Creare i file sorgente vuoti che serviranno per la modularizzazione: global\_state.h, ui\_login.h, ui\_login.cpp, ui\_plot.h, ui\_plot.cpp.

### **Fase 1: Architettura del Software (La Spina Dorsale)**

* **Obiettivo:** Definire lo stato globale e la struttura a schede dell'interfaccia.  
* **Task:**  
  1. **Definire lo Stato Globale (global\_state.h):**  
     * Implementare la classe GlobalState come **Singleton**.  
     * All'interno, definire l'enum UserRole.  
     * Definire una struct CsvData per contenere i std::vector dei dati (time, speed, etc.).  
     * Definire la struct AppState principale che conterrà il UserRole e un'istanza di CsvData.  
  2. **Implementare la Navigazione a Schede (app.cpp):**  
     * Rimuovere le demo window.  
     * Usare ImGui::BeginTabBar per creare il contenitore delle schede.  
     * Implementare due ImGui::BeginTabItem: "Login" e "Analisi Dati".  
     * Includere gli header dei nuovi moduli (ui\_login.h, ui\_plot.h) e chiamare le rispettive funzioni di rendering all'interno delle schede.

### **Fase 2: Modulo di Autenticazione**

* **Obiettivo:** Creare un sistema di login funzionante che aggiorni lo stato globale.  
* **Task:**  
  1. **Sviluppare l'UI di Login (ui\_login.cpp):**  
     * Aggiungere widget ImGui::InputText per username e password.  
     * Aggiungere un ImGui::Button per "Login" e uno per "Logout".  
  2. **Implementare la Logica di Login/Logout:**  
     * Al click del bottone, la logica deve confrontare le credenziali e aggiornare il userRole nello stato globale.  
     * Mostrare un messaggio di benvenuto o di errore. Il pulsante Logout deve essere visibile solo se loggati.

### **Fase 3: Modulo di Analisi Dati (Funzionalità Core)**

* **Obiettivo:** Caricare dati da un CSV e visualizzare il primo grafico.  
* **Task:**  
  1. **Sviluppare l'UI di Caricamento (ui\_plot.cpp):**  
     * Aggiungere un ImGui::InputText per il percorso del file e un ImGui::Button "Carica Dati".  
  2. **Scrivere il Parser CSV:**  
     * Implementare una funzione C++ che legga il file riga per riga, esegua il parsing dei valori e li inserisca nei std::vector dello stato globale.  
  3. **Implementare il Grafico di Base:**  
     * Usare ImPlot per disegnare il grafico della velocità in funzione del tempo.  
  4. **Implementare il Controllo degli Accessi:**  
     * Rendere la tab "Analisi Dati" accessibile solo se l'utente è autenticato (userRole \!= UserRole::LoggedOut).

### **Fase 4: Funzionalità Avanzate e Rifinitura**

* **Obiettivo:** Arricchire l'applicazione per renderla uno strumento di analisi completo.  
* **Task:**  
  1. **Aggiungere Grafici Avanzati:**  
     * Implementare il grafico degli input del pilota (acceleratore/freno).  
     * Implementare il grafico del tracciato (X vs Y).  
  2. **Calcolare e Mostrare Statistiche:**  
     * Sviluppare la logica per calcolare la velocità massima, etc.  
     * Mostrare questi dati in una sezione della UI, visibile solo ai ruoli autorizzati.  
  3. **Migliorare l'UI/UX:**  
     * Aggiungere feedback visivo per l'utente (es. messaggi di stato).  
     * Organizzare il layout con ImGui::BeginChild, ImGui::Separator, etc., per un aspetto più pulito e professionale.  
  4. **Aggiungere Tabella Dati:**  
     * Usare ImGui::BeginTable per visualizzare i dati grezzi in una tabella scrollabile.