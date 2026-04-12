#include <iostream>
#include <cmath>
#include <numbers>

#include "Utils/Landmark.h"
#include "Utils/DistanceMetric.h"
#include "Utils/json.hpp"
#include "Utils/httplib.h"
#include "Utils/GlobalConstants.h"

// json reader
using JsonParser = nlohmann::json;

std::string FilteredContinent = "-";
LandmarkType FilteredType = LandmarkType::NONE;

DistanceMetric DistanceMetricChosen = DistanceMetric::KILOMETERS;

Landmark CurrentLocation = {"Your location", 0.0 ,0.0};

// Provisional, waiting for database
std::vector<Landmark> RegisteredLandmarks = {
    {"Gran Muralla China", 40.4319, 116.5704, LandmarkType::MONUMENT, "Asia"},
    {"Petra", 30.3285, 35.4444, LandmarkType::MONUMENT, "Asia"},
    {"Coliseo Romano", 41.8902, 12.4922, LandmarkType::MONUMENT, "Europa"},
    {"Chichén Itzá", 20.6843, -88.5678, LandmarkType::MONUMENT, "America"},
    {"Machu Picchu", -13.1631, -72.5450, LandmarkType::MONUMENT, "America"},
    {"Taj Mahal", 27.1751, 78.0421, LandmarkType::MONUMENT, "Asia"},
    {"Cristo Redentor", -22.9519, -43.2105, LandmarkType::MONUMENT, "America"},
    {"Sagrada Familia", 41.4036, 2.1744, LandmarkType::MONUMENT, "Europa"},
    {"Estatua de la Libertad", 40.6892, -74.0445, LandmarkType::MONUMENT, "America"},
    {"Torre Eiffel", 48.8584, 2.2945, LandmarkType::MONUMENT, "Europa"},
    {"Cataratas del Niágara", 43.0782, -79.0757, LandmarkType::NATURALWONDER, "Africa"},
    {"Monte Kilimanjaro", -3.0674, 37.3556, LandmarkType::NATURALWONDER, "Africa"},
    {"Monte Everest", 27.9881, 86.9250, LandmarkType::NATURALWONDER, "Asia"},
    {"Falla de San Andrés", 35.1166, -119.6817, LandmarkType::NATURALWONDER, "America"},
    {"Tokyo Tower", 35.6586, 139.7454, LandmarkType::MONUMENT, "Asia"},
    {"Pirámides de Giza", 29.9792, 31.1342,LandmarkType::MONUMENT, "Africa"},
    {"Ayers Rock", -25.3444, 131.0369, LandmarkType::NATURALWONDER, "Oceania"}
};

LandmarkType TransformStringToType(std::string aLandmarkType)
{
    if(aLandmarkType == "Monumento")
    {
        return LandmarkType::MONUMENT;
    }
    else if(aLandmarkType == "Natural")
    {
        return LandmarkType::NATURALWONDER;
    }
    else
    {
        return LandmarkType::NONE;
    }
}

