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

  delay(2000);
}

void sendCommand(const char* command) {
  mySerial.println(command);
  delay(1000); // Delay for command execution
}

void sendCommandToSlave(const char* command) {
  mySerial.println(command);
  delay(1000); // Delay for command execution
}

bool waitForResponse(const char* response) {
  unsigned long startTime = millis();
  size_t responseLength = strlen(response);
  size_t index = 0;
  while (millis() - startTime < 3000) { 
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

void removeConnectedDevices() {
  sendCommand("AT+RMAAD");

  if (waitForResponse("OK")) {
    Serial.println("All devices deleted in the paired list");
    digitalWrite(GREENLED, LOW); // Turn off green LED
    digitalWrite(REDLED, HIGH); // Turn on red LED
  } else {
    Serial.println("Failed to delete devices");
  }
}

void disconnectedDevices() {
  sendCommand("AT+DISC");

  if (waitForResponse("+DISC:")) {
    processState("SUCCESS");
    if (waitForResponse("OK")) {
      Serial.println("Disconnected successfully");
      sendCommandToSlave("DISCONNECTED"); // Send message to slave
    } else {
      Serial.println("Failed to disconnect");
    }
  } else {
    Serial.println("Error: Disconnection error");
  }
}

void loop() {
  while (mySerial.available() > 0) {
    mySerial.read(); // Clear serial buffer
  }

  // Check if AT command is working
  sendCommand("AT");
  if (waitForResponse("OK")) {
    Serial.println("AT command is working");
    removeConnectedDevices();

    // Get role
    sendCommand("AT+ROLE?");
    if (waitForResponse("+ROLE:")) {
      int role = mySerial.parseInt();
      if (role == 1) {
        Serial.println("Role: MASTER");
        sendCommand("AT+STATE?");
        if (waitForResponse("+STATE:")) {
          processState("READY");
          sendCommand("AT+LINK=98D3,51,F94FC7"); 
          if (waitForResponse("OK")) {
            sendCommand("AT+STATE?");
            if (waitForResponse("+STATE:")) {
              processState("CONNECTED");
              digitalWrite(GREENLED, HIGH); // Turn on green LED
              digitalWrite(REDLED, LOW); // Turn off red LED
              sendCommandToSlave("EMERGENCY"); // Send emergency message to slave
            } else {
              Serial.println("Error: Device is not connected");
            }
          } else {
            Serial.println("Error: Device is not linked");
          }
          delay(1000);
          disconnectedDevices();
          delay(1000);
          removeConnectedDevices();
          delay(5000);
        }
      } else {
        Serial.println("Error: Invalid role");
      }
    }
  } else {
    Serial.println("Error: AT command failed");
  }
}
