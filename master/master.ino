#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

#define GREENLED 4
#define REDLED 6

void setup() {
  pinMode(GREENLED, OUTPUT);
  pinMode(REDLED, OUTPUT);

  Serial.begin(19200);
  while (!Serial) {}

  Serial.println("Welcome to RSSI Transmitter");
  mySerial.begin(38400);
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

void processState(const char* state) {
  String response = mySerial.readStringUntil('\r\n');
  response.trim();

  if (response.indexOf(state) != -1) {
    Serial.println("STATE: " + String(state));
  }
}

void loop() {
  while (mySerial.available() > 0) {
    mySerial.read();
  }

  // Check if AT command is working
  sendCommand("AT");
  if (waitForResponse("OK")) {
    Serial.println("AT command is working");

    // Get role
    sendCommand("AT+ROLE?");
    if (waitForResponse("+ROLE:")) {
      int role = mySerial.parseInt();
      if (role == 1) {
        Serial.println("Role: MASTER");

        // Check state
        sendCommand("AT+STATE?");
        if (waitForResponse("+STATE:")) {
          processState("READY");

          // Connect
          sendCommand("AT+LINK=98D3,51,F94FC7");
          sendCommand("AT+STATE?");
          if (waitForResponse("+STATE:")) {
            processState("CONNECTED");
            digitalWrite(GREENLED, HIGH);
            digitalWrite(REDLED, LOW);
            Serial.println("ALERT: Emergency");

            // Disconnect
            sendCommand("AT+DISC");

            // Wait for the "+DISC:" response indicating successful disconnection
            if (waitForResponse("OK")) {
              Serial.println("Disconnected successfully");
              digitalWrite(GREENLED, LOW);  // Turn off the green LED
              digitalWrite(REDLED, HIGH);   // Turn on the red LED
            } else {
              Serial.println("Failed to disconnect");
            }

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
