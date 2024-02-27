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

// MASTER HC-05
// PIN CONFIGURATION
// HC-05   ->  ARDUINO UNO
// EN      ->  3.3V
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

  Serial.begin(19200);
  while (!Serial) {}

  Serial.println("Welcome to RSSI Transmitter");
  mySerial.begin(38400); // Set baud rate to match slave

  // Wait for the slave to initialize
  delay(2000); // Adjust delay as needed
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
  } else {
    Serial.println("Failed to delete devices");
  }
}

void disconnectedDevices() {
  // Disconnect
  sendCommand("AT+DISC");

  // Wait for the "+DISC:" response indicating successful disconnection
  if (waitForResponse("OK")) {
    Serial.println("Disconnected successfully");
    digitalWrite(REDLED, HIGH);   // Turn on the red LED
  } else {
    Serial.println("Failed to disconnect");
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

    removeConnectedDevices();

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
          sendCommand("AT+LINK=98D3,51,F94FC7"); // Replace with slave's Bluetooth address
          sendCommand("AT+STATE?");

          if (waitForResponse("+STATE:")) {
            processState("CONNECTED");
            digitalWrite(GREENLED, HIGH);
            digitalWrite(REDLED, LOW);
            Serial.println("ALERT: Emergency");

            disconnectedDevices();

            removeConnectedDevices();
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


// #include <SoftwareSerial.h>

// SoftwareSerial bluetoothSerial(10, 11); // RX, TX pins for Bluetooth module

// void setup() {
//   Serial.begin(9600); // Initialize serial communication for debugging
//   bluetoothSerial.begin(9600); // Initialize Bluetooth serial communication
  
//   // Wait for Bluetooth module to initialize
//   delay(1000);
  
//   Serial.println("Master device ready.");
// }

// void loop() {
//   // Send disconnect command to slave device
//   disconnectSlave();
  
//   // Wait for a while before sending the disconnect command again (if needed)
//   delay(5000); // Adjust delay as needed
// }

// void disconnectSlave() {
//   // Send disconnect command to slave device
//   bluetoothSerial.println("D"); // Sending 'D' to represent disconnect command
  
//   // Wait for acknowledgment from slave
//   delay(1000); // Adjust delay as needed
  
//   // Check if disconnection acknowledgment is received
//   if (bluetoothSerial.find("Success")) {
//     Serial.println("Slave disconnected successfully.");
//   } else {
//     Serial.println("Failed to disconnect slave.");
//   }
// }
