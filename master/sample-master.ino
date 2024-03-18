#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

#define GREENLED 4
#define REDLED 6

char character;
int state = 0;
bool buttonstate = true;

void setup() {
  pinMode(GREENLED, OUTPUT);
  pinMode(REDLED, OUTPUT);

  Serial.begin(19200);
  while (!Serial) {}

  Serial.println("Welcome to RSSI Reciever");
  mySerial.begin(38400);

  initializeBluetooth();
}

void initializeBluetooth() {
  initializeCommand("AT");
  initializeCommand("AT+RMAAD");
  initializeCommand("AT+NAME?");
  initializeCommand("AT+ADDR?");
  initializeCommand("AT+ROLE=1");
  initializeCommand("AT+CMODE=0");
  initializeCommand("AT+BIND=0021,13,155F0");
  initializeCommand("AT+UART=9600,0,0");
  initializeCommand("AT+UART?");
}

void initializeCommand(const char* command) {
  mySerial.print(command);
  mySerial.print("\r\n");
  delay(100); // Adjust this delay if necessary

  // Check for response
  while (mySerial.available()) {
    char c = mySerial.read();
    Serial.write(c);
  }
}

void sendCommand(const char* command) {
  mySerial.println(command);
  delay(200); // Wait for response
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

  // Check if AT command is working
  sendCommand("AT");
  delay(200); 

  if (waitForResponse("OK")) {
    Serial.println("AT command is working");

    // Get role
    sendCommand("AT+ROLE?");
    delay(200); 

    if (waitForResponse("+ROLE:")) {
      int role = mySerial.parseInt();
      if (role == 1) {
        Serial.println("Role: SLAVE");

        // Check state
        sendCommand("AT+STATE?");
        delay(200); 

        if (waitForResponse("+STATE:")) {
          processState("READY");

          sendCommand("AT+STATE?");
          delay(200); 

          if (waitForResponse("+STATE:")) {
            processState("CONNECTED");
            digitalWrite(GREENLED, HIGH);
            digitalWrite(REDLED, LOW);

            mySerial.write('1');

            if(mySerial.available() > 0)
            { 
              state = mySerial.read();

              if (state == '1') 
              {
                Serial.println("CONNECTED BLUETOOTH DEVICE");
                Serial.print(state);
              }
              else if (state == '0') 
              {
                Serial.println("DISCONNECTED BLUETOOTH DEVICE");
                Serial.print(state);
              }
            } 

            // Disconnect
            sendCommand("AT+DISC");
            delay(200); 

            sendCommand("AT+STATE?");
            delay(200); 

            if (waitForResponse("+STATE:")) {
              processState("DISCONNECTED");

              Serial.println("Disconnected successfully");
              mySerial.write('0');
              digitalWrite(GREENLED, LOW);  // Turn off the green LED
              digitalWrite(REDLED, HIGH);   // Turn on the red LED
              delay(200); 

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
