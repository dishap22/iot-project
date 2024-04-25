import requests
import csv

def get_thingspeak_data(channel_id, read_api_key):
    url = f"https://api.thingspeak.com/channels/{channel_id}/feeds.csv"
    params = {'api_key': read_api_key, 'results': 8000}  # Adjust 'results' as per your requirement

    response = requests.get(url, params=params)
    if response.status_code == 200:
        return response.text
    else:
        print(f"Failed to retrieve data from ThingSpeak: {response.status_code}")
        return None

def save_csv(data, file_path):
    with open(file_path, 'w', newline='') as csvfile:
        csvfile.write(data)

def get_data(file_name):
    # Example usage
    channel_id = 2506706
    read_api_key = '50QMQKRPSTGYJ1UM'
    file_path = file_name

    data = get_thingspeak_data(channel_id, read_api_key)
    if data:
        save_csv(data, file_path)
        print("Data saved successfully.")
    else:
        print("Failed to retrieve data.")

if __name__ == "__main__":
    get_data('data3.csv')
