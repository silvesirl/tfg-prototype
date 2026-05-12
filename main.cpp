#include <iostream>
#include <cmath>

#include "Landmark.h"
#include "DistanceMetric.h"
#include "Utils/json.hpp"
#include "httplib.h"
#include "Utils/GlobalConstants.h"

#include "LandmarkController.h"
#include "LandmarkService.h"
#include "ILandmarkDBRepository.h"
#include "SQLiteLandmarkRepository.h"

using JsonParser = nlohmann::json;

std::vector<Landmark> LandmarkList;

std::unique_ptr<ILandmarkDBRepository> DBRepository;

int main()
{
    httplib::Server Server;

    DBRepository = std::unique_ptr<ILandmarkDBRepository>(new SQLiteLandmarkRepository());

    auto LService = std::make_shared<LandmarkService>(*DBRepository);

    LandmarkController LController(Server, LService);

    LController.RegisterRoutes();

    std::cout << "Servidor escuchando en http://localhost:18080..." << std::endl;
    Server.listen("0.0.0.0", SERVER_PORT);

    return 0;
}