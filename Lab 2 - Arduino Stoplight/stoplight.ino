#include <ESP8266WiFi.h> 

const char* wifiSSID = "nathan";
const char* wifiPSK  = "ilikepizza123";

WiFiServer server(80);

int redLED = D3;
int yellowLED = D2;
int greenLED = D1;


bool timerOn = false;
unsigned long previousMillis = 0;
long interval = 1000;

int currentLED = 0;

// re-usable function to turn on a specific LED
void turnLED(String color = "off") {
  // Turn off all LED's
  digitalWrite(redLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(greenLED, LOW);

  // only turn on the passed LED
  if (color == "red") { digitalWrite(redLED, HIGH); }
  else if (color == "yellow") { digitalWrite(yellowLED, HIGH); }
  else if (color == "green") { digitalWrite(greenLED, HIGH); }
}

void setup() {

  Serial.begin(9600);
  delay(5000); // Give things time to start up
  Serial.println("Initializing ...");

  // Set all color pins to output
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);

  // Initialize all LED's to OFF
  turnLED();
  digitalWrite(D4, HIGH); // turn off the built in LED

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

  // Start web server
  server.begin();
  Serial.println("Server started");

  // Print IP address to control stop light
  Serial.print("Control the Stop Light at: http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

  // ** End Adapted Code - Adapted from www.esp8266learning.com
  digitalWrite(D4, LOW); // turn the Built-in LED on after WiFi is connected and ready
}

void loop() {

  // run the timer if it is turned on
  if (timerOn) {
      // Serial.print("T"); // used for debugging to ensure it was actually on
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        
        // change the color depending on what color is currently set
        switch (currentLED) {
          case 0:
            turnLED("green");
            currentLED = 1;
            interval = 2000;
            break;
          case 1:
            turnLED("yellow");
            currentLED = 2;
            interval = 1000;
            break;
          case 2:
            turnLED("red");
            currentLED = 0;
            interval = 2000;
            break;          
        }
      }
    }

  // Create web server - Adapted from http://www.esp8266learning.com/wemos-webserver-example.php
  WiFiClient client = server.available();
  if (client) {  
    while (!client.available()) {
      delay(10); // client isn't ready, make like Aaron Burr and wait for it.
    }

    String request = client.readStringUntil('\r'); // get that first line of da request
    Serial.println(request); // print the request out for debugging purposes
    client.flush(); //  not really sure why this is needed but i trust

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println(""); //  do not forget this one

    if (request.indexOf("/red") != -1) {
      turnLED("red");
      timerOn = false;
    }

    if (request.indexOf("/yellow") != -1) {
      turnLED("yellow");
      timerOn = false;
    }

    if (request.indexOf("/green") != -1) {
      turnLED("green");
      timerOn = false;
    }

    if (request.indexOf("/off") != -1) {
      turnLED();
      timerOn = false;
    }

    if (request.indexOf("/timer") != -1) {
      timerOn = !timerOn;
      turnLED(); // turn off all LED's
      if (!timerOn) { Serial.println(); }
    }

      // page for the client to see
    const char *htmlpage =
      "<!DOCTYPE HTML>"
      "<html>"
      "<head>"
          "<meta name='viewport' content='width=device-width, initial-scale=1'>"
          "<title> WiFi Stoplight </title>"
      "</head>"
      "<body>"
          "<button onclick=\"window.location.href=\'/red\'\">Red</button>"
          "<button onclick=\"window.location.href=\'/yellow\'\">Yellow</button>"
          "<button onclick=\"window.location.href=\'/green\'\">Green</button>"
          "<button onclick=\"window.location.href=\'/off\'\">Off</button>"
          "<button onclick=\"window.location.href=\'/timer\'\">Timer</button>"
      "</body>"
      "</html>";
    client.println(htmlpage);
  }
  delay (10);
}
