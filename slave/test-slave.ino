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
  // Wait for the master to disconnect
  sendCommand("AT+STATE?");
  while (waitForResponse("+STATE:")) {
    if (!waitForResponse("CONNECTED")) {
      Serial.println("Master disconnected");
      digitalWrite(GREENLED, LOW);  // Turn off green LED
      digitalWrite(REDLED, HIGH);   // Turn on red LED
      break;
    }
  }
}

// bool inquireBluetoothDevices() {
//   // Start inquiry
//   sendCommand("AT+INQ");

//   // Wait for inquiry response
//   if (waitForResponse("+INQ:")) {
//     Serial.println("Inquiry completed");

//     // Read and parse inquiry results
//     while (true) {
//       String line = mySerial.readStringUntil('\n');
//       line.trim();

//       // Check if the line contains a Bluetooth address
//       if (line.startsWith("Inquiry")) {
//         continue; // Skip inquiry response lines
//       } else if (line == "OK") {
//         break; // End of inquiry response
//       } else {
//         Serial.println("Found Bluetooth device: " + line);

//         // Check if the Bluetooth address matches the specific address you are looking for
//         if (line.indexOf("0022,12,0232DB") != -1) {
//           Serial.println("Specific address found: " + line);

//           return true;
//         }
//       }
//     }
//   } else {
//     Serial.println("Inquiry failed");
//     return false;
//   }
// }

bool inquireBluetoothDevices() {
  
  sendCommand("AT+FSAD=0022,12,0232DB");
  if (waitForResponse("OK")) {
    Serial.println("ADDRESS FOUND");
  } else {
    Serial.println("ADDRESS NOT FOUND");
  }
}

void loop() {
  while (mySerial.available() > 0) {
    mySerial.read();
  }

  sendCommand("AT");
  if (waitForResponse("OK")) {
    Serial.println("AT command is working");

    removeConnectedDevices();

    inquireBluetoothDevices();

    // if (inquireBluetoothDevices()) {
    //   sendCommand("AT+ROLE?");
    //   if (waitForResponse("+ROLE:")) {
    //     int role = mySerial.parseInt();
    //     if (role == 0) { // Check if slave role
    //       Serial.println("Role: SLAVE");

    //       sendCommand("AT+STATE?");
    //       if (waitForResponse("+STATE:")) {
    //         processState("CONNECTED");
    //         Serial.println("Master is in range");

    //         digitalWrite(GREENLED, HIGH); // Turn on green LED
    //         digitalWrite(REDLED, LOW);    // Turn off red LED

    //         disconnectedDevices();

    //       } else {
    //         Serial.println("Bluetooth not found!");
    //         digitalWrite(REDLED, HIGH);
    //       }
    //     } else {
    //       Serial.println("Error: Invalid role");
    //     }
    //   }
    // }

    sendCommand("AT+ROLE?");
    if (waitForResponse("+ROLE:")) {
      int role = mySerial.parseInt();
      if (role == 0) { // Check if slave role
        Serial.println("Role: SLAVE");

        sendCommand("AT+STATE?");
        if (waitForResponse("+STATE:")) {
          processState("CONNECTED");
          Serial.println("Master is in range");

          digitalWrite(GREENLED, HIGH); // Turn on green LED
          digitalWrite(REDLED, LOW);    // Turn off red LED

          disconnectedDevices();

        } else {
          Serial.println("Bluetooth not found!");
          digitalWrite(REDLED, HIGH);
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


// #include <SoftwareSerial.h>

// SoftwareSerial bluetoothSerial(10, 11); // RX, TX pins for Bluetooth module

// void setup() {
//   pinMode(LED_BUILTIN, OUTPUT); // Initialize built-in LED for indicating success/failure
//   Serial.begin(9600); // Initialize serial communication for debugging
//   bluetoothSerial.begin(9600); // Initialize Bluetooth serial communication
  
//   // Wait for Bluetooth module to initialize
//   delay(1000);
  
//   Serial.println("Slave device ready.");
// }

// void loop() {
//   // Check if data is available from Bluetooth module
//   if (bluetoothSerial.available()) {
//     // Read data from Bluetooth module
//     char incomingByte = bluetoothSerial.read();
    
//     // Process incoming data
//     if (incomingByte == 'D') {
//       disconnectFromMaster();
//     }
//   }
// }

// void disconnectFromMaster() {
//   // Perform disconnection from master (you can put your actual disconnection logic here)
  
//   // In this example, we'll simulate a delay and then send a success message back to master
//   delay(1000); // Simulate disconnection process
  
//   // Send success message back to master
//   bluetoothSerial.println("Success");
// }
