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

def generate_timestamp_seconds_increment(start_timestamp, increment):
    return start_timestamp + timedelta(seconds=increment)

# Function to generate number of entries based on timestamp
def generate_num_entries(timestamp):
    if timestamp < datetime(2024, 4, 15, 2, 0, tzinfo=timezone.utc):
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
start_timestamp = datetime.strptime("2024-04-15T00:00:00+00:00", "%Y-%m-%dT%H:%M:%S%z").replace(tzinfo=timezone.utc)

# Open CSV file for writing
with open('data2.csv', 'w', newline='') as csvfile:
    fieldnames = ['created_at', 'entry_id', 'field1', 'field2', 'field3', 'field4', 'field5', 'field6', 'latitude', 'longitude', 'elevation', 'status']
    writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
    
    writer.writeheader()
    
    # Generate and write rows of data
    current_timestamp = start_timestamp
    entry_id = 1
    current_value = generate_random_value(current_timestamp)
    
    while current_timestamp < start_timestamp + timedelta(days=14):
        num_entries = generate_num_entries(current_timestamp);
        time_stamps = generate_unique_timestamps(current_timestamp, current_timestamp + timedelta(hours=1), num_entries)
        number_of_people_entering = random.randint(1, num_entries)
        number_of_people_leaving = num_entries - number_of_people_entering
        changes = [1] * number_of_people_entering + [-1] * number_of_people_leaving + [1]
        random.shuffle(changes)
        entries = 0
        exits = 0
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
                temp_list = [temp] * 3 + [temp * -1] * 2
                changes[i + 1] = random.choice([1, -1])
            if field1 < 0:
                field1 = 1
                changes[i] = 1
            if field1 > 30:
                field1 = 29
                changes[i] = -1
            current_value = field1
            current_timestamp = time_stamps[i]
            writer.writerow({
                'created_at': time_stamps[i].strftime("%Y-%m-%dT%H:%M:%S%z"),
                'entry_id': entry_id,
                'field1': field1,
                'field2': '',
                'field3': '',
                'field4': '',
                'field5': '',
                'field6': '',
                'latitude': '',
                'longitude': '',
                'elevation': '',
                'status': ''
            })
            entry_id += 1
            new_time = random.randint(10, 30)
            new_timestamp = generate_timestamp_seconds_increment(current_timestamp, 1)
            writer.writerow({
                'created_at': new_timestamp.strftime("%Y-%m-%dT%H:%M:%S%z"),
                'entry_id': entry_id,
                'field1': '',
                'field2': '',
                'field3': '',
                'field4': new_time,
                'field5': '',
                'field6': '',
                'latitude': '',
                'longitude': '',
                'elevation': '',
                'status': ''
            })
            entry_id += 1
            new_timestamp += timedelta(seconds=1)
            field5 = None
            field6 = None
            if changes[i] == 1:
                field6 = random.choice([1] * random.randint(1, 5) + [2] * random.randint(1, 5))
                field5 = '';
            else:
                field5 = random.choice([1] * random.randint(1, 5) + [2] * random.randint(1, 5))
                field6 = '';
            writer.writerow({
                'created_at': new_timestamp.strftime("%Y-%m-%dT%H:%M:%S%z"),
                'entry_id': entry_id,
                'field1': '',
                'field2': '',
                'field3': '',
                'field4': '',
                'field5': field5,
                'field6': field6,
                'latitude': '',
                'longitude': '',
                'elevation': '',
                'status': ''
            })
            entry_id += 1


            if i != num_entries - 1:
                current_timestamp = time_stamps[i]
                while current_timestamp + timedelta(minutes=5) < time_stamps[i + 1]:
                    current_timestamp = generate_timestamp(current_timestamp, 5)
                    writer.writerow({
                        'created_at': current_timestamp.strftime("%Y-%m-%dT%H:%M:%S%z"),
                        'entry_id': entry_id,
                        'field1': field1,
                        'field2': '',
                        'field3': '',
                        'field4': '',
                        'field5': '',
                        'field6': '',
                        'latitude': '',
                        'longitude': '',
                        'elevation': '',
                        'status': ''
                    })
                    entry_id += 1
        
        current_timestamp += timedelta(hours=1)
        for num_temp in changes:
            if num_temp == 1:
                entries += 1
            if num_temp == -1:
                exits += 1
        temp_time = current_timestamp - timedelta(hours=1)
        writer.writerow({
            'created_at': current_timestamp.strftime("%Y-%m-%dT%H:%M:%S%z"),
            'entry_id': entry_id,
            'field1': '',
            'field2': entries,
            'field3': exits,
            'field4': '',
            'field5': '',
            'field6': '',
            'latitude': '',
            'longitude': '',
            'elevation': '',
            'status': ''
        })
        entry_id += 1

print("CSV file has been updated.")
