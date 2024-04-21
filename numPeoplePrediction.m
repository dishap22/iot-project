readChannelID = 2468890; 
TemperatureFieldID = 1;

% Set your Read API Key
readAPIKey = 'G5OXHTW582CD6TKW';

% Specify the number of points to retrieve
NumPoints = 288;

% Read data from ThingSpeak
[data,timestamps] = thingSpeakRead(readChannelID, 'NumPoints', NumPoints, 'ReadKey', readAPIKey);

num_people = data(:, 1);

% Create a timetable from the data
timeTableData = timetable(timestamps, num_people);

% Resample the data to hourly frequency and calculate the mean values within each hour
hourly_data = retime(timeTableData, 'hourly', @mean);

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
hourly_timestamps = timestamps(1:12:end); % Assuming there are 12 data points per hour

plot(hourly_timestamps, predictions);