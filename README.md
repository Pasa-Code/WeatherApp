# C++ Weather Application (CLI)

A robust command-line interface (CLI) weather application built with **C++17**. This project demonstrates professional software development practices, including third-party API integration, secure credential handling, and clean architectural patterns.

## 🚀 Features
- **Real-time Data:** Fetches live weather information from the [OpenWeatherMap API](https://openweathermap.org/api).
- **Safe Parsing:** Implements the `nlohmann/json` library with strict type-safety and fallback values (`.value()`) to prevent runtime crashes.
- **Detailed Reporting:** Displays comprehensive data including temperature (current/min/max), humidity, wind conditions, and localized sunrise/sunset times.
- **Clean Architecture:** Utilizes a dedicated `WeatherReport` struct to decouple data processing from the user interface.
- **Automated Dependencies:** Uses CMake `FetchContent` to automatically download and link required libraries (`CPR` and `JSON`) during the build process.

## 🛠 Tech Stack
- **Language:** C++17
- **Networking:** [CPR](https://github.com/libcpr/cpr) (C++ Requests)
- **JSON Handling:** [nlohmann/json](https://github.com/nlohmann/json)
- **Build System:** CMake (min. version 3.20)

## 📦 Installation & Setup

### Prerequisites
- A C++17 compatible compiler (Clang, GCC, or MSVC).
- [CMake](https://cmake.org/) installed on your system.
- An API Key from OpenWeatherMap.

### Configuration
To ensure security, the API key is stored in a local header file that is excluded from version control.

1. Create a file named `api_key.h` in the root directory:
   ```cpp
   #ifndef WEATHERAPP_API_KEY_H
   #define WEATHERAPP_API_KEY_H

   // Replace with your actual OpenWeatherMap API Key
   #define OPENWEATHER_API_KEY "YOUR_API_KEY_HERE"

   #endif
2. Clone the repository:
    ```bash
   git clone [https://github.com/Pasa-Code/WeatherApp.git](https://github.com/Pasa-Code/WeatherApp.git)
    cd WeatherApp
3. Build with CMake(or simply open projekt in IDE):
    ```bash
    mkdir build && cd build
    cmake ..
    cmake --build . 
## 📸 Usage
Run the executable and follow the interactive prompts:
```plaintext
Stadtname eingeben: Berlin
Lade Wetterdaten fuer Berlin ...
--- WETTERBERICHT FUER Berlin (DE) ---
Zustand:        Klarer Himmel
Temperatur:     16.2 C (Gefuehlt: 15.1 C)
Luftfeuchte:    51%
Wind:           8.5 m/s aus 231 Grad
...
```
## 🔒 Security Note
The project uses a .gitignore file to ensure that sensitive credentials (api_key.h) and build artifacts (cmake-build-debug/) are never pushed to the public repository, following industry-standard security protocols.
