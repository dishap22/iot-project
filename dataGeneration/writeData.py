import csv
import random
from datetime import datetime, timedelta

# Function to generate random values between 0 and 100
def generate_random_value():
    return random.randint(0, 100)

# Function to generate timestamp with 5 minutes increment
def generate_timestamp(start_timestamp, minutes_increment):
    return start_timestamp + timedelta(minutes=minutes_increment)

# Start timestamp
start_timestamp = datetime.strptime("2024-04-15T00:00:00+00:00", "%Y-%m-%dT%H:%M:%S%z")

# Open CSV file for writing
with open('<add a .csv file>', 'w', newline='') as csvfile:
    fieldnames = ['created_at', 'entry_id', 'field1', 'field2', 'field3', 'field4']
    writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
    
    writer.writeheader()
    
    # Generate and write 1000 rows of data
    for i in range(1000):
        timestamp = generate_timestamp(start_timestamp, i*5)
        entry_id = 14 + i
        field1 = generate_random_value()
        
        writer.writerow({
            'created_at': timestamp.strftime("%Y-%m-%dT%H:%M:%S%z"),
            'entry_id': entry_id,
            'field1': field1,
            'field2': '',
            'field3': '',
            'field4': ''
        })

print("CSV file has been updated.")
