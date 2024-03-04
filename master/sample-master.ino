#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

#define GREENLED 4
#define REDLED 6

const char* addresses[] = {
  "98D3,51,H94FU0",
  "6843,10,194FT1",
  "AW33,21,L94FF6"
};

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
      if (role == 1) {
        Serial.println("Role: MASTER");

        sendCommand("AT+STATE?");

        if (waitForResponse("+STATE:")) {
          String state = mySerial.readStringUntil('\r\n');
          state.trim();

          if (state.indexOf("READY") != -1) {
            Serial.println("STATE: READY");

            for (int i = 0; i < sizeof(addresses) / sizeof(addresses[0]); i++) {
              sendCommand("AT+LINK=" + String(addresses[i]));

              sendCommand("AT+STATE?");

              if (waitForResponse("+STATE:")) {
                String state = mySerial.readStringUntil('\r\n');
                state.trim();

                if (state.indexOf("CONNECTED") != -1) {
                  Serial.println("STATE: CONNECTED");
                  digitalWrite(GREENLED, HIGH);
                  digitalWrite(REDLED, LOW);
                  Serial.println("ALERT: Emergency");

                  sendCommand("AT+DISC");

                  sendCommand("AT+STATE?");

                  if (waitForResponse("+STATE:")) {
                    String state = mySerial.readStringUntil('\r\n');
                    state.trim();

                    if (state.indexOf("DISCONNECTED") != -1) {
                      Serial.println("STATE: DISCONNECTED");
                    }
                  }
                }
              }
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
