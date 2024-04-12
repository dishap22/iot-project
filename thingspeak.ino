#include <WiFi.h>
#include <ThingSpeak.h>
#include <PubSubClient.h>
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

// Ensure correct credentials to connect to your WiFi Network.
char ssid[] = "<replace>";
char pass[] = "<replace>";

// Ensure that the credentials here allow you to publish and subscribe to the ThingSpeak channel.
#define channelID 2505748 //should not be a string, just an int
const char mqttUserName[] = "JhcvMg8VPCUfOxsSMgc4LDU"; 
const char clientID[] = "JhcvMg8VPCUfOxsSMgc4LDU";
const char mqttPass[] = "eTObAhOzbn21TMyIVt0XRoak";

const char * PROGMEM thingspeak_ca_cert = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDxTCCAq2gAwIBAgIQAqxcJmoLQJuPC3nyrkYldzANBgkqhkiG9w0BAQUFADBs\n" \
"MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n" \
"d3cuZGlnaWNlcnQuY29tMSswKQYDVQQDEyJEaWdpQ2VydCBIaWdoIEFzc3VyYW5j\n" \
"ZSBFViBSb290IENBMB4XDTA2MTExMDAwMDAwMFoXDTMxMTExMDAwMDAwMFowbDEL\n" \
"MAkGA1UEBhMCVVMxFTATBgNVBAoTDERpZ2lDZXJ0IEluYzEZMBcGA1UECxMQd3d3\n" \
"LmRpZ2ljZXJ0LmNvbTErMCkGA1UEAxMiRGlnaUNlcnQgSGlnaCBBc3N1cmFuY2Ug\n" \
"RVYgUm9vdCBDQTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMbM5XPm\n" \
"+9S75S0tMqbf5YE/yc0lSbZxKsPVlDRnogocsF9ppkCxxLeyj9CYpKlBWTrT3JTW\n" \
"PNt0OKRKzE0lgvdKpVMSOO7zSW1xkX5jtqumX8OkhPhPYlG++MXs2ziS4wblCJEM\n" \
"xChBVfvLWokVfnHoNb9Ncgk9vjo4UFt3MRuNs8ckRZqnrG0AFFoEt7oT61EKmEFB\n" \
"Ik5lYYeBQVCmeVyJ3hlKV9Uu5l0cUyx+mM0aBhakaHPQNAQTXKFx01p8VdteZOE3\n" \
"hzBWBOURtCmAEvF5OYiiAhF8J2a3iLd48soKqDirCmTCv2ZdlYTBoSUeh10aUAsg\n" \
"EsxBu24LUTi4S8sCAwEAAaNjMGEwDgYDVR0PAQH/BAQDAgGGMA8GA1UdEwEB/wQF\n" \
"MAMBAf8wHQYDVR0OBBYEFLE+w2kD+L9HAdSYJhoIAu9jZCvDMB8GA1UdIwQYMBaA\n" \
"FLE+w2kD+L9HAdSYJhoIAu9jZCvDMA0GCSqGSIb3DQEBBQUAA4IBAQAcGgaX3Nec\n" \
"nzyIZgYIVyHbIUf4KmeqvxgydkAQV8GK83rZEWWONfqe/EW1ntlMMUu4kehDLI6z\n" \
"eM7b41N5cdblIZQB2lWHmiRk9opmzN6cN82oNLFpmyPInngiK3BD41VHMWEZ71jF\n" \
"hS9OMPagMRYjyOfiZRYzy78aG6A9+MpeizGLYAiJLQwGXFK3xPkKmNEVX58Svnw2\n" \
"Yzi9RKR/5CYrCsSXaQ3pjOLAEFe4yHYSkVXySGnYvCoCWw9E1CAx2/S6cCZdkGCe\n" \
"vEsXCS+0yx5DaMkHJ8HSXPfqIbloEpw8nL+e/IBcm2PN7EeqJSdnoDfzAIJ9VNep\n" \
"+OkuE6N36B9K\n" \
"-----END CERTIFICATE-----\n";

#define mqttPort 1883
WiFiClient client;

