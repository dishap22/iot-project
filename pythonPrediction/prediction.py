import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression, ElasticNet, Ridge, Lasso
from sklearn.metrics import mean_squared_error
from sklearn.preprocessing import PolynomialFeatures
import random
from datetime import datetime, timedelta
import matplotlib.pyplot as plt

def load_and_preprocess_data(data_list):
    data = pd.DataFrame(data_list)
    data['created_at'] = pd.to_datetime(data['created_at'])
    data.set_index('created_at', inplace=True)
    num_people = pd.to_numeric(data['field1'], errors='coerce')
    hourly_data = num_people.resample('h').mean().dropna()
    return hourly_data

hourly_data = load_and_preprocess_data(data_list)

X = np.arange(len(hourly_data)).reshape(-1, 1)
y = hourly_data.values.reshape(-1, 1)

num_samples = len(hourly_data)
if num_samples == 1:
    print("Only one sample available. Using all data for training.")
    X_train, y_train = X, y
    X_test, y_test = X, y
elif num_samples == 2:
    print("Only two samples available. Using all data for training.")
    X_train, y_train = X, y
    X_test, y_test = X, y
else:

    from sklearn.model_selection import train_test_split
    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.5, random_state=0)


poly_features = PolynomialFeatures(degree=10)
X_train_poly = poly_features.fit_transform(X_train)
X_test_poly = poly_features.transform(X_test)
X_poly = poly_features.fit_transform(X)
model = LinearRegression()
model.fit(X_train_poly,y_train)

test_predictions = model.predict(X_poly)

test_predictions = np.round(test_predictions).astype(int)

test_rmse = np.sqrt(mean_squared_error(y, test_predictions))

print(f'Testing RMSE: {test_rmse}')
plt.figure(figsize=(10, 6))
plt.plot(hourly_data.index, y, label='Actual')
plt.plot(hourly_data.index[:], test_predictions, label='Test Predictions')
plt.xlabel('Timestamp')
plt.ylabel('Average Num People (hourly)')
plt.title('Actual vs. Predicted Average Number of People (Polynomial Regression)')
plt.legend()
plt.grid(True)
plt.show()