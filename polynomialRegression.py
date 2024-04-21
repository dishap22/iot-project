import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.preprocessing import PolynomialFeatures
from sklearn.linear_model import LinearRegression
from sklearn.metrics import mean_squared_error

# Load your sensor data into a pandas DataFrame
# Replace 'your_data.csv' with your actual data file
data = pd.read_csv('data2.csv', parse_dates=['created_at'], index_col='created_at')

# Extract the number of people from 'field1'
num_people = data['field1']

# Resample the data to hourly frequency and calculate the mean values within each hour
hourly_data = num_people.resample('h').mean().dropna()

# Create feature matrix X (timestamps)
X_train = np.arange(len(hourly_data)).reshape(-1, 1)

# Convert num_people to a numpy array
y_train = hourly_data.values.reshape(-1, 1)

# Polynomial features
poly_features = PolynomialFeatures(degree=3)  # You can adjust the degree as needed
X_poly = poly_features.fit_transform(X_train)

# Create and fit polynomial regression model
model = LinearRegression()
model.fit(X_poly, y_train)

# Make predictions
predictions = model.predict(X_poly)

# Calculate RMSE (Root Mean Squared Error) to evaluate model performance
rmse = np.sqrt(mean_squared_error(y_train, predictions))
print(f'RMSE: {rmse}')

# Plot actual vs. predicted values
plt.plot(hourly_data.index.to_numpy(), y_train, label='Actual')
plt.plot(hourly_data.index.to_numpy(), predictions, label='Predicted')
plt.xlabel('Timestamp')
plt.ylabel('Average Num People (hourly)')
plt.title('Actual vs. Predicted Average Number of People (Polynomial Regression)')
plt.legend()
plt.show()
