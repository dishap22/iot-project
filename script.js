window.onload = function() {
    // Define a function to fetch and update the latest value from ThingSpeak every second
    const updateLatestValue = () => {
        // ThingSpeak channel details
        const channelID = 2506706;

        // Construct the ThingSpeak URL
        const url = `https://api.thingspeak.com/channels/${channelID}/fields/1/last.json`;

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

    // Call the function to update the latest value every second
    setInterval(updateLatestValue, 1000);
};
