#include <SoftwareSerial.h>

#define GREENLED 4
#define REDLED 6

SoftwareSerial mySerial(8, 9); // RX, TX (Connect to RX, TX of master)

void setup() {
  pinMode(GREENLED, OUTPUT);
  pinMode(REDLED, OUTPUT);
  Serial.begin(9600);
  mySerial.begin(9600); // Set baud rate for communication with master
}

void loop() {
  if (mySerial.available()) {
    String message = mySerial.readStringUntil('\n');
    if (message == "ALERT: Emergency") {
      // Green LED on, Red LED off
      digitalWrite(GREENLED, HIGH);
      digitalWrite(REDLED, LOW);
    }
  }
}
