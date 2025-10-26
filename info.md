# Analizzatore Dati Telemetrici (eagleTest1)

Questo progetto è un'applicazione desktop multi-piattaforma, scritta in C++20, per il caricamento, l'analisi e la visualizzazione di dati telemetrici provenienti da file CSV.

L'interfaccia utente è costruita utilizzando **ImGui** per i widget e **ImPlot** per la generazione di grafici interattivi. L'applicazione include un sistema di autenticazione basato su ruoli (Admin, Advanced, User) che controlla l'accesso alle varie funzionalità di visualizzazione e gestione.

## Architettura e Scelte Progettuali

L'architettura del progetto segue una rigorosa **Separazione delle Competenze (Separation of Concerns)**, ispirata al pattern **Model-View-Controller (MVC)**. Questo approccio garantisce che il codice sia modulare, facile da manutenere e scalabile.

Il flusso dei dati è unidirezionale e reattivo, ideale per un framework *immediate-mode* come ImGui:
1.  La **View** cattura un input (es. click su "Login").
2.  La **View** invia l'input a un **Controller** (es. `auth_controller::login(...)`).
3.  Il **Controller** esegue la logica di business e aggiorna il **Model** (es. `global::get().currentUser.authenticated = true`).
4.  Al frame successivo, la **View** legge il **Model** aggiornato e si ridisegna di conseguenza (es. mostrando i grafici invece del popup di login).

---

### 1. Il Model (I Dati)

Il "Model" rappresenta la singola fonte di verità (Single Source of Truth) dell'intera applicazione.

* **Dove si trova:** `core/data/`, `core/model/`
* **File Chiave:** `core/data/global.h`, `core/data/user.h`, `core/model/telemetry_data.h`

#### Scelta Progettuale: Stato Globale (Singleton)

Invece di passare lo stato dell'applicazione (es. `AppState& state`) come parametro a decine di funzioni di rendering diverse (un pattern noto come "prop drilling"), abbiamo scelto di centralizzare lo stato in un **Singleton** (`global::get()`).

