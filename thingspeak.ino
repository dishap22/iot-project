#include <WiFi.h>
#include <ThingSpeak.h>
#include <PubSubClient.h>
#include <Arduino.h>

// Pin definitions for ultrasonic and IR sensors
const int trigPin1 = 2;
const int echoPin1 = 4;
const int trigPin2 = 12;
const int echoPin2 = 14;
const int trigPin3 = 25;
const int echoPin3 = 26;
const int ir = 5;

const int led50 = 27;
const int led100 = 19;
const int ledGround = 21;

int dur = 0;

float distance1;
float distance2;
float distance3;

int entering = 0;
int exiting = 0;
int num_people = 0;

unsigned long lastHourTimestamp = 0;
int entriesThisHour = 0;
int exitsThisHour = 0;
int DOOR=LOW;


void get_distances() {
    // Read distances from ultrasonic sensors
    distance1 = readDistance(trigPin1, echoPin1);
    distance2 = readDistance(trigPin2, echoPin2);
    distance3 = readDistance(trigPin3, echoPin3);
}

void updateLeds() {
  int occupancyPercentage = (num_people * 100) / 20; // Calculate occupancy percentage

  digitalWrite(led50, LOW);
  digitalWrite(led100, LOW);

  // Determine which LEDs to turn on based on occupancy percentage
  if (occupancyPercentage >= 50 && occupancyPercentage < 100) {
    digitalWrite(led50, HIGH); // Turn on led50
  } else if (occupancyPercentage >= 100) {
    digitalWrite(led50, HIGH);
    digitalWrite(led100, HIGH); // Turn on led100
  }
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

// Ensure correct credentials to connect to your WiFi Network.
char ssid[] = "seera";
char pass[] = "afwd3291";

// Ensure that the credentials here allow you to publish and subscribe to the ThingSpeak channel.
#define channelID 2506706 //should not be a string, just an int
const char mqttUserName[] = "CSodJDMlGCIHKA8LJB8JES0"; 
const char clientID[] = "CSodJDMlGCIHKA8LJB8JES0";
const char mqttPass[] = "uAbYatwOcDfR28N+7S4Wnp10";

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
  pinMode(ir, INPUT);
  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);
  pinMode(32, OUTPUT);
  pinMode(33, OUTPUT);
  digitalWrite(22, HIGH);
  digitalWrite(23, LOW);
  digitalWrite(32, HIGH);
  digitalWrite(33, LOW);
  pinMode(led50, OUTPUT);
  pinMode(led100, OUTPUT);
  pinMode(ledGround, OUTPUT);
  digitalWrite(ledGround, LOW);
}

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
  if(dur == 0) {
    if(entering == 2) {
      entering = 0;
    }
    if(exiting == 2) {
      exiting = 0;
    }
  }
  if(entering == 2 && distance3 < 50) {
    entering = 0;
    exiting = 0;
    Serial.println("Person entered");
    num_people++;
    entriesThisHour++;
    // Update ThingSpeak channel when num_people changes with random data.
    mqttPublish( channelID, (String("field1=")+String(num_people)));
  }
  if(exiting == 2 && distance1 < 50) {
    entering = 0;
    exiting = 0;
    Serial.println("Person exited");
    num_people--;
    exitsThisHour++;
    // Update ThingSpeak channel when num_people changes with random data.
    mqttPublish( channelID, (String("field1=")+String(num_people)));
  }
  DOOR=digitalRead(ir);
  if(DOOR == HIGH){
    Serial.print("Door is closed\n");
    if(dur != 0){
      Serial.print("Door was open for: ");
      Serial.print(dur);
      Serial.println(" seconds");
      mqttPublish( channelID, (String("field4=")+String(dur)));
    }
    dur = 0;
  }
  else if(DOOR == LOW){
    Serial.print("Door is open\n");
    dur += 1;
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

  unsigned long currentTimestamp = millis();
  if (currentTimestamp - lastHourTimestamp >= 3600000) { // 3600000 milliseconds = 1 hour
    // Publish the number of entries and exits for the current hour to fields 3 and 4
    mqttPublish( channelID, (String("field2=")+String(entriesThisHour)));
    mqttPublish( channelID, (String("field3=")+String(exitsThisHour)));
    // Reset the counters and update the timestamp
    entriesThisHour = 0;
    exitsThisHour = 0;
    lastHourTimestamp = currentTimestamp;
  }
  updateLeds();
  delay(1000);
}
