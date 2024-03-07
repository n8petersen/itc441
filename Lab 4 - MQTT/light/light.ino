#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Wifi + Broker Info
const char* ssid = "nathan";
const char* password = "ilikepizza123";
const char* server = "192.168.137.1";
const char* ledTopic = "/lights";
const char* host = "arduino-lights";  // Create hostname for this device

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

int redLED = D3;
int yellowLED = D2;
int greenLED = D1;

bool timerOn = false;
unsigned long previousMillis = 0;
long interval = 500;
bool redblinkon = false;

// ## Re-usable function to turn on a specific LED
void turnLED(String color = "off") {
  // Turn off all LED's
  digitalWrite(redLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(greenLED, LOW);

  // only turn on the passed LED
  if (color == "red") {
    digitalWrite(redLED, HIGH);
  } else if (color == "yellow") {
    digitalWrite(yellowLED, HIGH);
  } else if (color == "green") {
    digitalWrite(greenLED, HIGH);
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing ...");

  // ## Set all color pins to output
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);

  // ## Initialize all LED's to OFF
  turnLED();
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
  Serial.println(mqttClient.state());

  mqttClient.publish("/debug", "Lights Connected");

  digitalWrite(D4, LOW);  // turn on LED after WiFi and MQTT are connected
}

void loop() {

  mqttClient.loop();

  // // run the timer if it is turned on
  if (timerOn) {
    Serial.print("T");  // used for debugging to ensure it was actually on
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      turnLED();
      redblinkon = !redblinkon;
      digitalWrite(redLED, redblinkon);
    }
  }
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

  if (topic == (String)ledTopic) {
    if (message == "red") {
      turnLED("red");
      timerOn = false;
      // redblinkon = false;
    }

    if (message == "yellow") {
      turnLED("yellow");
      timerOn = false;
      // redblinkon = false;
    }

    if (message == "green") {
      turnLED("green");
      timerOn = false;
      // redblinkon = false;
    }

    if (message == "off") {
      turnLED();
      timerOn = false;
      // redblinkon = false;
    }

    if (message == "blink") {
      if (!timerOn) {
        timerOn = true;
      }
    }
  }
}