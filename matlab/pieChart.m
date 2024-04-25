% Define your ThingSpeak channel ID and read API key
channelID = 2506706;
readAPIKey = '50QMQKRPSTGYJ1UM';

% Fetch data from ThingSpeak
data = thingSpeakRead(channelID, 'NumPoints', 100, 'ReadKey', readAPIKey);
values = data(:, 6); % Assuming your data is in the first field

% Count occurrences of values 1 and 2
valueCounts = zeros(2, 1);
for i = 1:numel(values)
    if values(i) == 1
        valueCounts(1) = valueCounts(1) + 1;
    elseif values(i) == 2
        valueCounts(2) = valueCounts(2) + 1;
    end
end

names = ["Door 1","Door 2"];
p = piechart(valueCounts,names);
p.LabelStyle = "namedata";
title("Doorwise Entries")
