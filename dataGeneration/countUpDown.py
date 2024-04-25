import csv
from datetime import datetime

def count_changes_by_hour(rows):
    ups = downs = 0
    prev_value = None

    for row in rows:
        value = int(row[2])  # Assuming values are in the 3rd column
        if prev_value is not None:
            if value > prev_value:
                ups += 1
            elif value < prev_value:
                downs += 1
        prev_value = value

    return ups, downs

# Read values from CSV file and count changes by hour
with open('../data/field1andfield4.csv', 'r') as csvfile:
    reader = csv.reader(csvfile)
    next(reader)
    current_hour = None
    current_hour_rows = []

    for row in reader:
        timestamp = datetime.strptime(row[0], '%Y-%m-%dT%H:%M:%S%z')
        hour = timestamp.hour

        if current_hour is None or hour == current_hour:
            current_hour_rows.append(row)
            current_hour = hour
        else:
            ups, downs = count_changes_by_hour(current_hour_rows)
            print("Hour:", current_hour)
            print("Timestamp:", current_hour_rows[-1][0])
            print("Number of increases:", ups)
            print("Number of decreases:", downs)
            print("---------------------------------")
            current_hour = hour
            current_hour_rows = [row]

    # Output the last hour's counts
    if current_hour_rows:
        ups, downs = count_changes_by_hour(current_hour_rows)
        print("Hour:", current_hour)
        print("Timestamp:", current_hour_rows[-1][0])
        print("Number of increases:", ups)
        print("Number of decreases:", downs)
