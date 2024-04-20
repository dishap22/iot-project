import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression
from sklearn.metrics import mean_squared_error

# Load your sensor data into a pandas DataFrame
# Replace 'your_data.csv' with your actual data file
data = pd.read_csv('data1.csv', parse_dates=['created_at'], index_col='created_at')

# Extract the number of people from 'field1'
num_people = data['field1']

# Remove rows with missing values
num_people.dropna(inplace=True)

# Split data into training and testing sets
train_size = int(len(num_people) * 0.8)
train, test = num_people.iloc[:train_size], num_people.iloc[train_size:]

# Create feature matrix X (timestamps)
X_train = np.arange(len(train)).reshape(-1, 1)
X_test = np.arange(len(train), len(train) + len(test)).reshape(-1, 1)

# Convert num_people to a numpy array
y_train = train.values.reshape(-1, 1)
y_test = test.values.reshape(-1, 1)

# Create and fit linear regression model
model = LinearRegression()
model.fit(X_train, y_train)

# Make predictions
predictions = model.predict(X_test)

# Calculate RMSE (Root Mean Squared Error) to evaluate model performance
rmse = np.sqrt(mean_squared_error(y_test, predictions))
print(f'RMSE: {rmse}')

# Plot actual vs. predicted values
plt.plot(test.index, y_test, label='Actual')
plt.plot(test.index, predictions, label='Predicted')
plt.xlabel('Timestamp')
plt.ylabel('Number of People')
plt.title('Actual vs. Predicted Number of People (Linear Regression)')
plt.legend()
plt.show()
