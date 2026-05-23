# Geospatial Landmark API 🗺️

This project consists of a high-performance **Geospatial Web API** that calculates the physical distance between a user-defined location and famous natural wonders or man-made monuments around the world. 

The application offers real-time filtering by geographical continent and landmark type (monument or natural), seamless metric unit conversion (Kilometers, Miles, and Feet), user geolocation integration, and a fully dynamic multi-language localization system.

---

## 🛠️ Tech Stack & Frameworks

* **Backend:** C++20
* **Build System:** CMake
* **HTTP Server:** cpp-httplib
* **Database:** SQLite3
* **Frontend:** Vanilla HTML5, CSS3 and JavaScript
* **Benchmarking:** Python 3

---

## 🚀 Architectural Design & Performance Features

* **Hexagonal Architecture (Ports & Adapters):** Designed with a strict separation of concerns. The core business logic (`Domain` and `Services`) is completely decoupled from external infrastructure layers, framework details, and driving/driven actors (`Controllers` and `Repositories`).
* **In-Memory Cache Filtering:** To minimize disk I/O bottlenecks during runtime, the application implements a database dump pattern. At startup, the repository loads all data into memory using contiguous structures (`std::vector`). Geographic filtering is resolved instantly in RAM, achieving microsecond-level computation times.
* **Modern C++ Optimizations:** Heavy utilization of Modern C++ paradigms, including:
  * Efficient resource management via **RAII** principles.
  * Minimize deep copies using move semantics (`std::move`).
  * Heap allocation optimization using pre-reserved capacity (`.reserve()`) to eliminate dynamic vector reallocations during runtime.
* **Robust & Consistent Frontend:** Asynchronous, responsive UI featuring an interactive Leaflet map. It includes an optimized geolocation handler with cached positioning and high-accuracy bypass to prevent browser delays, alongside an explicit i18n lifecycle (`autocomplete="off"` constraints) to maintain multi-language data consistency across browser refreshes (F5).

---

## 📁 Project Structure

```text
├── DB/                         # Local database container (landmark.DB)
├── Frontend/
│   ├── index.html              # Main Dashboard UI
│   ├── script.js               # Leaflet map and Asynchronous Fetch API
│   └── style.css               # Visual styles
├── src/
│   ├── Core/                   # Pure business logic (Hexagon Core)
│   │   ├── Application/        # Application services (e.g., LandmarkService orchestration)
│   │   ├── Domain/             # Domain entities, constants, and core business models
│   │   └── Ports/              # Inbound/Outbound interfaces (API & SPI definitions)
│   └── Infrastructure/         # External adapters and framework-specific code
│       ├── Localization/       # Localization adapters and language logic
│       └── Persistence/        # Database management
│           ├── Api/            # DB controllers and endpoint handlers
│           └── Mappers/        # Data translation layer between infraestructure and Domain
├── Testing/
│   └── TestRequest.py          # Python benchmark script for stress testing
├── CMakeLists.txt              # Build system configuration
├── Localization.xml            # Shared multi-language dictionary (EN, ES, ITA, FR, DE, CH, JP, KR)
├── main.cpp                    # Application entry point & service bootstrapping
└── Third-party                 # Libraries
```

## 🏃‍♂️ Requisites for opening the project

* **A compiler fully compatible with C++20 (GCC, Clang, or MSVC).**
* **Python 3 installed**
* **Hosting solution**

## 💡 Recommended Development Setup

For the best development and evaluation experience, we recommend using **Visual Studio Code** along with the following extensions:
* **C/C++** (by Microsoft) — For syntax highlighting and CMake integration.
* **Python** — To execute the automated performance scripts.
* **Live Server** — To easily host the web dashboard with a single click.

### 🛠️ Execution Walkthrough

1. Open the root directory of the project in Visual Studio Code.
2. Build and run the `geospatial_api` target executable to start the C++ backend server.
3. Launch the **Live Server** extension from the status bar or the command palette.
4. A browser window will automatically open showing the project's root directory. Click on the `frontend/` folder.
5. The interactive Geospatial Dashboard will load, and you can begin interacting with the map, switching metrics, and filtering landmarks.
6. *(Optional)* With the backend running and the frontend open, you can execute the Python stress test script (`TestRequest.py`) to observe concurrent request latencies in real time.
