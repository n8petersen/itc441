#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Wifi + Broker Info
const char* ssid = "nathan";
const char* password = "ilikepizza123";
const char* server = "192.168.137.2";
const char* ledTopic = "/lights";
const char* doorTopic = "/garage";
const char* host = "arduino-garage";  // Create hostname for this device

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

const int switchPin = D2;  // Switch connected to digital pin D1 (Arduino pin 1)
int currState = 0;         // Variable to store the state of the switch
int newState = 0;

void setup() {
  Serial.begin(9600);  // Initialize serial communication at 9600 baud rate
  Serial.println("Initializing ...");

  pinMode(switchPin, INPUT_PULLUP);  // Set the switch pin as input
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(D4, HIGH);  // turn off the built in LED

  // ## Connect to WiFi - Adapted from http://www.esp8266learning.com/wemos-webserver-example.php
  Serial.print("Connecting WiFi to WiFi: ");
  Serial.print(ssid);

  WiFi.hostname(host);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {  //Wait for connection
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Connected");
  delay(2000);  // Give things time to start up properly

  //  ## Connect to MQTT
  Serial.println("Connecting to MQTT.");
  mqttClient.setServer(server, 1883);
  // mqttClient.setCallback(callback);
  mqttClient.connect(host);
  mqttClient.subscribe(ledTopic);
  Serial.println(mqttClient.state());

  mqttClient.publish("/debug", "Garage Connected");

  digitalWrite(D4, LOW);  // turn on LED after WiFi and MQTT are connected
}

void loop() {
  // Read the state of the switch
  newState = digitalRead(switchPin);

  if (newState != currState) {
    currState = newState;

    Serial.print("Switch state: ");
    Serial.println(currState);
    
    if (currState == HIGH) {
      mqttClient.publish("/garage", "open");
    }
    else if (currState == LOW) {
      mqttClient.publish("/garage", "closed");
    }

  }

  delay(200);  // Delay for debounce
}