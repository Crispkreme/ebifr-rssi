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
  delay(1000); // Wait for response
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

void processCommand(const String& command) {
  if (command == "EMERGENCY") {
    Serial.println("Received EMERGENCY command from master!");
    digitalWrite(GREENLED, HIGH); // Turn on green LED
    digitalWrite(REDLED, LOW);    // Turn off red LED
  } else if (command == "DISCONNECTED") {
    Serial.println("Disconnected by master");
    digitalWrite(GREENLED, LOW);  // Turn off green LED
    digitalWrite(REDLED, HIGH);   // Turn on red LED
  }
}

void loop() {
  while (mySerial.available() > 0) {
    mySerial.read(); // Clear serial buffer
  }

  sendCommand("AT");
  if (waitForResponse("OK")) {
    Serial.println("AT command is working");

    sendCommand("AT+ROLE?");
    if (waitForResponse("+ROLE:")) {
      int role = mySerial.parseInt();
      if (role == 0) { // Check if slave role
        Serial.println("Role: SLAVE");

        if (mySerial.available()) {
          String command = mySerial.readStringUntil('\n');
          command.trim();
          processCommand(command);
        }
      } else {
        Serial.println("Error: Invalid role");
      }
    }

    delay(5000); // Delay before next inquiry
  } else {
    Serial.println("Error: AT command failed");
  }
}
