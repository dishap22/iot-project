import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression
from sklearn.metrics import mean_squared_error

# Load your sensor data into a pandas DataFrame
# Replace 'your_data.csv' with your actual data file
data = pd.read_csv('data2.csv', parse_dates=['created_at'], index_col='created_at')

# Extract the number of people from 'field1'
num_people = data['field1']

# Resample the data to hourly frequency and sum the values within each hour
hourly_data = num_people.resample('h').sum().dropna()

# Create feature matrix X (timestamps)
X_train = np.arange(len(hourly_data)).reshape(-1, 1)

# Convert num_people to a numpy array
y_train = hourly_data.values.reshape(-1, 1)

# Create and fit linear regression model
model = LinearRegression()
model.fit(X_train, y_train)

# Make predictions
predictions = model.predict(X_train)

# Calculate RMSE (Root Mean Squared Error) to evaluate model performance
rmse = np.sqrt(mean_squared_error(y_train, predictions))
print(f'RMSE: {rmse}')

# Plot actual vs. predicted values
plt.plot(hourly_data.index.to_numpy(), y_train, label='Actual')
plt.plot(hourly_data.index.to_numpy(), predictions, label='Predicted')
plt.xlabel('Timestamp')
plt.ylabel('Num People (hourly)')
plt.title('Actual vs. Predicted Number of People (Linear Regression)')
plt.legend()
plt.show()
