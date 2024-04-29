#include "HardwareSerial.h"

void setup() {
  // Initialize serial communication at 9600 baud rate
  Serial.begin(115200);
}

void loop() {
  // Check if data is available to read
  if (Serial.available() > 0) {
    String received = Serial.readString(); // Read the incoming data
    Serial.print("Received: ");
    Serial.println(received); // Echo back the received data
  }

  // Delay for a second before sending again
  delay(1000);
}

