#include <Arduino.h>

// Pin definitions for ultrasonic sensors
const int trigPin1 = 2;
const int echoPin1 = 4;
const int trigPin2 = 12;
const int echoPin2 = 14;
const int trigPin3 = 25;
const int echoPin3 = 26;

float distance1;
float distance2;
float distance3;

int entering = 0;
int exiting = 0;

void get_distances() {
    // Read distances from ultrasonic sensors
    distance1 = readDistance(trigPin1, echoPin1);
    distance2 = readDistance(trigPin2, echoPin2);
    distance3 = readDistance(trigPin3, echoPin3);
}

// Function to read distance from ultrasonic sensor
float readDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  float duration = pulseIn(echoPin, HIGH);
  return (duration * 0.0343) / 2;  // Speed of sound = 343 m/s
}

void setup() {
  Serial.begin(9600);

  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);
  digitalWrite(22, HIGH);
  digitalWrite(23, LOW);
}

void loop() {
  // Read distances from ultrasonic sensors
  get_distances();

  if(distance1 < 50 && exiting != 2) {
    entering = 1;
    exiting = 0;
  }
  if(distance3 < 50 && entering != 2) {
    entering = 0;
    exiting = 1;     
  }
  if(entering == 1 && distance2 < 50) {
    entering = 2;
  }
  if(exiting == 1 && distance2 < 50) {
    exiting = 2;
  }
  if(entering == 2 && distance3 < 50) {
    entering = 0;
    exiting = 0;
    Serial.println("Person entered");
  }
  if(exiting == 2 && distance1 < 50) {
    entering = 0;
    exiting = 0;
    Serial.println("Person exited");
  }

  Serial.print("Entering: ");
  Serial.println(entering);
  Serial.print("Exiting: ");
  Serial.println(exiting);
  Serial.println();

  // Print sensor readings
  Serial.print("Distance 1: ");
  Serial.print(distance1);
  Serial.println(" cm");
  Serial.print("Distance 2: ");
  Serial.print(distance2);
  Serial.println(" cm");
  Serial.print("Distance 3: ");
  Serial.print(distance3);
  Serial.println(" cm");
  Serial.println();

  delay(1000);
}
