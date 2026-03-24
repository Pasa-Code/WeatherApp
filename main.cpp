#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include "api_key.h"

using json = nlohmann::json;

struct WeatherReport {
    std::string cityName;
    std::string country;
    double lat = 0.0;
    double lon = 0.0;
    double temp = 0.0;
    double feels_like = 0.0;
    double temp_min = 0.0;
    double temp_max = 0.0;
    int pressure = 0;
    int humidity = 0;
    int sea_level = 0;
    int grnd_lvl = 0;
    double windSpeed = 0.0;
    int windDeg = 0;
    double windGust = 0.0;
    int clouds = 0;
    int visibility = 0;
    std::string description;
    std::string icon;
    std::string mainCondition;
    std::string sunriseTime;
    std::string sunsetTime;
    std::string dataTime;
    bool success = false; // Flag, um zu prüfen ob das Parsen geklappt hat
};

// Hilfsfunktion zur Zeitumwandlung
std::string formatUnixTime(long timestamp) {
    if (timestamp == 0) return "n/a";
    std::time_t t = static_cast<std::time_t>(timestamp);
    std::tm *utc = std::gmtime(&t);
    char buffer[100];
    std::strftime(buffer, sizeof(buffer), "%H:%M:%S UTC", utc);
    return std::string(buffer);
}

// PARSE-FUNKTION
WeatherReport parseWeatherData(cpr::Response &r) {
    WeatherReport report;

    if (r.status_code == 200) {
        try {
            json data = json::parse(r.text);

            // Geografie & Basis
            report.cityName = data.value("name", "Unbekannt");
            report.country  = data["sys"].value("country", "??");
            report.lat      = data["coord"].value("lat", 0.0);
            report.lon      = data["coord"].value("lon", 0.0);
            report.visibility = data.value("visibility", 0);

            // Temperaturen & Luft
            report.temp        = data["main"].value("temp", 0.0);
            report.feels_like  = data["main"].value("feels_like", 0.0);
            report.temp_min    = data["main"].value("temp_min", 0.0);
            report.temp_max    = data["main"].value("temp_max", 0.0);
            report.pressure    = data["main"].value("pressure", 0);
            report.humidity    = data["main"].value("humidity", 0);
            report.sea_level   = data["main"].value("sea_level", 0);
            report.grnd_lvl    = data["main"].value("grnd_level", 0);

            // Wind & Wolken
            report.windSpeed = data["wind"].value("speed", 0.0);
            report.windDeg   = data["wind"].value("deg", 0);
            report.windGust  = data["wind"].value("gust", 0.0);
            report.clouds    = data["clouds"].value("all", 0);

            // Wetter-Beschreibung
            if (!data["weather"].empty()) {
                report.description   = data["weather"][0].value("description", "n/a");
                report.icon          = data["weather"][0].value("icon", "");
                report.mainCondition = data["weather"][0].value("main", "");
            }

            // Zeiten formatieren
            report.dataTime    = formatUnixTime(data.value("dt", 0L));
            report.sunriseTime = formatUnixTime(data["sys"].value("sunrise", 0L));
            report.sunsetTime  = formatUnixTime(data["sys"].value("sunset", 0L));

            report.success = true;
        } catch (json::parse_error& e) {
            std::cerr << "JSON Parse Fehler: " << e.what() << std::endl;
        }
    }
    return report;
}

int main() {
    std::string city;

    while (true) {
        std::cout << "Geben Sie einen Stadtnamen ein (oder 'exit'): ";
        std::getline(std::cin, city);

        if (city == "exit") break;
        if (city.empty()) {
            std::cout << "Eingabe leer. Bitte erneut versuchen." << std::endl;
            continue;
        }

        std::cout << "Lade Wetterdaten fuer " << city << " ..." << std::endl;

        // API Call
        cpr::Response r = cpr::Get(
                cpr::Url{"https://api.openweathermap.org/data/2.5/weather"},
                cpr::Parameters{
                        {"q", city},
                        {"appid", OPENWEATHER_API_KEY},
                        {"units", "metric"},
                        {"lang", "de"}
                }
        );

        if (r.status_code == 200) {
            // Wir erhalten das befüllte Paket
            WeatherReport current = parseWeatherData(r);

            if (current.success) {
                std::cout << "\n--- WETTERBERICHT FUER " << current.cityName << " (" << current.country << ") ---" << std::endl;
                std::cout << "Koordinaten:    [" << current.lat << ", " << current.lon << "]" << std::endl;
                std::cout << "Zustand:        " << current.description << " (" << current.mainCondition << ")" << std::endl;
                std::cout << "Temperatur:     " << std::fixed << std::setprecision(1) << current.temp << " C" << std::endl;
                std::cout << "Gefuehlt:       " << current.feels_like << " C" << std::endl;
                std::cout << "Min/Max:        " << current.temp_min << " / " << current.temp_max << " C" << std::endl;
                std::cout << "Luftfeuchtigkeit: " << current.humidity << "%" << std::endl;
                std::cout << "Luftdruck:      " << current.pressure << " hPa" << std::endl;
                if(current.sea_level > 0) std::cout << "Druck (Meer):   " << current.sea_level << " hPa" << std::endl;

                std::cout << "Wind:           " << current.windSpeed << " m/s aus " << current.windDeg << " Grad";
                if(current.windGust > 0) std::cout << " (Boeen: " << current.windGust << " m/s)";
                std::cout << std::endl;

                std::cout << "Bewoelkung:     " << current.clouds << "%" << std::endl;
                std::cout << "Sichtweite:     " << current.visibility << " m" << std::endl;
                std::cout << "Sonnenaufgang:  " << current.sunriseTime << std::endl;
                std::cout << "Sonnenuntergang: " << current.sunsetTime << std::endl;
                std::cout << "Datenstand:     " << current.dataTime << std::endl;
            }
        } else if (r.status_code == 404) {
            std::cout << "Fehler: Stadt '" << city << "' nicht gefunden." << std::endl;
        } else {
            std::cout << "Fehler beim Abrufen der Daten. Code: " << r.status_code << std::endl;
        }
    }

    std::cout << "Programm beendet. Bis bald!" << std::endl;
    return 0;
}