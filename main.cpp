#include "httplib.h"
#include <iostream>
#include "json.hpp"
#include <cmath>
#include <numbers>
using JsonParser = nlohmann::json;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

const double EARTH_RADIUS_KM = 6371.0;

int Port = 18080;

struct Landmark {
    std::string Name;
    double Lat;
    double Lon;
}; 

Landmark CurrentLocation = {"Your location", 0.0 ,0.0};

// Privisional, waiting for database
std::vector<Landmark> RegisteredLandmarks = {
    {"Gran Muralla China", 40.4319, 116.5704},
    {"Petra", 30.3285, 35.4444},
    {"Coliseo Romano", 41.8902, 12.4922},
    {"Chichén Itzá", 20.6843, -88.5678},
    {"Machu Picchu", -13.1631, -72.5450},
    {"Taj Mahal", 27.1751, 78.0421},
    {"Cristo Redentor", -22.9519, -43.2105},
    {"Sagrada Familia", 41.4036, 2.1744},
    {"Estatua de la Libertad", 40.6892, -74.0445},
    {"Torre Eiffel", 48.8584, 2.2945},
    {"Cataratas del Niágara", 43.0782, -79.0757},
    {"Monte Kilimanjaro", -3.0674, 37.3556},
    {"Monte Everest (Himalaya)", 27.9881, 86.9250},
    {"Falla de San Andrés", 35.1166, -119.6817},
    {"Tokyo Tower", 35.6586, 139.7454},
    {"Pirámides de Giza", 29.9792, 31.1342},
    {"Ayers Rock", -25.3444, 131.0369}
};

double HaversineDistanceCalculation(Landmark aCurrentLandmark, Landmark aMappedLandmark)
{
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

        JsonParser ResponseList = JsonParser::array();

        for(Landmark CurrentLandmark : RegisteredLandmarks)
        {
            double DistanceBetweenPoints = HaversineDistanceCalculation(CurrentLocation, CurrentLandmark);

            JsonParser LandMarkObject;

            LandMarkObject["name"] = CurrentLandmark.Name;
            LandMarkObject["distance"] = DistanceBetweenPoints;
            LandMarkObject["lat"] = CurrentLandmark.Lat;
            LandMarkObject["lon"] = CurrentLandmark.Lon;

            ResponseList.push_back(LandMarkObject);
        }

        Response.set_content(ResponseList.dump(), "application/json");
    });

    std::cout << "Servidor escuchando en http://localhost:18080..." << std::endl;
    Server.listen("0.0.0.0", Port);
    
    return 0;
}