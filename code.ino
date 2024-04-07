#include <Arduino.h>

// Pin definitions for ultrasonic sensors
const int trigPin1 = 2;
const int echoPin1 = 4;
const int trigPin2 = 12;
const int echoPin2 = 14;
const int trigPin3 = 25;
const int echoPin3 = 26;

// Pin definition for IR sensor
const int irPin = 32;

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

  pinMode(irPin, INPUT);
}

void loop() {
  // Read distances from ultrasonic sensors
  float distance1 = readDistance(trigPin1, echoPin1);
  float distance2 = readDistance(trigPin2, echoPin2);
  float distance3 = readDistance(trigPin3, echoPin3);

  // Read state of IR sensor
  int irValue = digitalRead(irPin);

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
  Serial.print("IR Sensor Value: ");
  Serial.println(irValue);

  delay(1000);  // Wait for 1 second before next reading
}
