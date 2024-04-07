#include <Arduino.h>

// Pin definitions for ultrasonic sensors
const int trigPin1 = 2;
const int echoPin1 = 4;
const int trigPin2 = 12;
const int echoPin2 = 14;
const int trigPin3 = 25;
const int echoPin3 = 26;

int people_in = 0;
int people_out = 0;

float height1;
float height2;
float height3;

float distance1;
float distance2;
float distance3;

void person_at_point_1_entered();
void person_at_point_2_entered();
void person_at_point_3_entered();
void person_entered();

void person_at_point_1_exited();
void person_at_point_2_exited();
void person_at_point_3_exited();
void person_exited();

// Pin definition for IR sensor
const int irPin = 32;

void person_entered() {
  people_in++;
  Serial.println("Person entered");
  Serial.print("Number of people in room: ");
  Serial.println(people_in - people_out);
}

void person_exited() {
  people_out++;
  Serial.println("Person exited");
  Serial.print("Number of people in room: ");
  Serial.println(people_in - people_out);
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

void get_height() {
  height1 = readDistance(trigPin1, echoPin1);
  height2 = readDistance(trigPin2, echoPin2);
  height3 = readDistance(trigPin3, echoPin3);
}

void get_distances() {
  distance1 = height1 - readDistance(trigPin1, echoPin1);
  distance2 = height2 - readDistance(trigPin2, echoPin2);
  distance3 = height3 - readDistance(trigPin3, echoPin3);
}

void person_at_point_1_entered() {
  while(distance1 != 0) {
    get_distances();
  }
  if(distance2 != 0) {
    person_at_point_2_entered();
  }
}

void person_at_point_2_entered() {
  while(distance2 != 0) {
    get_distances();
  }
  if(distance1 != 0) {
    person_at_point_1_entered();
  } else {
    person_at_point_3_entered();
  }
}

void person_at_point_3_entered() {
  while(distance3 != 0) {
    get_distances();
  }
  if(distance2 != 0) {
    person_at_point_2_entered();
  }
  person_entered();
}

void person_at_point_1_exited() {
  while(distance1 != 0) {
    get_distances();
  }
  if(distance2 != 0) {
    person_at_point_2_exited();
  }
}

void person_at_point_2_exited() {
  while(distance2 != 0) {
    get_distances();
  }
  if(distance1 != 0) {
    person_at_point_1_exited();
  } else {
    person_at_point_3_exited();
  }
}

void person_at_point_3_exited() {
  while(distance3 != 0) {
    get_distances();
  }
  if(distance2 != 0) {
    person_at_point_2_exited();
  }
  person_exited();
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
  get_height();
  // Read distances from ultrasonic sensors

  // Read state of IR sensor
  int irValue = digitalRead(irPin);

  if(distance1 != 0) {
    person_at_point_1_entered();
  }

  if(distance3 != 0) {
    person_at_point_3_exited();
  }

  // Print sensor readings
  // Serial.print("Distance 1: ");
  // Serial.print(distance1);
  // Serial.println(" cm");
  // Serial.print("Distance 2: ");
  // Serial.print(distance2);
  // Serial.println(" cm");
  // Serial.print("Distance 3: ");
  // Serial.print(distance3);
  // Serial.println(" cm");
  // Serial.print("IR Sensor Value: ");
  // Serial.println(irValue);

  delay(1000);  // Wait for 1 second before next reading
}
