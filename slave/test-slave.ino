// TO DO
// COMMUNICATE 2 BLUETOOTH MODULE
// PAIRING 2 BLUETOOTH MODULE
// CONNECT2 BLUETOOTH DEVICE
// CONNECTED - GREEN LED ON
//           - DISPLAY "CONNECTED DEVICE"
// DISCONNECTED BLUETOOTH DEVICE
//           - GREEN LED OFF
//           - RED LED ON
//           - DISPLAY "DISCONNECTED DEVICE"

//CONFIGURATION
// SLAVE
// ADDRESS - +ADDR:98D3:51:F94FC7
// NAME    - +NAME:BLUETOOTH-1
// PSWD    - 1234
// ROLE    - 0

// MASTER
// ADDRESS - +ADDR:0022:12:0232DB
// NAME    - +NAME:MASTER-BLUETOOTH
// PSWD    - 1234
// ROLE    - 1

// SLAVE HC-06 
// PIN CONFIGURATION
// VCC     ->  5V
// GND     ->  GND
// TX      ->  10
// RX      ->  11

#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

#define GREENLED 4
#define REDLED 6

void setup() {
  pinMode(GREENLED, OUTPUT);
  pinMode(REDLED, OUTPUT);

  Serial.begin(9600);
  while (!Serial) {}

  Serial.println("Welcome to RSSI Receiver");
  mySerial.begin(9600);
}

void sendCommand(const char* command) {
  mySerial.println(command);
  delay(500); // Wait for response
}

bool waitForResponse(const char* response) {
  unsigned long startTime = millis();
  size_t responseLength = strlen(response);
  size_t index = 0;
  while (millis() - startTime < 3000) { // Wait for maximum 3 seconds
    if (mySerial.available() > 0) {
      char c = mySerial.read();
      if (c == response[index]) {
        index++;
        if (index == responseLength) {
          return true;
        }
      } else {
        index = 0;
      }
    }
  }
  return false;
}

void loop() {

  while (mySerial.available() > 0) {
    mySerial.read();
  }

  sendCommand("AT");
  if (waitForResponse("OK")) {

    Serial.println("AT command is working");

    sendCommand("AT+VERSION?");
    if (waitForResponse("+VERSION:")) {
      String version = mySerial.readStringUntil('\r\n');
      Serial.println("Version: " + version);
    }

    sendCommand("AT+ROLE?");
    if (waitForResponse("+ROLE:")) {

      int role = mySerial.parseInt();
      if (role == 0) { // Check if slave role
        Serial.println("Role: SLAVE");

        sendCommand("AT+STATE?");
        if (waitForResponse("+STATE:")) {
          String state = mySerial.readStringUntil('\r\n');
          state.trim();

          if (state.indexOf("CONNECTED") != -1) {
            Serial.println("STATE: CONNECTED");
            digitalWrite(GREENLED, HIGH);
            digitalWrite(REDLED, LOW);
            Serial.println("ALERT: Emergency");
          } else {
            // Master not available
            Serial.println("Error: Master not available");
            digitalWrite(GREENLED, LOW);
            digitalWrite(REDLED, HIGH);
          }
        }
      } else {
        Serial.println("Error: Invalid role");
      }
    }

  } else {
    Serial.println("Error: AT command failed");
  }
}
