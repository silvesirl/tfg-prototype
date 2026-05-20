#include <iostream>
#include "json.hpp"
#include "httplib.h"
#include "Landmark.h"
#include "Language.h"
#include "Logger.h"
#include "DistanceMetric.h"
#include "LandmarkService.h"
#include "LandmarkController.h"
#include "DomainConstants.h"
#include "DistanceMetricManager.h"
#include "Localization/LocalizationManager.h"
#include "Mappers/LandmarkFilterContinentMapper.h"

using JsonParser = nlohmann::json;

LandmarkController::LandmarkController(httplib::Server& s, std::shared_ptr<LandmarkService> svc) 
    : Server(s), Service(svc)
{
}

void LandmarkController::RegisterRoutes()
{
    Server.set_post_routing_handler([](const auto& req, auto& res)
    {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
    });

    Server.Options(R"(/.*)", [](const auto& req, auto& res)
    {
        res.status = 200;
    });

    Server.Post("/calculate", [this](const httplib::Request& Request, httplib::Response& Response)
    {
        JsonParser RequestData = JsonParser::parse(Request.body);
        
        double Lat = RequestData.at("lat").get<double>();
        double Lon = RequestData.at("lon").get<double>();

        Landmark CurrentPosition = {"", Lat, Lon};

        Logger::Log(Logger::WarningLevel::INFO, std::format("Request received: lat {} lon {}", Lat, Lon));

        std::vector<Landmark> Landmarks = Service->GetProcessedLandmarks();

        JsonParser ResponseList = JsonParser::array();

        for(const auto& CurrentLandmark : Landmarks)
        {
            uint32_t Distance = Service->CalculateDistance(CurrentPosition, CurrentLandmark);
            std::string TranslatedName = LocalizationManager::GetTranslation(std::string(CurrentLandmark.Name));
            std::string TranslatedContinent = LocalizationManager::GetTranslation("Continent-" + std::string(LandmarkFilterContinentMapper::MapContinentToString(CurrentLandmark.Continent)));

            JsonParser LandMarkObject;
            LandMarkObject["name"] = TranslatedName;
            LandMarkObject["distance"] = Distance;
            LandMarkObject["lat"] = CurrentLandmark.Lat;
            LandMarkObject["lon"] = CurrentLandmark.Lon;
            LandMarkObject["type"] = CurrentLandmark.Type;
            LandMarkObject["continent"] = TranslatedContinent;
            LandMarkObject["imageurl"] = CurrentLandmark.Image;
            LandMarkObject["maplink"] = CurrentLandmark.MapsLink;

            ResponseList.push_back(LandMarkObject);
        }

        Response.set_content(ResponseList.dump(), "application/json");
    });

    Server.Post("/changecontinent", [this](const httplib::Request& Request, httplib::Response& Response)
    {
        nlohmann::json RequestData = nlohmann::json::parse(Request.body);

        std::string SelectedContinent = RequestData.value("continent", std::string{DomainConstants::EMPTY_FILTER});
        
        Service->SetFilteredContinent(SelectedContinent);
        
        Logger::Log(Logger::WarningLevel::INFO, "Continent filter updated to: " + SelectedContinent);

        nlohmann::json JsonResponse;
        JsonResponse["status"] = "success";

        Response.set_content(JsonResponse.dump(), "application/json");
    });

    Server.Post("/changetype", [this](const httplib::Request& Request, httplib::Response& Response)
    {
        nlohmann::json RequestData = nlohmann::json::parse(Request.body);

        std::string SelectedType = RequestData.value("type", std::string{DomainConstants::EMPTY_FILTER});
        
        Service->SetFilteredType(SelectedType);

        Logger::Log(Logger::WarningLevel::INFO, "Type filter updated to: " + SelectedType);

        nlohmann::json JsonResponse;
        JsonResponse["status"] = "success";

        Response.set_content(JsonResponse.dump(), "application/json");
    });

    Server.Post("/changekm", [this](const httplib::Request& Request, httplib::Response& Response)
    {
        nlohmann::json JsonResponse;

        JsonResponse["metric"] = "Kilometers";
        
        Logger::Log(Logger::WarningLevel::INFO, "Distance metric changed to Kilometers");

        Service->SetMetricDistance(JsonResponse["metric"]);

        Response.set_content(JsonResponse.dump(), "application/json");
    });

    Server.Post("/changemiles", [this](const httplib::Request& Request, httplib::Response& Response)
    {
        nlohmann::json JsonResponse;

        JsonResponse["metric"] = "Miles";

        Logger::Log(Logger::WarningLevel::INFO, "Distance metric changed to miles");

        Service->SetMetricDistance(JsonResponse["metric"]);

        Response.set_content(JsonResponse.dump(), "application/json");
    });

    Server.Post("/changefoot", [this](const httplib::Request& Request, httplib::Response& Response)
    {
        nlohmann::json JsonResponse;

        JsonResponse["metric"] = "Feet";

        Logger::Log(Logger::WarningLevel::INFO, "Distance metric changed to feet");

        Service->SetMetricDistance(JsonResponse["metric"]);

        Response.set_content(JsonResponse.dump(), "application/json");
    });

    Server.Post("/changelanguage", [](const httplib::Request& Request, httplib::Response& Response)
    {
        nlohmann::json RequestData = nlohmann::json::parse(Request.body);

        auto LanguageValue = RequestData["language"];

        LocalizationManager::SetLanguage(LanguageValue);

        Logger::Log(Logger::WarningLevel::INFO, "Cahnged language to " + LanguageValue);

        nlohmann::json JsonResponse;
        JsonResponse["status"] = "success";

        Response.set_content(JsonResponse.dump(), "application/json");
    });
}