import csv
import random
from datetime import datetime, timedelta, timezone

# Function to generate random values based on timestamp
def generate_random_value(timestamp):
    if timestamp < datetime(2024, 4, 15, 2, 0, tzinfo=timezone.utc):
        return random.randint(10, 30)
    elif timestamp < datetime(2024, 4, 15, 5, 0, tzinfo=timezone.utc):
        return random.randint(0, 25)
    elif timestamp < datetime(2024, 4, 15, 8, 0, tzinfo=timezone.utc):
        return random.randint(0, 10)
    else:
        return random.randint(10, 50)

# Function to adjust value within specified range
def adjust_value(value, min_value, max_value):
    new_value = value + random.choice([-1, 1])  # Change by either -1, 0, or 1
    return min(max(new_value, min_value), max_value)

# Function to generate timestamp with 5 minutes increment
def generate_timestamp(start_timestamp, minutes_increment):
    return start_timestamp + timedelta(minutes=minutes_increment)

# Start timestamp
start_timestamp = datetime.strptime("2024-04-15T00:00:00+00:00", "%Y-%m-%dT%H:%M:%S%z").replace(tzinfo=timezone.utc)

# Open CSV file for writing
with open('data2.csv', 'w', newline='') as csvfile:
    fieldnames = ['created_at', 'entry_id', 'field1', 'field2', 'field3', 'field4']
    writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
    
    writer.writeheader()
    
    # Generate and write rows of data
    current_timestamp = start_timestamp
    entry_id = 14
    current_value = None  # Initialize current value
    
    while current_timestamp < start_timestamp + timedelta(hours=24):
        if current_value is None:
            field1 = generate_random_value(current_timestamp)
        else:
            field1 = adjust_value(current_value, 10, 30) if current_timestamp < datetime(2024, 4, 15, 2, 0, tzinfo=timezone.utc) else \
                     adjust_value(current_value, 0, 25) if current_timestamp < datetime(2024, 4, 15, 5, 0, tzinfo=timezone.utc) else \
                     adjust_value(current_value, 0, 10) if current_timestamp < datetime(2024, 4, 15, 8, 0, tzinfo=timezone.utc) else \
                     adjust_value(current_value, 10, 50)
        
        writer.writerow({
            'created_at': current_timestamp.strftime("%Y-%m-%dT%H:%M:%S%z"),
            'entry_id': entry_id,
            'field1': field1,
            'field2': '',
            'field3': '',
            'field4': ''
        })
        
        current_timestamp += timedelta(minutes=5)
        entry_id += 1
        current_value = field1  # Update current value for next iteration

print("CSV file has been updated.")
