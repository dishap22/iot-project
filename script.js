window.onload = function() {
    // ThingSpeak channel details
    const channelID = 2506706;

    // Construct the ThingSpeak URL
    const url = `https://api.thingspeak.com/channels/2506706/fields/1/last.json`;

    // Make an HTTP GET request
    fetch(url)
        .then(response => response.json())
        .then(data => {
            // Extract the latest value from field 1
            const latestValue = data.field1;

            // Update the HTML with the latest value
            document.getElementById('latestValue').innerText = `Current Occupancy: ${latestValue}`;
        })
        .catch(error => console.error('Error fetching data:', error));
};
