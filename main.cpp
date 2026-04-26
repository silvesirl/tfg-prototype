#include <iostream>
#include <cmath>

#include "Utils/Landmark.h"
#include "Utils/DistanceMetric.h"
#include "Utils/json.hpp"
#include "Utils/httplib.h"
#include "Utils/GlobalConstants.h"
#include "Utils/Language.h"

#include "Utils/sqlite3.h"

using JsonParser = nlohmann::json;

std::string FilteredContinent = "-";
LandmarkType FilteredType = LandmarkType::NONE;

DistanceMetric DistanceMetricChosen = DistanceMetric::KILOMETERS;

Language LanguageChosen = Language::EN;

Landmark CurrentLocation = {"Your location", 0.0 ,0.0};

std::vector<Landmark> LandmarkList;

std::vector<Landmark> LoadDatabase(std::string aPath)
{
    sqlite3* DB;
    sqlite3_stmt* Statement;

    std::vector<Landmark> ReturnValue;

    sqlite3_open(aPath.c_str(), &DB);

    sqlite3_prepare_v2(DB, DB_STRUCTURE, -1, &Statement, nullptr);

    while(sqlite3_step(Statement) == SQLITE_ROW)
    {
        Landmark CurrentLandmark;

        CurrentLandmark.Name = reinterpret_cast<const char*>(sqlite3_column_text(Statement, 0));
        CurrentLandmark.Lat = sqlite3_column_double(Statement, 1);
        CurrentLandmark.Lon = sqlite3_column_double(Statement, 2);

        std::string StringType = reinterpret_cast<const char*>(sqlite3_column_text(Statement, 3));
        CurrentLandmark.Type = (StringType == "monument" ? LandmarkType::MONUMENT : LandmarkType::NATURALWONDER);

        CurrentLandmark.Continent = reinterpret_cast<const char*>(sqlite3_column_text(Statement, 4));
        CurrentLandmark.Image = reinterpret_cast<const char*>(sqlite3_column_text(Statement, 5));
        CurrentLandmark.MapsLink = reinterpret_cast<const char*>(sqlite3_column_text(Statement, 6));

        ReturnValue.push_back(CurrentLandmark);
    }

    sqlite3_finalize(Statement);
    sqlite3_close(DB);

    return ReturnValue;
}

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
            UnitMetric = "Feet";
            break;
    }

    std::cout << "La distancia entre "<< aMappedLandmark.Name << " y tu localizacion es de " << Distance << UnitMetric << std::endl;

    return Distance;
}

int main()
{
    httplib::Server Server;

    LandmarkList = LoadDatabase(DB_PATH);

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

        JsonResponse["metric"] = "Feet";

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

    Server.Post("/changelanguage", [](const httplib::Request& Request, httplib::Response& Response)
    {
        nlohmann::json RequestData = nlohmann::json::parse(Request.body);

        auto LanguageValue = TransformStringToType(RequestData["language"]);

        std::string LanguageString = RequestData.value("language", "en");

        LanguageChosen = StringToType(LanguageString);

        switch(LanguageChosen)
        {
            case Language::EN:
                std::cout << "lenguaje cambiado a Ingles" << std::endl;
                break;
            case Language::ES:
                std::cout << "lenguaje cambiado a Español" << std::endl;
                break;
            case Language::ITA:
                std::cout << "lenguaje cambiado a Italiano" << std::endl;
                break;
            case Language::FR:
                std::cout << "lenguaje cambiado a Frances" << std::endl;
                break;
            case Language::DE:
                std::cout << "lenguaje cambiado a Aleman" << std::endl;
                break;
            case Language::CH:
                std::cout << "lenguaje cambiado a Chino" << std::endl;
                break;
            case Language::JP:
                std::cout << "lenguaje cambiado a Japones" << std::endl;
                break;
            case Language::KR:
                std::cout << "lenguaje cambiado a Coreano" << std::endl;
                break;
        }

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

        for(Landmark CurrentLandmark : LandmarkList)
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
            LandMarkObject["imageurl"] = CurrentLandmark.Image;
            LandMarkObject["maplink"] = CurrentLandmark.MapsLink;

            ResponseList.push_back(LandMarkObject);
        }

        Response.set_content(ResponseList.dump(), "application/json");
    });

    std::cout << "Servidor escuchando en http://localhost:18080..." << std::endl;
    Server.listen("0.0.0.0", SERVER_PORT);
    
    return 0;
}