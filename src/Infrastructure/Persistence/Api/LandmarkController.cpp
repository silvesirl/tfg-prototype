#include <iostream>
#include "json.hpp"
#include "httplib.h"
#include "Landmark.h"
#include "LandmarkService.h"
#include "LandmarkController.h"

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
        std::cout << "1. He entrado en la lambda" << std::endl;

        JsonParser RequestData = JsonParser::parse(Request.body);
        
        double lat = RequestData.at("lat").get<double>();
        double lon = RequestData.at("lon").get<double>();
        std::string continent = RequestData.value("continent", "-");

        std::cout << "Request recibida: lat " << lat << " lon " << lon << std::endl;

        auto Landmarks = Service->GetProcessedLandmarks(lat, lon, continent);

        JsonParser ResponseList = JsonParser::array();

        for(const auto& CurrentLandmark : Landmarks)
        {
            auto Distance = Service->CalculateHaversine(lat, lon, CurrentLandmark.Lat, CurrentLandmark.Lon);

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
}