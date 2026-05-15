#include <iostream>
#include "json.hpp"
#include "httplib.h"
#include "Landmark.h"
#include "DistanceMetric.h"
#include "LandmarkService.h"
#include "LandmarkController.h"

#include "HaversineKilometerAlgorithm.h"
#include "HaversineMilesAlgorithm.h"
#include "HaversineFeetAlgorithm.h"

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

        std::cout << "Request recieved: lat " << Lat << " lon " << Lon << std::endl;

        auto Landmarks = Service->GetProcessedLandmarks();

        JsonParser ResponseList = JsonParser::array();

        for(const auto& CurrentLandmark : Landmarks)
        {
            auto Distance = Service->CalculateHaversine(CurrentPosition, CurrentLandmark);

            JsonParser LandMarkObject;
            LandMarkObject["name"] = CurrentLandmark.Name;
            LandMarkObject["distance"] = Distance;
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

    Server.Post("/changecontinent", [this](const httplib::Request& Request, httplib::Response& Response)
    {
        nlohmann::json RequestData = nlohmann::json::parse(Request.body);

        std::string SelectedContinent = RequestData.value("continent", "-");
        
        Service->SetFilteredContinent(SelectedContinent);

        std::cout << "Continent filter updated to: " << SelectedContinent << std::endl;

        nlohmann::json JsonResponse;
        JsonResponse["status"] = "success";

        Response.set_content(JsonResponse.dump(), "application/json");
    });

    Server.Post("/changecategory", [this](const httplib::Request& Request, httplib::Response& Response)
    {
        nlohmann::json RequestData = nlohmann::json::parse(Request.body);

        std::string SelectedCategory = RequestData.value("category", "-");
        
        Service->SetFilteredCategory(SelectedCategory);

        std::cout << "Category filter updated to: " << SelectedCategory << std::endl;

        nlohmann::json JsonResponse;
        JsonResponse["status"] = "success";

        Response.set_content(JsonResponse.dump(), "application/json");
    });

    Server.Post("/changekm", [this](const httplib::Request& Request, httplib::Response& Response)
    {
        bool Return;

        nlohmann::json JsonResponse;

        JsonResponse["metric"] = "Kilometers";

        std::cout << "Distance metric changed to " << JsonResponse["metric"] << std::endl;

        Service->SetHaversineStrategy(std::make_unique<HaversineKilometerAlgorithm>());

        Response.set_content(JsonResponse.dump(), "application/json");
    });

    Server.Post("/changemiles", [this](const httplib::Request& Request, httplib::Response& Response)
    {
        bool Return;

        nlohmann::json JsonResponse;

        JsonResponse["metric"] = "Miles";

        std::cout << "Distance metric changed to " << JsonResponse["metric"] << std::endl;

        Service->SetHaversineStrategy(std::make_unique<HaversineMilesAlgorithm>());

        Response.set_content(JsonResponse.dump(), "application/json");
    });

    Server.Post("/changefoot", [this](const httplib::Request& Request, httplib::Response& Response)
    {
        bool Return;

        nlohmann::json JsonResponse;

        JsonResponse["metric"] = "Feet";

        std::cout << "Distance metric changed to " << JsonResponse["metric"] << std::endl;

        Service->SetHaversineStrategy(std::make_unique<HaversineFeetAlgorithm>());

        Response.set_content(JsonResponse.dump(), "application/json");
    });
}