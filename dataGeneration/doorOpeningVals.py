import csv
import random
from datetime import datetime, timedelta

# Function to generate a random value for field4 biased towards 0-30
# Function to generate a biased random value for field4
def generate_biased_field4():
    rand = random.random()
    if rand < 0.05:  # 5% chance for 0-10
        return random.randint(0, 10)
    elif rand < 0.20:  # 15% chance for 31-50
        return random.randint(31, 50)
    else:  # 80% chance for 11-30
        return random.randint(11, 30)


# Function to modify timestamp by adding 1 second
def modify_timestamp(timestamp):
    dt = datetime.strptime(timestamp, "%Y-%m-%dT%H:%M:%S%z")
    modified_dt = dt + timedelta(seconds=1)
    return modified_dt.strftime("%Y-%m-%dT%H:%M:%S%z")

# Open original CSV file for reading
with open('../data/onlyField1.csv', 'r') as csv_file:
    csv_reader = csv.reader(csv_file)
    next(csv_reader)  # Skip header

    # Get the last index from the original CSV
    last_index = 0
    for row in csv_reader:
        last_index = int(row[1])

    # Open new CSV file for writing
    with open('../data/onlyField4.csv', 'w', newline='') as new_csv_file:
        csv_writer = csv.writer(new_csv_file)

        # Write header for new CSV file
        csv_writer.writerow(['timestamp', 'index', '', '', '', '', 'generatedvalue'])

        # Reset file pointer to beginning of original CSV
        csv_file.seek(0)
        next(csv_reader)  # Skip header again

        # Iterate through each line in original CSV file
        for row in csv_reader:
            timestamp = row[0]
            index = row[1]
            generated_value = generate_biased_field4()

            # Modify timestamp
            modified_timestamp = modify_timestamp(timestamp)

            # Write the modified row to the new CSV file
            csv_writer.writerow([modified_timestamp, '', '', '', '', '', generated_value])
