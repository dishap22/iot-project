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
    if new_value == min_value:
        return new_value + 1
    if new_value == max_value:
        return new_value - 1
    return new_value

def generate_timestamp(start_timestamp, minutes_increment):
    return start_timestamp + timedelta(minutes=minutes_increment)

# Function to generate number of entries based on timestamp
def generate_num_entries(timestamp):
    if timestamp < datetime(2024, 4, timestamp.day, 2, 0, tzinfo=timezone.utc):
        return random.randint(7, 12)
    elif timestamp < datetime(2024, 4, 15, 4, 0, tzinfo=timezone.utc):
        return random.randint(1, 5)
    elif timestamp < datetime(2024, 4, 15, 7, 0, tzinfo=timezone.utc):
        return random.randint(7, 12)
    elif timestamp < datetime(2024, 4, 15, 9, 0, tzinfo=timezone.utc):
        return random.randint(10, 15)
    elif timestamp < datetime(2024, 4, 15, 3, 0, tzinfo=timezone.utc):
        return random.randint(5, 15)
    elif timestamp < datetime(2024, 4, 15, 9, 0, tzinfo=timezone.utc):
        return random.randint(7, 15)
    else:
        return random.randint(8, 13)

def generate_unique_timestamps(start_timestamp, end_timestamp, num_timestamps):
    timestamps = set()
    while len(timestamps) < num_timestamps:
        random_minutes = random.randint(1, int((end_timestamp - start_timestamp).total_seconds() / 60))
        timestamps.add(start_timestamp + timedelta(minutes=random_minutes))
    return sorted(timestamps)

# Start timestamp
start_timestamp = datetime.strptime("2024-04-05T00:00:00+00:00", "%Y-%m-%dT%H:%M:%S%z").replace(tzinfo=timezone.utc)

# Open CSV file for writing
with open('data2.csv', 'w', newline='') as csvfile:
    fieldnames = ['created_at', 'entry_id', 'field1', 'field2', 'field3', 'field4']
    writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
    
    writer.writeheader()
    
    # Generate and write rows of data
    current_timestamp = start_timestamp
    entry_id = 14
    current_value = generate_random_value(current_timestamp)
    
    while current_timestamp < start_timestamp + timedelta(days=14):  # Generate entries for a week
        num_entries = generate_num_entries(current_timestamp)
        time_stamps = generate_unique_timestamps(current_timestamp, current_timestamp + timedelta(hours=1), num_entries)
        number_of_people_entering = random.randint(int(2 * num_entries / 3), num_entries)
        number_of_people_leaving = num_entries - number_of_people_entering
        changes = [1] * number_of_people_entering + [-1] * number_of_people_leaving + [1]
        random.shuffle(changes)
        for i in range(num_entries):
            field1 = current_value + changes[i]
            if field1 < 5 and changes[i + 1] == -1:
                changes[i + 1] = random.choice([1, 1, 1, -1])
            elif field1 > 25 and changes[i + 1] == 1:
                changes[i + 1] = random.choice([1, -1, -1, -1])
            elif field1 >= 5 and field1 <= 12 and changes[i + 1] == -1:
                changes[i + 1] = random.choice([1, 1, -1])
            elif field1 >= 18 and field1 <= 25 and changes[i + 1] == 1:
                changes[i + 1] = random.choice([1, -1, -1])
            else:
                temp = changes[i + 1]
                temp_list = [temp] * 2 + [temp * -1] * 4
                changes[i + 1] = random.choice([1, -1])
            if field1 < 0:
                field1 = 1
            if field1 > 30:
                field1 = 29
            current_value = field1
            writer.writerow({
                'created_at': time_stamps[i].strftime("%Y-%m-%dT%H:%M:%S%z"),
                'entry_id': entry_id,
                'field1': field1,
                'field2': '',
                'field3': '',
                'field4': ''
            })
            entry_id += 1
        
        current_timestamp += timedelta(hours=1)

print("CSV file has been updated.")
