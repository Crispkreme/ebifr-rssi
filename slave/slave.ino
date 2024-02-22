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
