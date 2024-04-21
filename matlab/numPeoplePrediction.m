% Set your ThingSpeak channel ID
readChannelID = 2506706; 

% Specify the number of points to retrieve
NumPoints = 288;

% Read data from ThingSpeak
[data, timestamps] = thingSpeakRead(readChannelID, 'NumPoints', NumPoints);

% Extract the number of people
num_people = data(:, 1);

% Create a timetable from the data
timeTableData = timetable(timestamps, num_people);

% Resample the data to hourly frequency and calculate the mean values within each hour
hourly_data = retime(timeTableData, 'hourly', @mean);

% Extract hourly timestamps
hourly_timestamps = hourly_data.Properties.RowTimes;

% Create feature matrix X (timestamps)
X_train = (1:height(hourly_data))';

% Convert num_people to a column vector
y_train = hourly_data{:,:};
y_train = y_train(:);

% Polynomial features
degree = 3;
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
