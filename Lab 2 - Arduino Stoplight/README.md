Nathan Petersen  
08 Feb 2024  
IT&C 441 - Embedded Systems  
Todd Berrett  

---

# Lab 2 - Wifi Stoplight pt. 2

## Objective
The purpose of this lab report is to learn the basic principles of the Arduino platform by creating a wifi-controlled stoplight. These principles include:
- Become familiar with the Arduino platform and its programming methodologies.
	- Setup() and Loop()
	- NBL's (non-blocking loops)
- Program an Arduino-based microcontroller to use the GPIO pins to control LEDs.
- Implement a simple webserver on the Arduino platform.
## Materials
- Arduino IDE (Software)
- Computer with USB port (or adapter)
- 1 x Wemos D1 Mini Microcontroller
- 1 x MicroUSB Cable (for power, and serial output)
- 1 x Breadboard
- 3 x LED's
	- Red
	- Yellow
	- Green
- 3 x Resistors (200 $\Omega$)
- 7 x Jumper Wires 
## References
Wemos webserver example  
https://www.esp8266learning.com/wemos-webserver-example.php  

Guide for setting up Wemos D1 Mini with Arduino IDE  
https://www.instructables.com/Programming-the-WeMos-Using-Arduino-SoftwareIDE/  

C++ Multi-line String - Used for HTML page  
https://stackoverflow.com/questions/1135841/how-to-write-a-multi-line-string-literal  

Circuit Diagraming  
https://fritzing.org/download/  
## Procedures
### 1. Setup breadboard
*Referencing the Photos and Diagrams in Appendix might be helpful for this step*  
1. Solder jumper pins to Wemos D1 so we can attach it to the breadboard
2. Place Wemos D1 into breadboard
3. Connect ground lane to Wemos D1 to ground pin
4. Complete circuit for each LED
	- Attach a unique GPIO pin to a resistor in its own lane
	- Connect each resistor to it's own LED
	- Connect all Negative (-) side of LED to ground lane
5. You can manually test your circuitry by plugging in the Wemos to power, and jumping the 3.3V pin to the same lane as each GPIO output
	- If everything is wired correctly, each LED should turn on as you jump the 3.3V output to each GPIO lane
### 2. Setup Arduino IDE
1. Install [Arduino IDE](https://www.arduino.cc/en/software)
2. Connect the Wemos to computer using Micro-USB cable
3. Check which COM Port the device uses on your machine
	- This can be located in Device Manager under the COM section
	- Disconnect and Connect the USB cable, while noting which COM port appears/disappears as you do so
4. Add the Wemos D1 library to the Arduino IDE
	- Go to Files > Preferences
	- In **Additional Boards Managers URLs** field, paste the following URL:
		  `http://arduino.esp8266.com/stable/package_esp8266com_index.json`
    - Under Boards Manager (second icon on left bar), search 'esp8266' or 'wemos', and click Install
    - Under Tools > Board> esp8266 > LOLIN(WEMOS) D1 R2 & mini
### 3. Program Board
1. Code the board to specification
   - Or clone the provided code
2. Compile and push the code to the board
### 4. Control the lights!
1. In the serial output, the IP address will be displayed
2. Visit the site, and control the lights
## State Diagram
*View Appendix 2 for state diagram*  
## Analysis of Two Different Views
### System View
1. What are the user-facing interfacing aspects of this system?
   - Web site view with buttons to control the lights
2. What does the whole system accomplish for the user?
   - User can control individual lights, or turn on/off automatic timer, or turn off all lights
3. What inputs / outputs occur?
   - Buttons for each color turns on each light individually
   - Timer button sets automatic timer to turn the lights in a cycle
   - Button to turn off all LED's

*View Appendix 4 for user view image*  
### Component View
1. What are the different parts and pieces of the system?
2. How does each component of the system interact?
3. What parameters are passed between components? What connections, ports, protocols, etc are used for these interactions?
4. What standards (APIs, taxonomy, etc) are utilized for these interactions?

*View Appendix 3 for circuit diagram*  
## Certification of Work
I, Nathan Petersen, certify that this Lab Submission represents my own work. Any other resources used have been references in code and in the *References* section above.
## Thought Questions
1. What are some key differences between developing this lab on a Raspberry Pi, and developing on Arduino?
	- Arduino uses C++ rather than Python (which is the primary language on Raspi), so the specific implementation is different, as the languages are different.
	- Arduino infrastructure is built on 2 functions, the setup() function which is run once at the beginning, and loop() which is run over and over. 
	- Since Arduino is meant for small controllers with 1 core, multithreating is either more difficult or impossible on these chips, meaning non-blocking loops are crucial to implement timing in the program without locking up the processor. 
1. What are the strengths and trade-offs of each of these platforms?
	- Arduino is much smaller and cheaper
	- Arduino has a really fast startup-time: just supply power and it goes cranking immediately.
	- Raspberry Pi's have more hardware available to it, so you can accomplish more complex tasks
	- Raspberry Pi's have an Operating System, rather than running compiled code written to its ROM.
1. How familiar were you with the Arduino platform prior to this lab?
	- Only what we've done in class, so very little.
2. How does the web server you implemented know what content to serve? How is this different than other webservers you have worked with previously?
	- It writes one line at at time to the content. 
	- I'm accustomed to web servers serving static (or dynamic pages) in bulk, so you write an HTML page and serve the entire page to the client. 
1. What was the biggest challenge you overcame in this lab?
	- Building the non-blocking loop for the timer. At first I had the `if` statement checking if the timer was turned on later in the loop, but it wasn't actually running, or it would run but not turn off. I realized that I needed to put it at the very beginning of the `loop()` function to do its operations before checking for HTTP requests and performing its other functions. 
	- I knew from our other class lectures that I needed to use NBL (non-blocking loop), so I implemented that part fairly easily the first time.
	- At first I had a global interval for each color change, but I realized that in my switch statement I could change that interval for each color, so the red and green lights last longer than the yellow light.
1. Please estimate the total time you spent on this lab and report.
	- 0.5 hour soldering pins and wiring up board
	- 3.5 hours coding 
	- 1 hour on writeup
	- **5 hours total**
## Appendix
### Appendix 1: System Flowchart
<img src='./_images/Lab 2 - Flowchart.png' width=700></img>    
### Appendix 2: State Diagram
<img src='./_images/Lab 2 - State Diagram.png' width=600></img>    
### Appendix 3: Circuit Diagram
<img src='./_images/Lab 2 - Circuit Diagram.png' width=600></img>  
### Appendix 4: Website Controller
<img src='./_images/Lab 2 - Website.png' width=400></img>    
### Appendix 5: Serial Monitor Output Example
<img src='./_images/Lab 2 - Serial Output.png' width=400></img>  
### Appendix 6: Pictures
<img src='./_images/Lab 2 - Image.png' width=500></img>  
### Appendix 7: Code
https://github.com/n8petersen/itc441/tree/main/Lab%202%20-%20Arduino%20Stoplight  
``` C++
#include <ESP8266WiFi.h> 

const char* wifiSSID = "natepete";
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
```