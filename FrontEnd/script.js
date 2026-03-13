document.addEventListener("DOMContentLoaded", function()
{
    const map = L.map('map').setView([41.38, 2.17], 13);

    L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png').addTo(map);

    let ClickMarker;

    map.on('click', async function(e)
    {
        const { lat, lng } = e.latlng;

        if (ClickMarker)
        {
            map.removeLayer(ClickMarker);
        }

        ClickMarker = L.marker([lat, lng]).addTo(map)
            .bindPopup("Calculating...")
            .openPopup();

        try
        {
            const response = await fetch('http://localhost:18080/calculate',
            {
                method: 'POST',
                body: JSON.stringify({ lat: lat, lon: lng })
            });

            const data = await response.json();

            const ResultsContainer = document.getElementById('results');
            ResultsContainer.innerHTML = '';

            data.forEach(landmark => {
                const div = document.createElement('div');
                div.className = 'result-item';
                div.innerHTML = `
                    <b>${landmark.name}</b>: ${landmark.distance.toFixed(2)} km
                    <button onclick="map.flyTo([${landmark.lat}, ${landmark.lon}], 15)">Ir</button>`;
                ResultsContainer.appendChild(div);

                L.marker([landmark.lat, landmark.lon]).addTo(map)
                    .bindPopup(`<b>${landmark.name}</b><br>Distancia: ${landmark.distance.toFixed(2)} km`)
                    .openPopup();
            });                 
        }
        catch (error)
        {
            console.error("Error connecting to C++ server:", error);
            ClickMarker.setPopupContent("Server Error! Check C++ terminal.").openPopup();
        }
    });
});