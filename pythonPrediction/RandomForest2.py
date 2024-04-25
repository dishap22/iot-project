import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.ensemble import RandomForestRegressor
from sklearn.metrics import mean_squared_error
import matplotlib.pyplot as plt
import numpy as np

# Load your sensor data into a pandas DataFrame
data = pd.read_csv('randomValues.csv', parse_dates=['created_at'], index_col='created_at')

# Extract the number of people from 'field1'
num_people = data['field1']

# Resample the data to hourly frequency and calculate the mean values within each hour
hourly_data = num_people.resample('h').mean().dropna()

# Create feature matrix X (timestamps)
X = np.arange(len(hourly_data)).reshape(-1, 1)

# Convert num_people to a numpy array
y = hourly_data.values.reshape(-1, 1)

# Split data into training and test sets (70% training, 30% test)
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.5, random_state=42)

# Create the Random Forest model (Regressor)
model = RandomForestRegressor(n_estimators=100, random_state=42)

# Train the model
model.fit(X_train, y_train.ravel())

# Make predictions on the test set
y_pred = model.predict(X_test)

# Evaluate model performance using MSE
mse = mean_squared_error(y_test, y_pred)
print("Mean Squared Error:", mse)

# Convert timestamps in hourly_data_index to a more readable format (e.g., datetime objects)
hourly_data_datetime = hourly_data.index.to_pydatetime()

# Separate timestamps for test data based on X_test indices
test_timestamps = hourly_data_datetime[X_test.ravel()]

# Sort together based on timestamps
sorted_data = pd.DataFrame({'timestamp': test_timestamps, 'actual': y_test.ravel(), 'predicted': y_pred})
sorted_data = sorted_data.sort_values(by='timestamp')

# Extract sorted data
sorted_timestamps = sorted_data['timestamp'].to_numpy()
sorted_actual = sorted_data['actual'].to_numpy().reshape(-1, 1)
sorted_predicted = sorted_data['predicted'].to_numpy().reshape(-1, 1)

plt.plot(sorted_timestamps, sorted_actual, label='Actual Test Data', color='blue')
plt.plot(sorted_timestamps, sorted_predicted, label='Predicted Test Data', color='orange')

# # Plot actual test data and predicted data as separate points
# plt.figure(figsize=(10, 6))
# plt.scatter(test_timestamps, y_test, label='Actual Test Data', color='blue')
# plt.scatter(test_timestamps, y_pred, label='Predicted Test Data', color='orange')

# Add titles and labels
plt.title('Actual vs. Predicted Average Number of People (Random Forest)')
plt.xlabel('Time (Datetime)')
plt.ylabel('Average Num People (hourly)')

# Add legend
plt.legend()

# Rotate x-axis labels for better readability if needed (consider plt.xticks(rotation=45))
plt.tight_layout()  # Adjust spacing between elements
plt.show()