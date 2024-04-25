import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression, ElasticNet, Ridge, Lasso
from sklearn.metrics import mean_squared_error
from sklearn.preprocessing import PolynomialFeatures
import random
from datetime import datetime, timedelta
import matplotlib.pyplot as plt
import json
import csv
import updateData

def load_and_preprocess_data(data_list):
    data = pd.DataFrame(data_list)
    data['created_at'] = pd.to_datetime(data['created_at'])
    data.set_index('created_at', inplace=True)
    num_people = pd.to_numeric(data['field1'], errors='coerce')
    hourly_data = num_people.resample('h').mean().dropna()
    return hourly_data

def csv_to_json(csv_file, json_file):
    # Open the CSV file
    with open(csv_file, 'r') as csv_file:
        # Read the CSV file into a dictionary
        csv_reader = csv.DictReader(csv_file)
        
        # Convert the CSV data into JSON format
        json_data = json.dumps(list(csv_reader), indent=4)
        
        # Write the JSON data to a file
        with open(json_file, 'w') as json_file:
            json_file.write(json_data)

# updateData.get_data('data3.csv')

csv_to_json('data2.csv', 'data.json')

# Load data from data.json
json_data = None
with open('data.json', 'r') as file:
    json_data = json.load(file)

hourly_data = load_and_preprocess_data(json_data)

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
    print(f'Training samples: {len(X_train)}')
    print(f'Testing samples: {len(X_test)}')

degrees = []
for i in range(1, 20):
    poly_features = PolynomialFeatures(degree=i)
    X_train_poly = poly_features.fit_transform(X_train)
    X_test_poly = poly_features.transform(X_test)
    X_poly = poly_features.fit_transform(X)
    model = LinearRegression()
    model.fit(X_train_poly,y_train)

    test_predictions = model.predict(X_poly)

    test_predictions = np.round(test_predictions).astype(int)

    test_rmse = np.sqrt(mean_squared_error(y, test_predictions))

    degrees.append((i, test_rmse))
    
# Sorting by RMSE
degrees = sorted(degrees, key=lambda x: x[1])
for degree, rmse in degrees:
    print(f'Degree: {degree}, RMSE: {rmse}')

poly_features = PolynomialFeatures(degree=6)
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

# # Get time of first entry
# first_entry_time = hourly_data.index[0]
# print(f'Time of first entry: {first_entry_time}')

# # Get dictionary of timestamps and number of people
# timestamps = hourly_data.index
# num_people = hourly_data.values.flatten()
# data_dict = dict(zip(timestamps, num_people))
# print(f'Dictionary of timestamps and number of people: {data_dict}')

# # Get max number of entries in each day
# max_entries = hourly_data.resample('D').max()
# print(f'Max number of entries in each day: {max_entries}')

