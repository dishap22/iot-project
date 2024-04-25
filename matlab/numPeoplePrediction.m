% Set your ThingSpeak channel ID and field ID
readChannelID = 2506706; 

% Set your Read API Key
readAPIKey = '50QMQKRPSTGYJ1UM';

% Specify the number of points to retrieve
NumPoints = 8000;

% Read data from ThingSpeak
[data, timestamps] = thingSpeakRead(readChannelID, 'NumPoints', NumPoints, 'ReadKey', readAPIKey);

% Extract the number of people
% num_people = data(:, 1);

filtered_timestamps = [];
filtered_num_people = [];

% Loop through all the data
for i = 1:size(data, 1)
    % Check if field1 is not empty
    if ~isempty(data(i, 1))
        % Append timestamp to filtered_timestamps
        filtered_timestamps = [filtered_timestamps; timestamps(i)];
        % Append field1 value to filtered_num_people
        filtered_num_people = [filtered_num_people; data(i, 1)];
    end
end

% Identify NaN values
nan_indices = isnan(filtered_num_people);

% Exclude NaN values from the timetable
filtered_timestamps_no_nan = filtered_timestamps(~nan_indices);
filtered_num_people_no_nan = filtered_num_people(~nan_indices);

% Create a timetable from the filtered data without NaN values
timeTableData_no_nan = timetable(filtered_timestamps_no_nan, filtered_num_people_no_nan);

% Display the timetable without NaN values
% disp(timeTableData_no_nan);

% Create a timetable from the data
% timeTableData = timetable(timestamps, num_people);

% Resample the data to hourly frequency and calculate the mean values within each hour
hourly_data = retime(timeTableData_no_nan, 'hourly', @mean);

% Extract hourly timestamps
hourly_timestamps = hourly_data.Properties.RowTimes;

% Create feature matrix X (timestamps)
X_train = (1:height(hourly_data))';

% Convert num_people to a column vector
y_train = hourly_data{:,:};
y_train = y_train(:);

% Polynomial features
degree = 7;
X_poly = zeros(length(X_train), degree+1);
for i = 1:degree+1
    X_poly(:, i) = X_train.^(i-1);
end

% Create and fit polynomial regression model
model_coeffs = regress(y_train, X_poly);

% Make predictions
predictions = X_poly * model_coeffs;

% Plot the original hourly average data
plot(hourly_timestamps, hourly_data{:,:}, 'o-', 'LineWidth', 1.5, 'DisplayName', 'Original Data');
hold on;

% Plot the predicted hourly average data
plot(hourly_timestamps, predictions, 'o-', 'LineWidth', 1.5, 'DisplayName', 'Predicted Data');

% Customize the plot
xlabel('Timestamp');
ylabel('Average Num People (hourly)');
title('Hourly Average Number of People');
legend('Location', 'best');
grid on;