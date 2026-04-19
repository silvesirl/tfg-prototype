#include <iostream>
#include <cmath>
#include <numbers>

#include "Utils/Landmark.h"
#include "Utils/DistanceMetric.h"
#include "Utils/json.hpp"
#include "Utils/httplib.h"
#include "Utils/GlobalConstants.h"
#include "Utils/Language.h"

// json reader
using JsonParser = nlohmann::json;

std::string FilteredContinent = "-";
LandmarkType FilteredType = LandmarkType::NONE;

DistanceMetric DistanceMetricChosen = DistanceMetric::KILOMETERS;

Language LanguageChosen = Language::EN;

Landmark CurrentLocation = {"Your location", 0.0 ,0.0};

// Provisional, waiting for database
std::vector<Landmark> RegisteredLandmarks = {
    {"Gran Muralla China", 40.4319, 116.5704, LandmarkType::MONUMENT, "Asia", "https://content.nationalgeographic.com.es/medio/2014/06/12/china2_1787x2000.jpg", "https://maps.app.goo.gl/fY9fCMhcmmrTFhH78"},
    {"Petra", 30.3285, 35.4444, LandmarkType::MONUMENT, "Asia", "https://isram.com/wp-content/uploads/2018/01/Petra-Jordan-Ad-Deir-Facade.jpg", "https://maps.app.goo.gl/WM7XU9ar5st97oRH7"},
    {"Coliseo Romano", 41.8902, 12.4922, LandmarkType::MONUMENT, "Europa", "https://www.enroma.com/wp-content/uploads/elementor/thumbs/Coliseo-Romano-p9hfybrrriyw7zyeoat3i5xq91dbuq09smp8uhsmrk.jpg", "https://maps.app.goo.gl/DwDBFFh9buRTCfHM6"},
    {"Chichén Itzá", 20.6843, -88.5678, LandmarkType::MONUMENT, "America", "https://www.mostbeautifulspots.com/wp-content/uploads/2016/03/Ayers-Rock-Uluru.jpg", "https://maps.app.goo.gl/fwUZyHwoR1y5RrXd9"},
    {"Machu Picchu", -13.1631, -72.5450, LandmarkType::MONUMENT, "America", "https://www.pakistantraveler.com/wp-content/uploads/2022/09/machu-2-1536x1028.jpg", "https://maps.app.goo.gl/ZvpkfywUcq9BF8BE7"},
    {"Taj Mahal", 27.1751, 78.0421, LandmarkType::MONUMENT, "Asia", "https://cdn.britannica.com/86/170586-050-AB7FEFAE/Taj-Mahal-Agra-India.jpg", "https://maps.app.goo.gl/xfJPPJgEuWSUX6Sw6"},
    {"Cristo Redentor", -22.9519, -43.2105, LandmarkType::MONUMENT, "America", "https://blogger.googleusercontent.com/img/b/R29vZ2xl/AVvXsEh6QXwZ4FfSBArm4iTJLCsFi_MWb-vBId1rYyRK09rrZXGq_edDZ5swVEOov3JG77m009OXexronGQXuJhAfSZ_mXC4PshKh-rAcszS0WqmBa3lcIT5MrFkpHnYkXHRI0cNrENZyikctyk/s1600/cristo-redentor.jpg", "https://maps.app.goo.gl/t3v3jEFvV3Xus94D6"},
    {"Sagrada Familia", 41.4036, 2.1744, LandmarkType::MONUMENT, "Europa", "https://blog.centraldereservas.com/wp-content/uploads/2017/02/Sagrada-familia.-Barcelona-Fotolia_102733741_Subscription_Monthly_M.jpg", "https://maps.app.goo.gl/avcxtxZb7qn5o9mH6"},
    {"Estatua de la Libertad", 40.6892, -74.0445, LandmarkType::MONUMENT, "America", "https://springinstitute.org/wp-content/uploads/2019/11/statue-of-liberty.jpeg", "https://maps.app.goo.gl/YFkjXjXdSKRydGnSA"},
    {"Torre Eiffel", 48.8584, 2.2945, LandmarkType::MONUMENT, "Europa", "https://traveldigg.com/wp-content/uploads/2016/06/Eiffel-Tower-Paris-France-720x404.jpg", "https://maps.app.goo.gl/rxazUxbpbxd5vxGg9"},
    {"Cataratas del Niágara", 43.0782, -79.0757, LandmarkType::NATURALWONDER, "America", "https://cdn.holidayguru.es/wp-content/uploads/2016/07/Kanada_Niagara_Wasserfall_iStock-1058667536_900x600.jpg", "https://maps.app.goo.gl/hoPvQNP59kGr9LoUA"},
    {"Monte Kilimanjaro", -3.0674, 37.3556, LandmarkType::NATURALWONDER, "Africa", "https://s3.amazonaws.com/cdn.micato.com/wp-content/uploads/2018/09/07224331/mt-kilimanjaro-1110x700.jpg", "https://maps.app.goo.gl/6HiWVLDzTvZw9kKU7"},
    {"Monte Everest", 27.9881, 86.9250, LandmarkType::NATURALWONDER, "Asia", "https://upload.wikimedia.org/wikipedia/commons/b/bf/Mt._Everest_from_Gokyo_Ri_November_5%2C_2012_Cropped.jpg", "https://maps.app.goo.gl/HnZWH9eTDdZduVxM9"},
    {"Falla de San Andrés", 35.1166, -119.6817, LandmarkType::NATURALWONDER, "America", "https://www.debate.com.mx/__export/1727306313577/sites/debate/img/2024/09/25/falla-san-andres_1.jpg_1597830298.jpg", "https://maps.app.goo.gl/S7p9H1FhVkHSL5VH8"},
    {"Tokyo Tower", 35.6586, 139.7454, LandmarkType::MONUMENT, "Asia", "https://3.bp.blogspot.com/-khHMpahBpLQ/Wg4xeoOt16I/AAAAAAAAUc4/99dGUvaJkWYnNfHe3ZXBb84-lSbX8P2YACLcBGAs/s1600/IMG_9209.jpg", "https://maps.app.goo.gl/R5firw6qQYtM18Qh8"},
    {"Pirámides de Giza", 29.9792, 31.1342,LandmarkType::MONUMENT, "Africa", "https://images.ecestaticos.com/0g0WS6y0cAu8br39yucfVnRk9lM=/370x312:2271x1367/557x418/filters:fill(white):format(jpg)/f.elconfidencial.com%2Foriginal%2Fa5a%2Fc33%2Ff6b%2Fa5ac33f6b7b9aec9894e536c3741a5e8.jpg", "https://maps.app.goo.gl/TjeAh9GerRw3bzSp9"},
    {"Ayers Rock", -25.3444, 131.0369, LandmarkType::NATURALWONDER, "Oceania", "https://www.mostbeautifulspots.com/wp-content/uploads/2016/03/Ayers-Rock-Uluru.jpg", "https://maps.app.goo.gl/cWrVMECzWjaGrwXQA"}
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

        // translation script
        //

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