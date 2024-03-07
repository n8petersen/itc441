#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "NewPing.h"  // Imported "NewPing" library by Tim Eckel <eckel.tim@gmail.com>

// Wifi + Broker Info
const char* ssid = "nathan";
const char* password = "ilikepizza123";
const char* server = "192.168.137.2";
const char* ledTopic = "/lights";
const char* doorTopic = "/garage";
const char* host = "arduino-distance";  // Create hostname for this device

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// Distances Settings (cm)
const int blink_red = 10;
const int red = 15;
const int yellow = 20;
const int green = 25;

char* currColor = "off";
char* newColor = "off";

// Sensor setup
#define ECHO D2
#define TRIG D3
#define MAX 400
NewPing sonar(TRIG, ECHO, MAX);

bool toggle = true;
bool garageOpen = false;

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing ...");

  // Set built-in LED to output and turn off
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
  mqttClient.setCallback(callback);
  mqttClient.connect(host);
  mqttClient.subscribe(ledTopic);
  mqttClient.subscribe(doorTopic);
  Serial.println(mqttClient.state());

  mqttClient.publish("/debug", "Sensor Connected");

  digitalWrite(D4, LOW);  // turn on LED after WiFi and MQTT are connected
}

void loop() {

  mqttClient.loop();

  if (garageOpen) {
    // Get the distance using the NewPing library
    float distance = (sonar.ping() / 2) * 0.0343;

    // print distance for debugging
    // Serial.print("Dist: ");
    // Serial.print(distance);
    // Serial.println(" cm");

    if (green > distance && distance >= yellow) {
      newColor = "green";
    }
    else if (yellow > distance && distance >= red) {
      newColor = "yellow";
    }
    else if (red > distance && distance >= blink_red) {
      newColor = "red";
    }
    else if (blink_red > distance && distance > 0) {
      newColor = "blink";
    }
    else {
      newColor = "off";
    }

  } else {
    newColor = "off";
  }

  if (newColor != currColor) {
    mqttClient.publish("/lights", newColor);
    currColor = newColor;
  }
  delay(200);
}

void callback(char* topicChar, byte* payload, unsigned int length) {
  String topic = (String)topicChar;
  String message = "";

  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.println(message);

  if (topic == (String)doorTopic) {
    if (message == "open") {
      garageOpen = true;
      // Serial.println("Garage opened"); // Used for debugging
    }

    else {
      garageOpen = false;
      // Serial.println("Garage closed"); // Used for debugging
    }
  }
}