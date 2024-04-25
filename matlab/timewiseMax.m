% Enter your MATLAB code below

% Enter your MATLAB code below
% Define channel ID and field number
channelID = 2506706; % Replace with your ThingSpeak channel ID
fieldNumber = 1; % Replace with your field number (e.g., 1, 2, 3, etc.)

% Define a large date range for data retrieval
startDate = '2024-04-01'; % Start date in 'YYYY-MM-DD' format (adjust as needed)
endDate = datetime('now'); % End date is current date

% Read data from the channel
[data, timestamps] = thingSpeakRead(channelID, 'Fields', fieldNumber, 'DateRange', [startDate, endDate], ReadKey='50QMQKRPSTGYJ1UM');

% Convert timestamps to datetime format
timestamps = datetime(timestamps, 'InputFormat', 'yyyy-MM-dd HH:mm:ss');

% Calculate the hour of the day for each timestamp
hours = hour(timestamps);

% Find the unique days in the data
[uniqueDates, ~, dateIndices] = unique(dateshift(timestamps, 'start', 'day'));

% Initialize an array to store the count of days each hour was most populated
mostPopulatedHours = zeros(24, 1);

% Loop through each unique day
for i = 1:length(uniqueDates)
    % Get indices of data points for the current day
    dayIndices = dateIndices == i;
    
    % Get the data and hours for the current day
    dayData = data(dayIndices);
    dayHours = hours(dayIndices);
    
    % Find the hour with the maximum occupancy
    [~, maxHourIndex] = max(dayData);
    maxHour = dayHours(maxHourIndex);
    
    % Increment the count for the most populated hour
    mostPopulatedHours(maxHour + 1) = mostPopulatedHours(maxHour + 1) + 1;
end

% Plot a bar graph of the most populated hours
figure;
bar(mostPopulatedHours);
xlabel('Hour of the Day');
ylabel('Number of Days Most Populated');
title('Hours with Highest Occupancy per Day');
xlim([0, 24]);
xticks(0:23);
xticklabels(arrayfun(@num2str, 0:23, 'UniformOutput', false));
grid on;

% Optional: Add customization
set(gca, 'FontSize', 10); % Set font size of the axes