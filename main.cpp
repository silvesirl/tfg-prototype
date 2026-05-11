#include <iostream>
#include <cmath>

#include "Landmark.h"
#include "DistanceMetric.h"
#include "Utils/json.hpp"
#include "httplib.h"
#include "Utils/GlobalConstants.h"
#include "Utils/Language.h"

#include "LandmarkController.h"
#include "LandmarkService.h"
#include "ILandmarkDBRepository.h"
#include "SQLiteLandmarkRepository.h"

using JsonParser = nlohmann::json;

std::string FilteredContinent = "-";
LandmarkType FilteredType = LandmarkType::NONE;

DistanceMetric DistanceMetricChosen = DistanceMetric::KILOMETERS;

Language LanguageChosen = Language::EN;

Landmark CurrentLocation = {"Your location", 0.0 ,0.0};

std::vector<Landmark> LandmarkList;

std::unique_ptr<ILandmarkDBRepository> DBRepository;

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

    DBRepository = std::unique_ptr<ILandmarkDBRepository>(new SQLiteLandmarkRepository());

    auto LService = std::make_shared<LandmarkService>(*DBRepository);

    LandmarkController LController(Server, LService);

    LController.RegisterRoutes();

    /**

    Server.set_post_routing_handler([](const auto& req, auto& res)
    {
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

    */

    std::cout << "Servidor escuchando en http://localhost:18080..." << std::endl;
    Server.listen("0.0.0.0", SERVER_PORT);

    
    return 0;
}