double HaversineDistanceCalculation(Landmark aCurrentLandmark, Landmark aMappedLandmark)
{
    std::string UnitMetric;

    auto ToRadians = [](double degree) -> double
    { 
        return degree * (PI / 180.0); 
    };

    double DLat = ToRadians(aMappedLandmark.Lat - aCurrentLandmark.Lat);
    double DLon = ToRadians(aMappedLandmark.Lon - aCurrentLandmark.Lon);

    double ChordLengthSq = std::sin(DLat / 2.0) * std::sin(DLat / 2.0) +
                           std::cos(ToRadians(aCurrentLandmark.Lat)) * std::cos(ToRadians(aMappedLandmark.Lat)) *
                           std::sin(DLon / 2.0) * std::sin(DLon / 2.0);

    double AngularDistanceRadians = 2.0 * std::atan2(std::sqrt(ChordLengthSq), std::sqrt(1.0 - ChordLengthSq));

    double Distance = EARTH_RADIUS_KM * AngularDistanceRadians;

    switch(DistanceMetricChosen)
    {
        case DistanceMetric::KILOMETERS:
            UnitMetric = "Km";
            break;
        
        case DistanceMetric::MILES:
            Distance = Distance * MILES_TRANSFORM;
            UnitMetric = "Miles";
            break;

        case DistanceMetric::FOOT:
            Distance = Distance * FEET_TRANSFORM;
            UnitMetric = "Foot";
            break;
    }

    std::cout << "La distancia entre "<< aMappedLandmark.Name << " y tu localizacion es de " << Distance << UnitMetric << std::endl;

    return Distance;
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

    Server.Get("/", [](const httplib::Request&, httplib::Response& Response)
    {
        Response.set_content("API Geospacial en ejecucion", "text/plain");
    });

    Server.Post("/changekm", [](const httplib::Request& Request, httplib::Response& Response)
    {
        bool Return;

        nlohmann::json JsonResponse;

        JsonResponse["metric"] = "Kilometers";

        std::cout << "Cambiada metrica de distancia a " << JsonResponse["metric"] << std::endl;

        DistanceMetricChosen = DistanceMetric::KILOMETERS;

        Response.set_content(JsonResponse.dump(), "application/json");
    });

    Server.Post("/changemiles", [](const httplib::Request& Request, httplib::Response& Response)
    {
        bool Return;

        nlohmann::json JsonResponse;

        JsonResponse["metric"] = "Miles";

        std::cout << "Cambiada metrica de distancia a " << JsonResponse["metric"] << std::endl;

        DistanceMetricChosen = DistanceMetric::MILES;

        Response.set_content(JsonResponse.dump(), "application/json");
    });

    Server.Post("/changefoot", [](const httplib::Request& Request, httplib::Response& Response)
    {
        bool Return;

        nlohmann::json JsonResponse;

        JsonResponse["metric"] = "Foot";

        std::cout << "Cambiada metrica de distancia a " << JsonResponse["metric"] << std::endl;

        DistanceMetricChosen = DistanceMetric::FOOT;

        Response.set_content(JsonResponse.dump(), "application/json");
    });

    Server.Post("/changecontinent", [](const httplib::Request& Request, httplib::Response& Response)
    {
        nlohmann::json RequestData = nlohmann::json::parse(Request.body);
        
        FilteredContinent = RequestData.value("continent", "-");

        std::cout << "Filtro de continente actualizado a: " << FilteredContinent << std::endl;

        nlohmann::json JsonResponse;
        JsonResponse["status"] = "success";

        Response.set_content(JsonResponse.dump(), "application/json");
    });

    Server.Post("/changecategory", [](const httplib::Request& Request, httplib::Response& Response)
    {
        nlohmann::json RequestData = nlohmann::json::parse(Request.body);

        FilteredType = TransformStringToType(RequestData.value("category", "-"));

        std::cout << "Filtro de categoria actualizado a: " << FilteredContinent << std::endl;

        nlohmann::json JsonResponse;
        JsonResponse["status"] = "success";

        Response.set_content(JsonResponse.dump(), "application/json");
    });

    Server.Post("/calculate", [](const httplib::Request& Request, httplib::Response& Response)
    {
        JsonParser RequestData = JsonParser::parse(Request.body);

        std::cout << "Request recibida: latitud es " << RequestData["lat"] << "y la longitud es " << RequestData["lon"] << std::endl;

        Landmark CurrentLocation = {CurrentLocation.Name, static_cast<double>(RequestData["lat"]), static_cast<double>(RequestData["lon"])};

        JsonParser ResponseList = JsonParser::array();

        for(Landmark CurrentLandmark : RegisteredLandmarks)
        {
            if(FilteredContinent != "-" && CurrentLandmark.Continent != FilteredContinent)
            {
                continue;
            }

            if(FilteredType != LandmarkType::NONE && CurrentLandmark.Type != FilteredType)
            {
                continue;
            }

            double DistanceBetweenPoints = HaversineDistanceCalculation(CurrentLocation, CurrentLandmark);

            JsonParser LandMarkObject;

            LandMarkObject["name"] = CurrentLandmark.Name;
            LandMarkObject["distance"] = DistanceBetweenPoints;
            LandMarkObject["lat"] = CurrentLandmark.Lat;
            LandMarkObject["lon"] = CurrentLandmark.Lon;
            LandMarkObject["category"] = CurrentLandmark.Type;
            LandMarkObject["continent"] = CurrentLandmark.Continent;

            ResponseList.push_back(LandMarkObject);
        }

        Response.set_content(ResponseList.dump(), "application/json");
    });

    std::cout << "Servidor escuchando en http://localhost:18080..." << std::endl;
    Server.listen("0.0.0.0", SERVER_PORT);
    
    return 0;
}