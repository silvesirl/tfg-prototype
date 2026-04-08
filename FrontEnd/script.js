let Map;
let MetricDistance = "Kilometers";

document.addEventListener("DOMContentLoaded", function()
{
    Map = L.map('map').setView([41.38, 2.17], 13);
    const SwitchKilometersButton = document.getElementById("ChangeKilometers");
    const SwitchMilesButton = document.getElementById("ChangeMiles");
    const SwitchFootButton = document.getElementById("ChangeFoot");

    L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png').addTo(Map);

    let ClickMarker;

    SwitchKilometersButton.addEventListener("click", async function()
    {
        const Response = await fetch('http://localhost:18080/changekm',
        {
            method: 'POST'
        });

        const Data = await Response.json();

        MetricDistance = Data["metric"];
    });

    SwitchMilesButton.addEventListener("click", async function()
    {
        const Response = await fetch('http://localhost:18080/changemiles',
        {
            method: 'POST'
        });

        const Data = await Response.json();

        MetricDistance = Data["metric"];
    });


    SwitchFootButton.addEventListener("click", async function()
    {
        const Response = await fetch('http://localhost:18080/changefoot',
        {
            method: 'POST'
        });

        const Data = await Response.json();

        MetricDistance = Data["metric"];
    });

    Map.on("click", async function(e)
    {
        const { lat, lng } = e.latlng;

        if (ClickMarker)
        {
            Map.removeLayer(ClickMarker);
        }

        ClickMarker = L.marker([lat, lng]).addTo(Map)
            //.bindPopup("Calculating...")
            //.openPopup();

        try
        {
            const Response = await fetch('http://localhost:18080/calculate',
            {
                method: 'POST',
                body: JSON.stringify({ lat: lat, lon: lng })
            });

            const Data = await Response.json();

            const ResultsContainer = document.getElementById('results');
            ResultsContainer.innerHTML = '';

            Data.forEach(Landmark => {
                const Div = document.createElement('div');
                Div.className = 'result-item';
                Div.innerHTML = `
                    <b>${Landmark.name}</b>: ${Landmark.distance.toFixed(2)} ${MetricDistance}
                    <button onclick="Map.flyTo([${Landmark.lat}, ${Landmark.lon}], 15)">Ir</button>`;
                ResultsContainer.appendChild(Div);

                L.marker([Landmark.lat, Landmark.lon]).addTo(Map)
                .bindPopup(`<b>${Landmark.name}</b><br>Distancia: ${Landmark.distance.toFixed(2)}${MetricDistance}`);
            });                 
        }
        catch (error)
        {
            console.error("Error connecting to C++ server:", error);
            ClickMarker.setPopupContent("Server Error! Check C++ terminal.").openPopup();
        }
    });
});