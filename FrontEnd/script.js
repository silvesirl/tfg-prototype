let Map;
let MetricDistance = "Kilometers";
let LastLat = null;
let LastLon = null;

let ClickMarker = null

var RedIcon = new L.Icon({
    iconUrl: 'https://raw.githubusercontent.com/pointhi/leaflet-color-markers/master/img/marker-icon-2x-red.png',
    shadowUrl: 'https://cdnjs.cloudflare.com/ajax/libs/leaflet/0.7.7/images/marker-shadow.png',
    iconSize: [25, 41],
    iconAnchor: [12, 41],
    popupAnchor: [1, -34],
    shadowSize: [41, 41]
});

document.addEventListener("DOMContentLoaded", function()
{
    Map = L.map('map').setView([41.38, 2.17], 13);

    const SwitchKilometersButton = document.getElementById("ChangeKilometers");
    const SwitchMilesButton = document.getElementById("ChangeMiles");
    const SwitchFootButton = document.getElementById("ChangeFoot");
    const ContinentDropDown = document.getElementById("Continente");
    const CategoryDropDown = document.getElementById("Categoria");
    const CurrentLocationButton = document.getElementById("CurrentLocationButton");
    const LanguageDropDown = document.getElementById("Language");

    function UpdateActiveButton(aActiveButton)
    {
        [SwitchKilometersButton, SwitchMilesButton, SwitchFootButton].forEach(Button => {
            Button.classList.remove('active');
        });

        aActiveButton.classList.add('active');
    }

    async function CalculateDistance(aLat, aLon)
    {
        try
        {
            const Response = await fetch('http://localhost:18080/calculate',
            {
                method: 'POST',
                body: JSON.stringify({ lat: aLat, lon: aLon })
            });

            const Data = await Response.json();

            const ResultsContainer = document.getElementById('results');
            ResultsContainer.innerHTML = '';

            Map.eachLayer((layer) =>
            {
                if (layer instanceof L.Marker || layer instanceof L.Polyline)
                {
                    if (layer !== ClickMarker)
                    {
                        Map.removeLayer(layer);
                    }
                }
            });

            Data.forEach(Landmark => {
                const Div = document.createElement('div');
                Div.className = 'result-item';
                Div.innerHTML = `
                    <b>${Landmark.name}</b>: ${Landmark.distance.toFixed(2)} ${MetricDistance}
                    <button onclick="Map.flyTo([${Landmark.lat}, ${Landmark.lon}], 15)">Ir</button>`;
                ResultsContainer.appendChild(Div);

                L.marker([Landmark.lat, Landmark.lon]).addTo(Map)
                .bindPopup(`
                    <b>${Landmark.name}</b><br>
                    Distancia: ${Landmark.distance.toFixed(2)} ${MetricDistance}<br>
                    Continente: ${Landmark.continent}<br>
                    <a href="${Landmark.maplink}" target="_blank">Link</a><br>
                    <img src="${Landmark.imageurl}" width="200" style="margin-top: 5px;">
                `);
            });                 
        }
        catch (error)
        {
            console.error("Error connecting to C++ server:", error);
            ClickMarker.setPopupContent("Server Error! Check C++ terminal.").openPopup();
        }
    }

    L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png').addTo(Map);

    SwitchKilometersButton.addEventListener("click", async function()
    {
        UpdateActiveButton(this);

        const Response = await fetch('http://localhost:18080/changekm',
        {
            method: 'POST'
        });

        const Data = await Response.json();

        MetricDistance = Data["metric"];

        if(LastLat != null && LastLon != null)
        {
            CalculateDistance(LastLat, LastLon);
        }
    });

    SwitchMilesButton.addEventListener("click", async function()
    {
        UpdateActiveButton(this);

        const Response = await fetch('http://localhost:18080/changemiles',
        {
            method: 'POST'
        });

        const Data = await Response.json();

        MetricDistance = Data["metric"];

        if(LastLat != null && LastLon != null)
        {
            CalculateDistance(LastLat, LastLon);
        }
    });

    SwitchFootButton.addEventListener("click", async function()
    {
        UpdateActiveButton(this);

        const Response = await fetch('http://localhost:18080/changefoot',
        {
            method: 'POST'
        });

        const Data = await Response.json();

        MetricDistance = Data["metric"];

        if(LastLat != null && LastLon != null)
        {
            CalculateDistance(LastLat, LastLon);
        }
    });

    ContinentDropDown.addEventListener("change", async function()
    {
        let FilteredContinent = this.value;

        const Response = await fetch('http://localhost:18080/changecontinent',
        {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({continent: FilteredContinent})
        });

        const Data = await Response.json();

        if(LastLat != null && LastLon != null)
        {
            CalculateDistance(LastLat, LastLon);
        }
    });

    CategoryDropDown.addEventListener("change", async function()
    {
        let FilteredCategory = this.value;

        const Response = await fetch('http://localhost:18080/changecategory',
        {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({category: FilteredCategory})
        });

        const Data = await Response.json();

        if(LastLat != null && LastLon != null)
        {
            CalculateDistance(LastLat, LastLon);
        }
    });

    LanguageDropDown.addEventListener("change", async function()
    {
        let LanguageChosen = this.value;

        const Response = await fetch('http://localhost:18080/changelanguage',
        {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({language: LanguageChosen})
        });

        const Data = await Response.json();
    });

    CurrentLocationButton.addEventListener("click", async function()
    {
        if (!navigator.geolocation)
        {
            alert("Geolocalizacion no soportada por el navegador");
            return;
        }

        navigator.geolocation.getCurrentPosition((position) => {
            const lat = position.coords.latitude;
            const lon = position.coords.longitude;

            LastLat = lat;
            LastLon = lon;

            Map.flyTo([lat, lon], 14);

            if (ClickMarker) Map.removeLayer(ClickMarker);
            ClickMarker = L.marker([lat, lon],{icon: RedIcon}).addTo(Map)
                .bindPopup("Ubicación actual")
                .openPopup();

            CalculateDistance(lat, lon);
        },
        (error) => {
            console.error("Error obteniendo ubicación:", error);
            alert("No se pudo obtener tu ubicación. Revisa los permisos de tu navegador.");
        });
    });

    Map.on("click", async function(e)
    {
        const { lat, lng } = e.latlng;

        LastLat = e.latlng.lat;
        LastLon = e.latlng.lng;

        if (ClickMarker)
        {
            Map.removeLayer(ClickMarker);
        }

        ClickMarker = L.marker([lat, lng],{icon: RedIcon}).addTo(Map)
            .bindPopup("Ubicación actual")
            .openPopup();

        CalculateDistance(lat, lng);
    });
});