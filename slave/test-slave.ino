#include <SoftwareSerial.h>

SoftwareSerial bluetoothSerial(10, 11); // RX, TX

void setup() {
  Serial.begin(9600);
  bluetoothSerial.begin(9600); // Set the baud rate to match your HC-06 module
}

void loop() {
  // Receive data from master
  while (bluetoothSerial.available()) {
    char c = bluetoothSerial.read();
    Serial.print(c);
  }

  // Send data to master
  bluetoothSerial.println("Hello from slave!");

  delay(1000);
}
