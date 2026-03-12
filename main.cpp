#include "httplib.h"
#include <iostream>
#include "json.hpp"
#include <cmath>
#include <numbers>
using JsonParser = nlohmann::json;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int Port = 18080;

struct Landmark {
    std::string Name;
    double Lat;
    double Lon;
};

Landmark CurrentLocation = {"Your location", 0.0 ,0.0};

// Privisional, waiting for database
std::vector<Landmark> RegisteredLandmarks = {
    {"Sagrada Familia", 41.4036, 2.1744},
    {"Park Guell", 41.4145, 2.1527},
    {"Casa Batllo", 41.3916, 2.1649},
    {"Camp Nou", 41.3809, 2.1228},
    {"Arc de Triomf", 41.3911, 2.1806}
};

double HaversineDistanceCalculation(Landmark aCurrentLandmark, Landmark aMappedLandmark)
{
    const double EARTH_RADIUS_KM = 6371.0; 

    auto ToRadians = [](double degree) -> double
    { 
        return degree * (M_PI / 180.0); 
    };

    double DLat = ToRadians(aMappedLandmark.Lat - aCurrentLandmark.Lat);
    double DLon = ToRadians(aMappedLandmark.Lon - aCurrentLandmark.Lon);

    double ChordLengthSq = std::sin(DLat / 2.0) * std::sin(DLat / 2.0) +
                           std::cos(ToRadians(aCurrentLandmark.Lat)) * std::cos(ToRadians(aMappedLandmark.Lat)) *
                           std::sin(DLon / 2.0) * std::sin(DLon / 2.0);

    double AngularDistanceRadians = 2.0 * std::atan2(std::sqrt(ChordLengthSq), std::sqrt(1.0 - ChordLengthSq));

    std::cout << "La distancia entre "<< aMappedLandmark.Name << " y tu localizacion es de " << EARTH_RADIUS_KM * AngularDistanceRadians << " Km" << std::endl;

    return EARTH_RADIUS_KM * AngularDistanceRadians;
}

int main()
{
    httplib::Server Server;

    Server.set_post_routing_handler([](const auto& req, auto& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
    });

    Server.Options(R"(/.*)", [](const auto& req, auto& res) {
        res.status = 200;
    });

    Server.Get("/", [](const httplib::Request&, httplib::Response& Response) {
        Response.set_content("API Geospacial en ejecucion", "text/plain");
    });

    Server.Post("/calculate", [](const httplib::Request& Request, httplib::Response& Response)
    {
        JsonParser RequestData = JsonParser::parse(Request.body);

        std::cout << "Request recibida: latitud es " << RequestData["lat"] << "y la longitud es " << RequestData["lon"] << std::endl;

        Landmark CurrentLocation = {CurrentLocation.Name, static_cast<double>(RequestData["lat"]), static_cast<double>(RequestData["lon"])};

        for(Landmark CurrentLandmark : RegisteredLandmarks)
        {
            double DistanceBetweenPoints = HaversineDistanceCalculation(CurrentLocation, CurrentLandmark);
        
            std::string ResponseString = "{\"status\": \"success\", \"distance\": " + std::to_string(DistanceBetweenPoints) + "}";
            Response.set_content(ResponseString, "application/json");   
        }
    });

    std::cout << "Servidor escuchando en http://localhost:18080..." << std::endl;
    Server.listen("0.0.0.0", Port);
    
    return 0;
}