const char* server = "mqtt3.thingspeak.com";
int status = WL_IDLE_STATUS; 
long lastPublishMillis = 0;
int connectionDelay = 1;
int updateInterval = 15;
PubSubClient mqttClient( client );

// Function to handle messages from MQTT subscription.
void mqttSubscriptionCallback( char* topic, byte* payload, unsigned int length ) {
  // Print the details of the message that was received to the serial monitor.
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

// Subscribe to ThingSpeak channel for updates.
void mqttSubscribe( long subChannelID ){
  String myTopic = "channels/"+String( subChannelID )+"/subscribe";
  mqttClient.subscribe(myTopic.c_str());
}

// Publish messages to a ThingSpeak channel.
void mqttPublish(long pubChannelID, String message) {
  String topicString ="channels/" + String( pubChannelID ) + "/publish";
  mqttClient.publish( topicString.c_str(), message.c_str() );
}

// Connect to WiFi.
void connectWifi()
{
  Serial.print( "Connecting to Wi-Fi..." );
  // Loop until WiFi connection is successful
  while ( WiFi.status() != WL_CONNECTED ) {
    WiFi.begin( ssid, pass );
    delay( connectionDelay*1000 );
    Serial.print( WiFi.status() ); 
  }
  Serial.println( "Connected to Wi-Fi." );
}

// Connect to MQTT server.
void mqttConnect() {
  // Loop until connected.
  while ( !mqttClient.connected() )
  {
    // Connect to the MQTT broker.
    if ( mqttClient.connect( clientID, mqttUserName, mqttPass ) ) {
      Serial.print( "MQTT to " );
      Serial.print( server );
      Serial.print (" at port ");
      Serial.print( mqttPort );
      Serial.println( " successful." );
    } else {
      Serial.print( "MQTT connection failed, rc = " );
      // See https://pubsubclient.knolleary.net/api.html#state for the failure code explanation.
      Serial.print( mqttClient.state() );
      Serial.println( " Will try again in a few seconds" );
      delay( connectionDelay*1000 );
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin( 115200 );
  // Delay to allow serial monitor to come up.
  delay(3000);
  // Connect to Wi-Fi network.
  connectWifi();
  // Configure the MQTT client
  mqttClient.setServer( server, mqttPort ); 
  // Set the MQTT message handler function.
  mqttClient.setCallback( mqttSubscriptionCallback );
  // Set the buffer to handle the returned JSON. NOTE: A buffer overflow of the message buffer will result in your callback not being invoked.
  mqttClient.setBufferSize( 2048 );

  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);

  pinMode(irPin, INPUT);
}

/*void loop() {
  // put your main code here, to run repeatedly:
  // Reconnect to WiFi if it gets disconnected.
  if (WiFi.status() != WL_CONNECTED) {
      connectWifi();
  }
  
  // Connect if MQTT client is not connected and resubscribe to channel updates.
  if (!mqttClient.connected()) {
     mqttConnect(); 
     mqttSubscribe( channelID );
  }
  
  // Call the loop to maintain connection to the server.
  mqttClient.loop(); 
  
  // Update ThingSpeak channel periodically. The update results in the message to the subscriber.
  if ( abs(long(millis()) - lastPublishMillis) > updateInterval*1000) {
    mqttPublish( channelID, (String("field1=")+String(WiFi.RSSI())) );
    lastPublishMillis = millis();
  }
}*/

void loop() {
  // Reconnect to WiFi if it gets disconnected.
  if (WiFi.status() != WL_CONNECTED) {
      connectWifi();
  }
  
  // Connect if MQTT client is not connected and resubscribe to channel updates.
  if (!mqttClient.connected()) {
     mqttConnect(); 
     mqttSubscribe( channelID );
  }
  
  // Call the loop to maintain connection to the server.
  mqttClient.loop(); 

  // Update ThingSpeak channel periodically with random data.
  if ( abs(long(millis()) - lastPublishMillis) > updateInterval*1000) {
    
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


    // Publish the random value to ThingSpeak.
    mqttPublish( channelID, (String("field1=")+String(people_in)) );
    mqttPublish(channelID, (String("field2=")+String(people_out)));
    
    // Update the last publish time.
    lastPublishMillis = millis();
  }
}