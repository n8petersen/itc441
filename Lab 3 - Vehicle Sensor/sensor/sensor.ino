#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include "NewPing.h"  // Imported "NewPing" library by Tim Eckel <eckel.tim@gmail.com>

// WiFi Settings
const char* wifiSSID = "nathan";
const char* wifiPSK = "ilikepizza123";

// Stoplight IP
String serverName = "http://192.168.137.247";

// Distances Settings (cm)
const int blink_red = 10;
const int red = 15;
const int yellow = 20;
const int green = 25;

// Sensor setup
#define ECHO D2
#define TRIG D3
#define MAX 400
NewPing sonar(TRIG, ECHO, MAX);

bool toggle = true;

void setup() {
  Serial.begin(9600);
  delay(5000);  // Give things time to start up
  Serial.println("Initializing ...");

  // Set built-in LED to output and turn off
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(D4, HIGH);  // turn off the built in LED

  // Connect to WiFi - Adapted from http://www.esp8266learning.com/wemos-webserver-example.php
  Serial.println("Connecting to ");
  Serial.println(wifiSSID);
  WiFi.begin(wifiSSID, wifiPSK);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  digitalWrite(D4, LOW);  // turn the Built-in LED on after WiFi is connected and ready
}

void loop() {

  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;
    String serverPath;


    // Get the distance using the NewPing library
    float distance = (sonar.ping() / 2) * 0.0343;

    // print distance for debugging
    // Serial.print("Dist: ");
    // Serial.print(distance);
    // Serial.println(" cm");

    if (green > distance && distance >= yellow) {
      // Serial.println("green");
      serverPath = serverName + "/green";
      toggle = true;
    }

    else if (yellow > distance && distance >= red) {
      // Serial.println("yellow");
      serverPath = serverName + "/yellow";
      toggle = true;
    }

    else if (red > distance && distance >= blink_red) {
      // Serial.println("red");
      serverPath = serverName + "/red";
      toggle = true;
    }

    else if (blink_red > distance && distance > 0) {
      // Serial.println("blink");
      serverPath = serverName + "/timer";
      // send once and stop repeating it
      Serial.print("dist: ");
      Serial.print(distance);

      Serial.print("  --  ");
      Serial.print(serverPath);

      http.begin(client, serverPath);
      int httpResponseCode = http.GET();

      Serial.print("  --  ");
      Serial.println(httpResponseCode);
      toggle = false;
    }

    else {
      serverPath = serverName + "/off";
      toggle = true;
    }

    if (toggle == true) {
      Serial.print("dist: ");
      Serial.print(distance);

      Serial.print("  --  ");
      Serial.print(serverPath);

      http.begin(client, serverPath);
      int httpResponseCode = http.GET();

      Serial.print("  --  ");
      Serial.println(httpResponseCode);
    }
  }

  delay(500);
}
