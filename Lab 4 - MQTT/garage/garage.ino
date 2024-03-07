const int switchPin = D1; // Switch connected to digital pin D1 (Arduino pin 1)
int switchState = 0;     // Variable to store the state of the switch

void setup() {
  pinMode(switchPin, INPUT); // Set the switch pin as input
  Serial.begin(9600);        // Initialize serial communication at 9600 baud rate
}

void loop() {
  // Read the state of the switch
  switchState = digitalRead(switchPin);

  // Print the state of the switch to the serial monitor
  Serial.print("Switch state: ");
  Serial.println(switchState);

  delay(100); // Delay for debounce
}