* **Perché?** Qualsiasi componente, sia View che Controller, può accedere istantaneamente allo stato corrente (es. `global::get().currentUser`) o ai dati caricati (es. `global::get().AppState.loadedFiles`) senza bisogno di passaggi di parametri complessi. Questo semplifica enormemente la gestione dello stato in un'architettura ImGui.
* **Contenuto:** Il Singleton `global` possiede `AppState` (flag della UI), `currentUser` (stato di autenticazione) e `userDatabase` (l'elenco di tutti gli utenti).

---

### 2. La View (L'Interfaccia)

La "View" è responsabile esclusivamente del rendering dell'interfaccia utente. Non contiene *alcuna* logica di business.

* **Dove si trova:** `core/view/`
* **File Chiave:** `main_view.cpp`, `plot_view.cpp`, `login_view.cpp`, `user_management_view.cpp`

#### Scelta Progettuale: Componenti Modulari

Invece di un unico, monolitico file `.cpp` per l'intera UI, ogni componente logico dell'interfaccia è isolato nel proprio file (`.h`/`.cpp`).

* **Perché?** Questo rende il codice incredibilmente pulito. Il file `main_view.cpp` agisce da "assemblatore": disegna gli elementi principali (Menu Bar, Sidebar) e poi, in base allo stato, chiama le funzioni di rendering appropriate (es. `RenderLoginView()` o `RenderContentView()`).
* **Principio:** Le Viste sono "stupide" (Dumb Views). Leggono i dati dal **Model** e disegnano i widget ImGui. Non modificano *mai* lo stato direttamente; delegano questo compito ai **Controller**.

---

### 3. Il Controller (La Logica)

I "Controller" sono il cervello dell'applicazione. Contengono tutta la logica di business e sono gli unici componenti autorizzati a *modificare* il Model.

* **Dove si trova:** `core/controllers/`
* **File Chiave:** `auth_controller.cpp`, `user_management_controller.cpp`, `upload_file_controller.cpp`

#### Scelta Progettuale: Logica di Business Separata

Quando un utente clicca un pulsante nella **View** (es. `ImGui::Button("Aggiungi Utente")`), la View non fa altro che chiamare una funzione statica del **Controller** (es. `user_management_controller::RequestShowAddUserPopup()`).

* **Perché?** La View non sa (e non deve sapere) *come* si aggiunge un utente, *come* si parsa un file CSV o *come* si valida una password. Questo compito spetta ai Controller, che manipolano il **Model**. La View si limiterà a reagire al cambiamento del Model nel frame successivo.
* **Esempio:** `upload_file_controller.cpp` contiene la logica per chiamare il parser (`csv_parser.cpp`), creare un oggetto `TelemetryData` e inserirlo nel vettore `loadedFiles` dello stato globale.

##  Struttura del Progetto

```
.
├── CMakeLists.txt            # File di build principale per CMake
├── bin/                      # Output della compilazione (eseguibili)
│   ├── imgui.ini             # Salva lo stato della UI (posizioni finestre)
│   └── project_1             # L'eseguibile compilato
├── core/                     # Il cuore del codice sorgente dell'applicazione
│   ├── app.cpp / .h          # Setup del loop principale di ImGui/ImPlot
│   ├── main.cpp              # Entry point (`int main()`)
│   │
│   ├── data/                 # MODEL: Contiene lo stato globale (Singleton)
│   │   ├── global.cpp / .h
│   │   ├── AppState.h
│   │   ├── user.h
│   │   └── ...
│   │
│   ├── model/                # MODEL: Definizioni e implementazioni delle strutture dati
│   │   ├── telemetry_data.cpp / .h
│   │   └── user.cpp / .h
│   │
│   ├── view/                 # VIEW: Tutti i componenti UI (modulari)
│   │   ├── main_view.cpp / .h        # Assemblatore UI principale (Menu, Sidebar)
│   │   ├── plot_view.cpp / .h        # Logica di rendering per i grafici ImPlot
│   │   ├── login_view.cpp / .h       # Popup di login
│   │   ├── user_management_view.cpp  # Popup di gestione utenti (Admin)
│   │   └── ...
│   │
│   ├── controllers/          # CONTROLLER: Tutta la logica di business
│   │   ├── auth_controller.cpp / .h
│   │   ├── upload_file_controller.cpp / .h
│   │   ├── user_management_controller.cpp / .h
│   │   └── ...
│   │
│   └── utilities/            # Funzioni di supporto
│       └── csv_parser.cpp / .h     # Logica per il parsing dei file CSV
│
├── csv_samples/              # Dati di esempio per il testing
│   ├── acceleration.csv
│   └── skidpad.csv
│
├── readme.md               # File della consegna
├── info.md                 # Questo file
│
└── thirdparty/               # Librerie esterne (non modificate)
    ├── ImGuiFileDialog/      # Helper per la finestra di dialogo "Apri File"
    ├── imgui/                # Libreria Dear ImGui (core UI)
    └── implot/               # Libreria ImPlot (grafici)
```

##  Dipendenze Principali

* **[Dear ImGui](https://github.com/ocornut/imgui)**: Per l'intera interfaccia grafica.
* **[ImPlot](https://github.com/epezent/implot)**: Per i grafici 2D.
* **[ImGuiFileDialog](https://github.com/aiekick/ImGuiFileDialog)**: Per la finestra di dialogo nativa "Apri File".
* **GLFW** & **OpenGL**: Per la creazione della finestra e il rendering (gestiti tramite i *backends* di ImGui).
---

## Struttura Dati Principale (Il Model)

Il Model è composto da poche strutture dati chiave che definiscono l'intero stato dell'applicazione.

### `core/data/global.h`

Questa è l'implementazione del **Singleton**. Definisce le macro-aree dello stato:

* `AppState AppState`: Una struct che contiene tutti i **flag della UI** (es. `showLoginPopup`, `showUserManagementPopup`). Serve alla View per sapere quali finestre mostrare.
* `currentUser`: Una struct che memorizza i dati dell'**utente attualmente loggato** (username, ruolo, e il flag `authenticated`).
* `userDatabase`: Un `std::vector<UserData>` che agisce come "database" in memoria di tutti gli utenti registrati.
* `loadedFiles`: Un `std::vector<TelemetryData>` che contiene tutti i file CSV parsati e pronti per essere visualizzati.

### `core/model/telemetry_data.h`

Questa classe è il cuore dei dati telemetrici.

* `class DataColumn`: Una struct interna che memorizza i dati di una singola colonna del CSV (es. "throttle").
    * **Perché `min` e `max`?** Memorizziamo il valore minimo e massimo di ogni colonna *al momento del parsing*. Questo è fondamentale per una scelta progettuale: il **Master Plot** richiede la **normalizzazione** dei dati (adattare tutti i valori a un range 0.0-1.0). Calcolare min/max una sola volta all'inizio ci evita di ricalcolarli a ogni singolo frame, garantendo performance elevate.
* `class TelemetryData`: Rappresenta un intero file CSV.
    * Contiene `fileName`, `columnNames` (l'intestazione) e una mappa (`std::map`) che associa il nome di una colonna (`std::string`) al suo oggetto `DataColumn`.
    * **Perché una mappa?** Permette un accesso semantico e immediato ai dati (es. `currentFile.GetColumn("speed")`) invece di usare indici numerici (es. `currentFile.columns[3]`), rendendo il codice più leggibile. Questo porta ad un problema (facilmente risolvibile) ovvero che se ci sono 2 o più colonne con lo stesso nome verranno sovra scritte dall'ultima (assumo per il momento che cò non possa accadere). 

### `core/data/user.h`

Definisce la logica di autenticazione.

* `enum UserRole`: Definisce i tre livelli di accesso: `USER`, `ADVANCED`, `ADMIN`.
* `struct UserData`: Rappresenta un singolo utente nel `userDatabase`. Contiene `username`, `password` e il suo `UserRole`.

---

## Ruoli Utente e Permessi

Il sistema di ruoli è progettato per fornire diversi livelli di accesso ai dati e alle funzionalità di amministrazione.

### 1. `USER` (Utente Base)

* **Scopo:** Visualizzazione di base.
* **Permessi:**
    * Può caricare file CSV.
    * Può visualizzare i dati solo tramite **Plot Individuali** (un grafico separato per ogni colonna: velocità, acceleratore, ecc.).
    * Non può vedere il "Master Plot".
    * Non può accedere al menu "Admin".

### 2. `ADVANCED` (Utente Avanzato)

* **Scopo:** Analisi dati avanzata.
* **Permessi:**
    * Tutti i permessi dell'utente `USER`.
    * Può visualizzare il **Master Plot**, che sovrappone tutti i segnali normalizzati in un unico grafico per un confronto diretto.
    * Non può accedere al menu "Admin".

### 3. `ADMIN` (Amministratore)

* **Scopo:** Piena funzionalità e gestione utenti.
* **Permessi:**
    * Tutti i permessi dell'utente `ADVANCED` (inclusi tutti i grafici).
    * Ha accesso al menu **"Admin"**.
    * Può aprire il popup **"Gestione Utenti"** per:
        * Visualizzare la lista di tutti gli utenti (tranne altri Admin).
        * Aggiungere nuovi utenti (`USER` o `ADVANCED`).
        * Eliminare utenti esistenti.

### Credenziali per accedere
Gli utenti e le loro credenziali sono salvati in un file (core/data/userData.csv) vista l'assenza di un DB.
* Tipo: admin, Username: admin, Password: 1234
* Tipo: advanced, Username: advanced, Password: 1234
* Tipo: user, Username: user, Password: 1234